#ifndef _EVENT_H_
#define _EVENT_H_

namespace mulgame {

    enum class key_t
    {
	W, A, S, D,

	SPACE, LSHIFT,

	INVALID
    };
    
    enum class event_t
    {
	MOVE_FORWARD,

	INVALID
    };

}

#endif /* _EVENT_H_ */
