#ifndef SRC_SERVERSOCKET_H_
#define SRC_SERVERSOCKET_H_

#include "Socket.h"
#include <sys/socket.h>
#include <iostream>
#include <unistd.h>

namespace pr {

class ServerSocket {
	int socketfd;

public :
	// Demarre l'ecoute sur le port donne
	ServerSocket(int port);

	int getFD() { return socketfd;}
	bool isOpen() const {return socketfd != -1;}

	Socket accept();

	void close();
};

ServerSocket::ServerSocket(int port){
	int fd=socket(AF_INET, SOCK_STREAM, 0);
	if(fd==-1){
		perror("socket error");
		exit(-1);
	}

	struct sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = INADDR_ANY;

	if(bind(fd, (struct sockaddr*) &sin, sizeof(sin))==-1){
		perror("bind error");
		::close(fd);
		exit(-1);
	}
	listen(fd,50);
	socketfd = fd;

	std::cout << "Server listening on port " << port << std::endl;
}

Socket ServerSocket::accept(){
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    int clientfd = ::accept(socketfd, (struct sockaddr*)&addr, &len);
    if (clientfd == -1) {
        perror("accept socket error");
        exit(-1);
    }
	std::cout << "Connexion de " << inet_ntoa(addr.sin_addr) << std::endl;
	return clientfd;
}

void ServerSocket::close(){
	shutdown(socketfd,SHUT_RDWR);
	::close(socketfd);
}
} // ns pr
#endif /* SRC_SERVERSOCKET_H_ */

