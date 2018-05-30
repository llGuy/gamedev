#ifndef _NETWORK_HANDLER_H_
#define _NETWORK_HANDLER_H_

#include <thread>
#include <glm/glm.hpp>
#include "socket.h"
#include "msg_prototype.h"
#include "event_handler.h"
#include "entities_handler.h"
#include "terrain.h"

namespace mulgame {

    enum mode_t
    {
	CLIENT_MODE,

	SERVER_MODE
    };
    
    class NetworkHandler
    {
    public:
	NetworkHandler(void) = default;
	void Launch(const std::string& port);
	void Launch(const std::string& serverName, const std::string& port);
	// thread needs to be able to access data from EntitiesHandler and Terrain
	void LaunchThread(EntitiesHandler& entitiesHandler, Terrain& terrain);
    private:
	void CommunicationThread(EntitiesHandler& entitiesHandler, Terrain& terrain);
    private:
	UDPSocket m_udpSock;
	MSGPrototype<glm::vec3> m_msgPrototype;

	// communication thread needs :
	// - entities handler
	// - terrain
	std::thread m_communicationThread;
    };

}

#endif /* _NETWORK_HANDLER_H_ */
