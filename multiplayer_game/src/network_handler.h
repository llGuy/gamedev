#ifndef _NETWORK_HANDLER_H_
#define _NETWORK_HANDLER_H_

#include <thread>
#include <mutex>
#include <memory>
#include <thread>
#include <unordered_map>
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
	void SendPlayerDatatoServer(std::vector<Byte>& data, uint32_t size);
	uint32_t ParseUDPMessage(EntitiesHandler& ehandler, Terrain&, MSGParser&);
	std::optional<MSGParser> ReceiveFromServer(void);
	
	std::mutex& EHandlerMutex(void) { return m_ehandlerMutex; };
	bool& ServerReturnedWithMessage(void) { return m_received; };
	mode_t Mode(void) { return m_mode; };
    private:
	void EncodePlayerData(MSGEncoder& encoder, Entity& player, EntitiesHandler& ehandler, Terrain& terrain);
	void AcceptThread(EntitiesHandler& ehandler);
	void TCPThread(Socket sock, EntitiesHandler& ehandler);

	void ServerUDPThread(EntitiesHandler& ehandler, Terrain& terrain);
	void ClientUDPThread(EntitiesHandler& ehandler, Terrain& terrain);
    private:
	void UpdatePlayer(EntitiesHandler&, Terrain&, uint32_t playerID, ClientAddress& addr);
    private:
	static constexpr int32_t MAX_PENDING = 8;
	
	mode_t m_mode;
	Socket m_udpSock;
	Socket m_tcpSock;

	// only the server has this
	std::unordered_map<uint32_t, ClientAddress> m_addresses;
	
	// thread that accepts connections to the server
	std::unique_ptr<std::thread> m_acceptThread;
	// thread that the client needs to use to communicate with the server
	std::unique_ptr<std::thread> m_clientCommunicationThread;
	// client and server will have udp thread
	std::unique_ptr<std::thread> m_udpThread;
	std::vector<std::unique_ptr<std::thread>> m_connectionThreads;

	bool m_received;

	/*
	  mutex only needs to locked when the entities
	  because they entities data only get accessed when they 
	  get rendered
	 */
	std::mutex m_ehandlerMutex;
	std::mutex m_terrainMutex;
    };

}

#endif /* _NETWORK_HANDLER_H_ */
