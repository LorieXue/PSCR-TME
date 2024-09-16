#ifndef SRC_TCPSERVER_H_
#define SRC_TCPSERVER_H_

#include <thread>
#include <vector>
#include <sys/select.h>
#include <signal.h>
#include "ServerSocket.h"
#include "ConnectionHandler.h"
#include <functional>
#include <unistd.h>
#include <iostream>
#include <cstring>


namespace pr {

// un serveur TCP, la gestion des connections est déléguée
class TCPServer {
	ServerSocket * ss; // la socket d'attente si elle est instanciee
	ConnectionHandler * handler; // le gestionnaire de session passe a la constru
	std::vector<std::thread> threads;
	static int stopper[2];
	static int selected;


public :
	TCPServer(ConnectionHandler * handler): ss(nullptr),handler(handler) {
	}

	static void signalHandler(int sig) {
	    if (sig == SIGINT) {
	    	const char* message = "stop";
	        write(stopper[1], message, std::strlen(message));
	    }
	}

	// Tente de creer une socket d'attente sur le port donné
	bool startServer (int port){
		int i;
	    if (pipe(stopper) == -1) {
	        perror("pipe error");
	        exit(EXIT_FAILURE);
	    }
	    signal(SIGINT, signalHandler);
		pr::ServerSocket serv = ServerSocket(port);
		if(!serv.isOpen()){
		    close(stopper[0]);
		    close(stopper[1]);
			exit(-1);
		}
		ss = &serv;
		while(true){
			fd_set set;
			FD_ZERO(&set);
			FD_SET(ss->getFD(),&set);
			std::cout << ss->getFD() <<std::endl;
			FD_SET(stopper[1],&set);
			int maxfd = std::max(ss->getFD(), stopper[1]) + 1;
			std::cout << "En attente" << std::endl;
			std::cout << "selecting" <<std::endl;
			if ((selected = ::select(maxfd, &set, nullptr, nullptr, nullptr)) == -1) {
//			        std::cerr << "Error in select." << std::endl;
//			        this->stopServer();
//			        return false;
			}
			if(FD_ISSET(stopper[1], &set)){
				std::cout << "stop" <<std::endl;
				this->stopServer();
				return false;
			}
			if(FD_ISSET(ss->getFD(), &set)){
				std::cout << "need connect" <<std::endl;
				std::cout << ss->getFD() <<std::endl;
				pr::Socket sc = ss->accept();
				std::cout << "accepted" <<std::endl;
				handler->handleConnection(sc);
				threads.emplace_back([](pr::Socket sc,pr::ConnectionHandler* handler){
					handler->handleConnection(sc);
				},sc,handler);
			}
			std::cout << "????" <<std::endl;

		}
		return true;
	}

	// stoppe le serveur
	void stopServer () {
		ss->close();
	    close(stopper[0]);
	    close(stopper[1]);
		delete handler;
	}

	~TCPServer(){
		for(auto& t: threads){
			t.join();
		}
	}
};

int TCPServer::stopper[2] = {-1, -1};
int TCPServer::selected = -1;

} // ns pr

#endif /* SRC_TCPSERVER_H_ */
