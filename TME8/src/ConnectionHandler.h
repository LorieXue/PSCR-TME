#ifndef SRC_CONNECTIONHANDLER_H_
#define SRC_CONNECTIONHANDLER_H_

#include "Socket.h"

namespace pr {

// une interface pour gerer la communication
class ConnectionHandler {
public:
	// gerer une conversation sur une socket
	virtual void handleConnection(Socket s) = 0;
	// une copie identique
	virtual ConnectionHandler * clone() const = 0;
	// pour virtual
	virtual ~ConnectionHandler() {}
};

//class HandlerClient : ConnectionHandler{
//public:
//	HandlerClient(){}
//	// gerer une conversation sur une socket
//	void handleConnection(Socket s){
//		int i;
//		read(s.getFD(),&i,sizeof(i));
//		i*=2;
//		write(s.getFD(),&i,sizeof(i));
//		s.close();
//	}
//	// une copie identique
//	ConnectionHandler * clone() const {
//		return (ConnectionHandler*) new HandlerClient();
//	}
//	// pour virtual
//	~HandlerClient() {}
//};


}
#endif /* SRC_CONNECTIONHANDLER_H_ */
