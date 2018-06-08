#ifndef _NET_HANDLER_H_
#define _NET_HANDLER_H_

#include <string>
#include <chrono>
#include <thread>
#include <memory>
#include "socket.h"
#include "mtime.h"

namespace mulgame {

    struct InputStatePacket
    {
	// key presses
	// bits (right to left) : W, A, S, D, SPACE
	int8_t keyStates;
	// direction is needed
	glm::vec3 direction;

	// id of the packet
	uint64_t sequenceID;
    };
    
    class NetworkInterface
    {
    public:
	NetworkInterface(void)
	    : m_timer(1000000.0) // get time in milliseconds
	    {
	    }
	virtual ~NetworkInterface(void) = default;
	// network interfaces need to have direct access to the entities handler and terrain
	virtual void Launch(EntitiesHandler&, Terrain&, const std::string&, const std::string&) = 0;
    private:
	virtual void UDPThread(EntitiesHandler&, Terrain&) = 0;
	virtual void TCPThread(EntitiesHandler&, Terrain&) = 0;
    private:
	// for regular sending such as key presses
	Socket m_udpSock;
	std::unique_ptr<std::thread> m_udpCommunicationThread;
	// for important packets like username...
	Socket m_tcpSock;
	std::unique_ptr<std::thread> m_tcpCommunicationThread;

	Time m_timer;
    };

}

#endif /* _NET_HANDLER_H_ */
