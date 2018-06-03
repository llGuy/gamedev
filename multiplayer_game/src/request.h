#ifndef _REQUEST_H_
#define _REQUEST_H_

#include <stdint.h>

namespace mulgame {

    enum class clientreq_t
    : int8_t
    {
	CONNECT = 'c',

	INVALID
    };

    enum class serverreq_t
    : int8_t
    {
	CONNECT = 'c',

	INVALID
    };

}

#endif /* _REQUEST_H_ */
