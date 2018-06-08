#ifndef _CLIENT_NET_H_
#define _CLIENT_NET_H_

#include "net_handler.h"

namespace mulgame {

    class ClientInterface
	: public NetworkInterface
    {
    public:
	ClientInterface(void) = default;
	void Launch(EntitiesHandler&, Terrain& const std::string&, const std::string&) override;
    private:
	void UDPThread(EntitiesHandler&, Terrain&) override;
	void TCPThread(EntitiesHandler&, Terrain&) override;

	// receive username given by server and other clients' usernames
	void ReceiveUsernames(EntitiesHandler& ehandler);
    private:
	
    };

}

#endif /* _CLIENT_NET_H_ */
