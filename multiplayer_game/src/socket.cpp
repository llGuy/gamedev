#include "socket.h"

#include <errno.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>

namespace mulgame {

    UDPSocket::UDPSocket(void)
    {
    }

    void UDPSocket::InitSocket(void)
    {
	m_handle = socket(m_address->ai_family, m_address->ai_socktype, m_address->ai_protocol);
	if(m_handle < 0) std::cerr << "socket() failed" << std::endl;
    }

    void UDPSocket::Connect(void)
    {
	int err = connect(m_handle, (sockaddr*)m_address->ai_addr, m_address->ai_addrlen);
	if(err < 0) std::cerr << "unable to connect() : " << strerror(err) << std::endl;
    }

    void UDPSocket::Send(Byte* data, uint32_t dataSize)
    {
	int32_t bytes = send(m_handle, data, dataSize, 0);
	if(bytes < 0) std::cerr << "send() failed\n";
	else if(bytes != dataSize) std::cerr << "send() send unexpected number of bytes\n";
    }

    void UDPSocket::Receive(Byte* data, uint32_t maxSize)
    {
	int32_t bytes = recv(m_handle, data, maxSize, 0);
    }

    addrinfo UDPSocket::FillCriteria(int family, int flags, int socktype, int protocol)
    {
	addrinfo criteria;
	memset(&criteria, 0, sizeof(criteria));
	criteria.ai_family = family;
	criteria.ai_flags = flags;
	criteria.ai_socktype = socktype;
	criteria.ai_protocol = protocol;
	return criteria;
    }

    void UDPSocket::GetAddressInformation(int family, int flags, int socktype, int protocol, const char* address, const char* port)
    {
	addrinfo criteria = FillCriteria(family, flags, socktype, protocol);
	int err = getaddrinfo(address, port, &criteria, &m_address);
	if(err != 0) std::cerr << "getaddrinfo() failed : " << gai_strerror(err) << std::endl;
    }

    void UDPSocket::Sendto(Byte* data, uint32_t dataSize, addrinfo* address)
    {
	addrinfo* addr = address == nullptr ? m_address : address;
	socklen_t addressSize = addr->ai_addrlen;
        int32_t bytes = sendto(m_handle, data, dataSize, 0, (sockaddr*)addr->ai_addr, addressSize);
	if(bytes < 0) std::cerr << "sendto() failed\n";
	else if(bytes != dataSize) std::cerr << "sent unexpected number of bytes\n";
    }
    
    sockaddr_storage UDPSocket::ReceiveFrom(Byte* data, uint32_t maxSize)
    {
	sockaddr_storage client;
	socklen_t clientSize = sizeof(client);
	int32_t bytes = recvfrom(m_handle, data, maxSize, 0, (sockaddr*)&client, &clientSize);
	if(bytes < 0) std::cerr << "recvfrom() failed\n";
	return client;
    }
    
}
