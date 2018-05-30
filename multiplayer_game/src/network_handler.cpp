#include "network_handler.h"

namespace mulgame {

    void NetworkHandler::Launch(const std::string& port)
    {
	
    }
    
    void NetworkHandler::Launch(const std::string& serverName, const std::string& port)
    {
	// connect the socket so that we can send and received from server
	m_udpSock.GetAddressInformation(AF_UNSPEC, 0, SOCK_DGRAM, IPPROTO_UDP, "192.168.1.230", "5000");
	m_udpSock.InitSocket();
	m_udpSock.Connect();

	m_msgPrototype.Fill(glm::vec3(0.0f), 0);
	m_udpSock.Send(m_msgPrototype.Data(), m_msgPrototype.Size());
    }

}
