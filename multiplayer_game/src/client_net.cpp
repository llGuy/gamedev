#include "client_net.h"

namespace mulgame {

    void ClientInterface::Launch(EntitiesHandler& eh, Terrain& ter, const std::string& port, const std::string& addr)
    {
	// setup tcp socket
	m_tcpSock.Init(AF_INET, 0, SOCK_STREAM, IPPROTO_TCP, addr.c_str(), port.c_str());
	m_tcpSock.Connect();

	ReceiveUsernames(eh);

	m_tcpCommunicationThread = std::make_unique<std::thread>([this, &eh, &ter] { TCPThread(eh, ter); });

	// setup udp socket
	m_udpSock.Init(AF_INET, 0, SOCK_DGRAM, IPPROTO_UDP, addr.c_str(), port.c_str());
	m_udpSock.Connect();
	m_udpCommunicationThread = std::make_unique<std::thread>([this, &eh, &ter] { UDPThread(eh, ter); });
    }
    
    void ClientInterface::UDPThread(EntitiesHandler&, Terrain&)
    {
	m_timer.StartTimer();
	for(;;)
	{
	    if(m_timer.Difference() > 30.0 /* 30.0 milliseconds */)
	    {
		InputStatePacket isp;
		
		
		// reset the timer
		m_timer.Update();
	    }
	}
    }
    
    void ClientInterface::TCPThread(EntitiesHandler&, Terrain&)
    {

    }

    void ClientInterface::ReceiveUsernames(EntitiesHandler& ehandler)
    {
	static constexpr int32_t MAX_SIZE = 64;
	Byte message[MAX_SIZE];
	if(int32_t size = m_tcpSock.Receive(message, MAX_SIZE); size > 0)
	{
	    MSGParser parser(message, size);
	    
	    uint32_t boundEntity = ehandler.Cam().BoundEntity();
	    std::string& username = ehandler[boundEntity].Username();
	    username = "";
	    parser.ReadNext(CHAR_DELIMITER, username);

	    std::cout << "username : " << username << std::endl;

	    for(; !parser.Max();)
	    {
		std::string user;
		parser.ReadNext(CHAR_DELIMITER, user);
		std::cout << "pushed : " << user << std::endl;
		ehandler.PushEntity(user);
	    }
	}
    }

}
