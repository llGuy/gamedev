#include <functional>

#include "request.h"
#include "network_handler.h"
#include <string>
#include "vec_io.h"

namespace mulgame {

    NetworkHandler::NetworkHandler(mode_t mode, EntitiesHandler& ehandler, Terrain& terrain)
	: m_mode(mode), m_received(true)
    {
    }
    
    void NetworkHandler::Launch(const std::string& port, EntitiesHandler& ehandler, Terrain& terrain)
    {
	// setup TCP socket
	m_tcpSock.Init(AF_INET, AI_PASSIVE, SOCK_STREAM, IPPROTO_TCP, nullptr, port.c_str());
	m_tcpSock.Bind();
	m_tcpSock.Listen(MAX_PENDING);

	// start accepting connections
	m_acceptThread = std::make_unique<std::thread>([this, &ehandler] { AcceptThread(ehandler); });

	// setup UDP socket
	m_udpSock.Init(AF_INET, AI_PASSIVE, SOCK_DGRAM, IPPROTO_UDP, nullptr, port.c_str());
	m_udpSock.Bind();

	m_udpThread = std::make_unique<std::thread>([this, &ehandler, &terrain] { ServerUDPThread(ehandler, terrain); });
    } 

    // this launch function applies for clients only
    void NetworkHandler::Launch(const std::string& serverName, const std::string& port, EntitiesHandler& ehandler, Terrain& terrain)
    {
	// connect tcp socket to server for important requests
	m_tcpSock.Init(AF_INET, 0, SOCK_STREAM, IPPROTO_TCP, serverName.c_str(), port.c_str());
	m_tcpSock.Connect();

	// receive username sent by server
	Byte message[512];
	if(int32_t size = m_tcpSock.Receive(message, 512); size > 0)
	{
	    uint32_t boundEntity = ehandler.Cam().BoundEntity();
	    MSGParser parser(message, size);

	    ehandler[boundEntity].Username() = "";
	    parser.ReadNext(CHAR_DELIMITER, ehandler[boundEntity].Username());
	    
	    std::cout << ehandler[boundEntity].Username() << std::endl;
	    
	    for(; !parser.Max();)
	    {
		std::string username;
		parser.ReadNext(CHAR_DELIMITER, username);
		std::cout << "pushed player : " << username << std::endl;
		ehandler.PushEntity(username);
	    }
	}
	m_clientCommunicationThread = std::make_unique<std::thread>([this, &ehandler] { TCPThread(m_tcpSock, ehandler); });
	// setup UDP socket for client
	m_udpSock.Init(AF_INET, 0, SOCK_DGRAM, IPPROTO_UDP, serverName.c_str(), port.c_str());
	m_udpSock.Connect();
	// launch udp thread
	m_udpThread = std::make_unique<std::thread>([this, &ehandler, &terrain] { ClientUDPThread(ehandler, terrain); });
    }

    void NetworkHandler::AcceptThread(EntitiesHandler& ehandler)
    {
	for(;;)
	{
	    // accept new connection
	    auto socket = m_tcpSock.Accept();
	    std::cout << "handling client" << std::endl;
	    // tell the client its username
	    MSGEncoder mencoder;
	    std::string username = std::string("player") + std::to_string(ehandler.Size());
	    mencoder.PushString(username);
	    for(uint32_t i = 0; i < ehandler.Size(); ++i) mencoder.PushString(ehandler[i].Username());
	    
	    socket.Send(mencoder.Data(), mencoder.Size());

	    auto& entity = ehandler.PushEntity(glm::vec3(5.0f), glm::vec3(1.0f));
	    entity.Username() = username;
	    
	    m_connectionThreads.emplace_back( std::make_unique<std::thread>([this, &ehandler, &socket] { TCPThread(socket, ehandler); }) );
	}
    }

    void NetworkHandler::TCPThread(Socket s, EntitiesHandler& ehandler)
    {
	if(m_mode == CLIENT_MODE)
	{
	    // receive username
	}
    }

    void NetworkHandler::ServerUDPThread(EntitiesHandler& ehandler, Terrain& terrain)
    {
	static constexpr uint32_t BUFFER_MAX_SIZE = 512;
	// main loop
	for(;;)
	{
	    // receive message
	    Byte messageBuffer[BUFFER_MAX_SIZE] { 0 };
	    auto client = m_udpSock.ReceiveFrom(messageBuffer, BUFFER_MAX_SIZE);
	    // parse message

	    MSGParser parser(messageBuffer, client.size /* message size */);
	    auto playerID = ParseUDPMessage(ehandler, terrain, parser);
	    m_addresses[playerID] = client.address;
//	    SendAllPlayersDatatoClient(ehandler, terrain, playerID, client.address);
	    // send all players' data to all players
//	    UpdatePlayer(ehandler, terrain, playerID, client.address);
	    for(auto address = m_addresses.begin(); address != m_addresses.end(); ++address)
	    {
		UpdatePlayer(ehandler, terrain, address->first, address->second);
	    }
	}
    }

