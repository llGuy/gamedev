#ifndef _SOCKET_H_
#define _SOCKET_H_

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/types.h>
#include <stdint.h>
#include <string>

namespace mulgame {

    using Byte = int8_t;
    
    // udp socket
    class UDPSocket
    {
    public:
	// only need the ip family
	UDPSocket(void);
	void InitSocket(void);
	void Connect(void);
	
	void Send(Byte* data, uint32_t dataSize);
	void Receive(Byte* data, uint32_t maxSize);

	void Sendto(Byte* data, uint32_t dataSize, addrinfo* address = nullptr);
	sockaddr_storage ReceiveFrom(Byte* data, uint32_t maxSize);
    public:
	// new functions
	addrinfo FillCriteria(int family, int flags, int socktype, int protocol);
	void GetAddressInformation(int, int, int, int, const char* address, const char* port);
    private:
	addrinfo* m_address;
	int32_t m_handle;
    };
    
}

#endif /* _SOCKET_H_ */
