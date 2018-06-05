#include "socket.h"

#include <errno.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>

namespace mulgame {

    Socket::Socket(void)
    {
    }

    Socket::Socket(int32_t handle)
	: m_handle(handle)
    {
    }	

/*    Socket::Socket(int32_t handle, sockaddr* address, int32_t length)
	: m_address(addr), m_handle(handle)
    {
    }*/

    void Socket::InitSocket(int32_t family, int32_t socktype, int32_t protocol)
    {
	m_handle = socket(family, socktype, protocol);
	if(m_handle < 0) std::cerr << "socket() failed" << std::endl;
    }

    void Socket::Connect(void)
    {
	int err = connect(m_handle, m_ainfo->ai_addr, m_ainfo->ai_addrlen);
	if(err < 0) std::cerr << "unable to connect() : " << strerror(err) << std::endl;
    }

    Socket Socket::Accept(void) const
    {
	sockaddr_storage addr;
	socklen_t addressSize = sizeof(addr);
	int32_t socket = accept(m_handle, (sockaddr*)&addr, &addressSize);
	return Socket(socket);
    }
    
    void Socket::Send(const Byte* data, uint32_t dataSize)
    {
	int32_t bytes = send(m_handle, data, dataSize, 0);
//	if(bytes < 0) std::cerr << "send() failed\n";
//	else if(bytes != dataSize) std::cerr << "send() send unexpected number of bytes\n";
    }

    bool Socket::Receive(Byte* data, uint32_t maxSize)
    {
	int32_t bytes = recv(m_handle, data, maxSize, 0);
	return bytes > 0;
    }

    addrinfo Socket::FillCriteria(int family, int flags, int socktype, int protocol)
    {
	addrinfo criteria;
	memset(&criteria, 0, sizeof(criteria));
	criteria.ai_family = family;
	criteria.ai_flags = flags;
	criteria.ai_socktype = socktype;
	criteria.ai_protocol = protocol;
	return criteria;
    }

    void Socket::Init(int family, int flags, int socktype, int protocol, const char* address, const char* port)
    {
	InitSocket(family, socktype, protocol);
	
	addrinfo criteria = FillCriteria(family, flags, socktype, protocol);
	addrinfo* addr;
	int err = getaddrinfo(address, port, &criteria, &addr);
	if(err != 0) std::cerr << "getaddrinfo() failed : " << gai_strerror(err) << std::endl;
	else m_ainfo = addr;
    }

    void Socket::Sendto(const Byte* data, uint32_t dataSize, addrinfo* address)
    {
	socklen_t addressSize = m_ainfo->ai_addrlen;
        int32_t bytes = sendto(m_handle, data, dataSize, 0, m_ainfo->ai_addr, addressSize);
	if(bytes < 0) std::cerr << "sendto() failed\n";
	else if(bytes != dataSize) std::cerr << "sent unexpected number of bytes\n";
    }
    
    Socket::ReceiveFromRet Socket::ReceiveFrom(Byte* data, uint32_t maxSize)
    {
	ClientAddress client;
	socklen_t clientSize = sizeof(client.address);
	int32_t bytes = recvfrom(m_handle, data, maxSize, 0, (sockaddr*)&client.address, &clientSize);
	
	if(bytes < 0) std::cerr << "recvfrom() failed\n";
	return { client, bytes };
    }

    void Socket::Bind(void)
    {
	auto err = bind(m_handle, m_ainfo->ai_addr, m_ainfo->ai_addrlen);
	if(err < 0) std::cerr << "bind() failed" << std::endl;
    }

    void Socket::Listen(int32_t max)
    {
	auto err = listen(m_handle, max);
	if(err < 0) std::cerr << "listen() failed" << std::endl;
    }

    void Socket::Free(void)
    {
	freeaddrinfo(m_ainfo);
    }
    
}