    void NetworkHandler::ClientUDPThread(EntitiesHandler& ehandler, Terrain& terrain)
    {
	static constexpr uint32_t BUFFER_MAX_SIZE = 512;

	for(;;)
	{
	    // send or receive
	    auto received = ReceiveFromServer();
	    if(received.has_value())
	    {
		MSGParser& parser = received.value();

		std::lock_guard<std::mutex> guard(m_ehandlerMutex);
		for(; !parser.Max();)
		    ParseUDPMessage(ehandler, terrain, parser);
	    }

	    MSGEncoder encoder;
	    Entity& boundPlayer = ehandler[ehandler.Cam().BoundEntity()];
	    EncodePlayerData(encoder, boundPlayer, ehandler, terrain);
	    
	    Byte* bytes = encoder.Data();
	    m_udpSock.Send(bytes, encoder.Size());
	}
    }

    void NetworkHandler::EncodePlayerData(MSGEncoder& encoder, Entity& player,
	 EntitiesHandler& ehandler, Terrain& terrain)
    {
	encoder.PushString(player.Username());
	encoder.PushBytes(player.Position(), player.Direction());
	// push if player is shooting and terraforming
	bool shot = ehandler.BoundEntityShot();
	bool terraformed = (player.Terraforming() != -1);
	int8_t flags = shot + (terraformed << 1);
	encoder.PushBytes(flags);
	if(terraformed)
	{
	    decltype(auto) fp = terrain.FP(player.Terraforming());
	    encoder.PushBytes(fp);
	}
	else encoder.PushBytes(ForcePoint{});
    }

    void NetworkHandler::UpdatePlayer(EntitiesHandler& ehandler, Terrain& terrain, uint32_t playerID, ClientAddress& addr)
    {
	MSGEncoder encoder;
	for(uint32_t i = 0; i < ehandler.Size(); ++i)
	{
	    Entity& ent = ehandler[i];
	    // only sned other people's data
	    if(ent.ID() != playerID)
	    {
		EncodePlayerData(encoder, ent, ehandler, terrain);
	    }
	}
	// keep address in map updated
	m_addresses[playerID] = addr;
	m_udpSock.Sendto(encoder.Data(), encoder.Size(), addr.address);
    }

    std::optional<MSGParser> NetworkHandler::ReceiveFromServer(void)
    {
	Byte buffer[512];
	auto bytes = m_udpSock.Receive(buffer, 512, MSG_DONTWAIT);

	if(bytes > 0)
	{
	    m_received = true;
	    return MSGParser(buffer, bytes);
	}
	else
	{
	    return std::optional<MSGParser>{};
	}
    }
    
    uint32_t NetworkHandler::ParseUDPMessage(EntitiesHandler& ehandler, Terrain& terrain, MSGParser& parser)
    {
	auto getBit = [] (int8_t byte, uint32_t rshift) -> bool
	{
	    return (byte >> rshift) & 0b00000001;
	};
	
	std::string username;
	parser.ReadNext(CHAR_DELIMITER, username);
	
	// determine type of message and parse according to type
	auto ent = ehandler.EViaUsername(username);
	if(ent.has_value())
	{
	    // updates the position and direction
	    (ent.value())->Position() = parser.ReadNext<glm::vec3>(CHAR_DELIMITER);
	    (ent.value())->Direction() = parser.ReadNext<glm::vec3>(CHAR_DELIMITER);

	    int8_t flags = parser.ReadNext<int8_t>(CHAR_DELIMITER);
	    bool shot = getBit(flags, 0);
	    bool tf = getBit(flags, 1);
	    if(shot)
	    {
		ehandler.Handle(ability_t::SHOOT, ent.value()->ID());
	    }
	    ForcePoint fp = parser.ReadNext<ForcePoint>(CHAR_DELIMITER);
	    terrain.Handle(fp, *(ent.value()), tf);
	    return ent.value()->ID();
	}
	return -1;
    }

    void NetworkHandler::SendPlayerDatatoServer(std::vector<Byte>& data, uint32_t size)
    {
	// make sure there is the size of the packet
	m_udpSock.Send(&data[0], size);
    }
    

}
