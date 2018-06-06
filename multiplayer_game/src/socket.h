#ifndef _SOCKET_H_
#define _SOCKET_H_

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/types.h>
#include <stdint.h>
#include <string>
#include "client_addr.h"

namespace mulgame {

    using Byte = int8_t;
    
    // udp socket
    class Socket
    {
    public:
	// only need the ip family
	Socket(void);
	Socket(int32_t);
//	Socket(int32_t handle, sockaddr*, int32_t);
	
	void InitSocket(int32_t, int32_t, int32_t);
	void Connect(void);
	void Bind(void);
	void Listen(int32_t max);
	Socket Accept(void) const;

	void Send(const Byte* data, uint32_t dataSize);
	int32_t Receive(Byte* data, uint32_t maxSize, int32_t flags = 0);

	void Sendto(const Byte* data, uint32_t dataSize, addrinfo* address = nullptr);
	void Sendto(const Byte* data, uint32_t dataSize, const sockaddr_in& address);
	
	struct ReceiveFromRet
	{
	    ClientAddress address;
	    int32_t size;
	};

        ReceiveFromRet ReceiveFrom(Byte* data, uint32_t maxSize);
    public:
	// new functions
	addrinfo FillCriteria(int family, int flags, int socktype, int protocol);
	void Init(int family, int flags, int socktype, int protocol, const char* address, const char* port);
	void Free(void);
    private:
	union
	{
	    sockaddr m_address;
	    sockaddr_in m_addressin;
	};
	// may or may not have addrinfo
	addrinfo* m_ainfo;
	int32_t m_handle;
    };
    
}

#endif /* _SOCKET_H_ */
