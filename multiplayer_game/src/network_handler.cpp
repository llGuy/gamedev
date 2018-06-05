#include <functional>
#include "request.h"
#include "network_handler.h"
#include <string>
#include "vec_io.h"

namespace mulgame {

    NetworkHandler::NetworkHandler(mode_t mode, EntitiesHandler& ehandler, Terrain& terrain)
	: m_mode(mode)
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
	if(m_tcpSock.Receive(message, 512))
	{
	    // print message
	    std::string username ((const char*)message);
	    std::cout << "my username : " << username << std::endl;
	    uint32_t boundEntity = ehandler.Cam().BoundEntity();
	    ehandler[boundEntity].Username() = username;
	}

	m_clientCommunicationThread = std::make_unique<std::thread>([this, &ehandler] { TCPThread(m_tcpSock, ehandler); });

	// setup UDP socket for client
	m_udpSock.Init(AF_INET, 0, SOCK_DGRAM, IPPROTO_UDP, serverName.c_str(), port.c_str());
	m_udpSock.Connect();

//	m_udpThread = std::make_unique<std::thread>([this, &ehandler] { TCPThread() });
    }

    void NetworkHandler::AcceptThread(EntitiesHandler& ehandler)
    {
	for(;;)
	{
	    // accept new connection
	    auto socket = m_tcpSock.Accept();
	    std::cout << "handling client" << std::endl;
	    // tell the client its username
	    std::string username = std::string("player") + std::to_string(ehandler.Size());
	    std::string message = username;
//	    message.insert(0, 1, static_cast<int8_t>(serverreq_t::CONNECT));
	    socket.Send((const Byte*)message.c_str(), message.size());

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
	    ParseClientUDPMessages(messageBuffer, client.size, ehandler, terrain);
	}
    }

    void NetworkHandler::ParseClientUDPMessages(Byte* bytes, int32_t size, EntitiesHandler& ehandler, Terrain& terrain)
    {
	MSGParser parser(bytes, size);
	// type of message
	std::string username;
	parser.ReadNext(CHAR_DELIMITER, username);

	std::cout << "received message from " << username << std::endl;
	
	// determine type of message and parse according to type
	auto ent = ehandler.EViaUsername(username);
	if(ent.has_value())
	{
	    std::cout << ent.value()->Username() << " is being edited" << std::endl;
	    // updates the position and direction
	    (ent.value())->Position() = parser.ReadNext<glm::vec3>(CHAR_DELIMITER);
	    VOut((ent.value())->Position() );
	    (ent.value())->Direction() = parser.ReadNext<glm::vec3>(CHAR_DELIMITER);
	}
    }

    void NetworkHandler::SendPlayerDatatoServer(std::vector<Byte>& data, uint32_t size)
    {
	// make sure there is the size of the packet
	m_udpSock.Send(&data[0], size);
    }
    
}
