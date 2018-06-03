#ifndef _CADDR_DATA_STRUCT_H_
#define _CADDR_DATA_STRUCT_H_

#include <optional>
#include "socket.h"
#include "memory"

namespace mulgame {

    struct ClientAddress
    {
	ClientAddress(void)
	    : id(-1)
	{
	}
	
	sockaddr_storage addr;
	int32_t id;
    };
    
    // database that contains all client addresses
    class ClientAddrDB
    {
    public:
	ClientAddrDB(void)
	    : m_clientAddresses(nullptr), m_max(0), m_size(0)
	{
 	}
	~ClientAddrDB(void)
	{
	    m_clientAddresses.reset();
	}

	void Init(uint32_t max)
	{
	    m_clientAddresses = std::make_unique<ClientAddress[]>(max);
	    m_max = max;
	}

	ClientAddress& operator[](uint32_t id)
	{
	    for(uint32_t i = 0; i < m_size; ++i)
	    {
		if(m_clientAddresses[i].id == id) return m_clientAddresses[i];
	    }
	}

	void Push(const sockaddr_storage& addr, int32_t id /* id from entities_handler vector of entities */)
	{
	    if(m_size != m_max)
	    {
		m_clientAddresses[++m_size].addr = addr;
		m_clientAddresses[m_size].id = id;
	    }
	}

	std::optional<ClientAddress*> Exists(const sockaddr_in* address)
	{
	    unsigned long numericAddr = address->sin_addr.s_addr;
	    for(uint32_t i = 0; i < m_size; ++i)
	    {
		sockaddr_in* clientAddress = (sockaddr_in*)(&m_clientAddresses[i].addr);
		unsigned long clientNumericAddr = clientAddress->sin_addr.s_addr;
		if(clientNumericAddr == numericAddr) return &m_clientAddresses[i];
	    }
	    return std::optional<ClientAddress*>();
	}

	uint32_t Size(void)
	{
	    return m_size;
	}

	uint32_t Max(void)
	{
	    return m_max;
	}
    private:
	uint32_t m_max;
	uint32_t m_size;
	// array to store all addresses
	std::unique_ptr<ClientAddress[]> m_clientAddresses; 
    };
    
}

#endif /* _CADDR_DATA_STRUCT_H_ */
