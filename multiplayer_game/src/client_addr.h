#ifndef _CLIENT_ADDR_H_
#define _CLIENT_ADDR_H_

#include <netinet/in.h>

namespace mulgame {

    struct ClientAddress
    {
	bool isLocal;
	sockaddr_in address;

	ClientAddress(bool local)
	{
	}

	ClientAddress(void) = default;
	bool operator==(const ClientAddress& other)
	{
	    return address.sin_port == other.address.sin_port &&
		address.sin_addr.s_addr == other.address.sin_addr.s_addr;
	}
	const ClientAddress& operator=(const ClientAddress& other)
	    {
		address = other.address;
	    }
    };

}

#endif /* _CLIENT_ADDR_H_ */
