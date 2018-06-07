#ifndef _CADDR_DATA_STRUCT_H_
#define _CADDR_DATA_STRUCT_H_

#include <optional>
#include <unordered_map>
#include "socket.h"
#include "memory"

namespace mulgame {

    class ClientAddressDataBase
    {
    public:
	ClientAddressDataBase(void) = default;
	ClientAddress& 
    private:
	std::unordered_map<uint32_t, ClientAddress> m_addresses;
    };
    
}

#endif /* _CADDR_DATA_STRUCT_H_ */
