#include <functional>
#include "request.h"
#include "network_handler.h"
#include <string>

namespace mulgame {

    NetworkHandler::NetworkHandler(mode_t mode, EntitiesHandler& ehandler, Terrain& terrain)
	: m_mode(mode)
    {
	if(m_mode == SERVER_MODE)
	{
//	    m_clientAddresses.Init(8);
	    Launch("5000", ehandler, terrain);
	}
	else
	{
	    Launch("192.168.1.230", "5000", ehandler, terrain);
	}
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

	m_clientCommunicationThread = std::make_unique<std::thread>([this, &ehandler] { TCPThread(m_tcpSock, ehandler); });

	// setup UDP socket for client
	m_udpSock.Init(AF_INET, 0, SOCK_DGRAM, IPPROTO_UDP, serverName.c_str(), port.c_str());
	m_udpSock.Connect();

//	m_udpThread = std::make_unique<std::thread>([this, &ehandler, &terrain] {  });
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
	    message.insert(0, 1, static_cast<int8_t>(serverreq_t::CONNECT));
	    socket.Send((const Byte*)message.c_str(), message.size());

	    auto& entity = ehandler.PushEntity(glm::vec3(5.0f), glm::vec3(1.0f));
	    entity.Username() = username;
	    
	    m_connectionThreads.emplace_back( std::make_unique<std::thread>([this, &ehandler, &socket] { TCPThread(socket, ehandler); }) );
	}
    }

    void NetworkHandler::TCPThread(Socket s, EntitiesHandler& ehandler)
    {
	for(;;)
	{
	    Byte message[512];
	    if(s.Receive(message, 512))
	    {
		// print message
		std::cout << message << std::endl;
	    }
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
	    
	}
    }

    void NetworkHandler::ParseClientUDPMessages(const std::string& message, EntitiesHandler& ehandler, Terrain& terrain)
    {
	MSGParser parser(message);
	// type of message
	std::string username ;
	parser.ReadNext(CHAR_DELIMITER, username);
	// determine type of message and parse according to type
	auto ent = ehandler.EViaUsername(username);
	if(ent.has_value())
	{
	    // updates the position and direction
	    (ent.value())->Position() = parser.ReadNext<glm::vec3>(CHAR_DELIMITER);
	    (ent.value())->Direction() = parser.ReadNext<glm::vec3>(CHAR_DELIMITER);
	}
    }

    void NetworkHandler::SendPlayerDatatoServer(Byte* data, uint32_t size)
    {
	m_udpSock.Send(data, size);
    }
    
}
