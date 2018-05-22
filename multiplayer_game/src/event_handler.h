#ifndef _EVENT_FORWARDER_H_
#define _EVENT_FORWARDER_H_

#include <tuple>

#include "event.h"
#include "entities_handler.h"

namespace mulgame {

    template<typename... _Elems>
    class EventForwarder
    {
    public:
	// event handler needs all the data
	// that it should be able to control
	// in the case that it receives an event
	template<typename... _Els>
	EventForwarder(_Els*... elements)
	    : m_elements(elements...)
	{
	}

	template<typename _Elem, typename... _Data>
	void Handle(_Data&&... data)
	{
	    auto element = std::get<_Elem*>(m_elements);
	    element->Handle(std::forward<_Data>(data)...);
	}
    private:
	// tuple stores all the elements that the
	// event handler can forward events to
	std::tuple<_Elems*...> m_elements;
    };

    // event forward can forward events to the entities handler
    using MULGEventForwarder = EventForwarder<EntitiesHandler>;
    
}

#endif /* _EVENT_HANDLER_H_ */
