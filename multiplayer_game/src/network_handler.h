#ifndef _NETWORK_HANDLER_H_
#define _NETWORK_HANDLER_H_

#include <thread>
#include <memory>
#include <thread>
#include <glm/glm.hpp>
#include "socket.h"
#include "msg_prototype.h"
#include "event_handler.h"
#include "entities_handler.h"
#include "terrain.h"

#include "msg_encoder.h"
#include "msg_parser.h"

namespace mulgame {

    enum mode_t
    : int8_t
    {
	CLIENT_MODE = 'c',

	SERVER_MODE = 's'
    };

    class NetworkHandler
    {
    public:
	NetworkHandler(void) = default;
	NetworkHandler(mode_t mode, EntitiesHandler& ehandler, Terrain& terrain);
	void Launch(const std::string& port, EntitiesHandler&, Terrain& terrain);
	void Launch(const std::string& serverName, const std::string& port, EntitiesHandler&, Terrain& terrain);

	void SendPlayerDatatoServer(Byte* data, uint32_t size);

	mode_t Mode(void) { return m_mode; };
    private:
	void AcceptThread(EntitiesHandler& ehandler);
	void TCPThread(Socket sock, EntitiesHandler& ehandler);

	void ServerUDPThread(EntitiesHandler& ehandler, Terrain& terrain);
    private:
	void ParseClientUDPMessages(const std::string& message, EntitiesHandler& ehandler, Terrain&);
    private:
	static constexpr int32_t MAX_PENDING = 8;
	
	mode_t m_mode;
	Socket m_udpSock;
	Socket m_tcpSock;

	// thread that accepts connections to the server
	std::unique_ptr<std::thread> m_acceptThread;
	// thread that the client needs to use to communicate with the server
	std::unique_ptr<std::thread> m_clientCommunicationThread;
	std::unique_ptr<std::thread> m_udpThread;
	std::vector<std::unique_ptr<std::thread>> m_connectionThreads;
    };

}

#endif /* _NETWORK_HANDLER_H_ */
