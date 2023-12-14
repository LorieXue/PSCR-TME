#ifndef __COMBAT_H__
#define __COMBAT_H__

#include <stdlib.h>
#include <sched.h>
#include <signal.h>
#include "rsleep.h"
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

int healthPoints = 3;
int waitingPid;

namespace pr{
	void attaque (pid_t adversaire);
	void defense();
	void combat(pid_t adversaire);
	int wait_till_pid(pid_t pid);
	int wait_till_pid(pid_t pid, int sec);
}

void handlerAttack(int sig){
	healthPoints--;
}

void handlerDefense(int sig){
	std::cout << getpid() << " : Coup pare" << std::endl;
}

void pr::attaque(pid_t adversaire){
	//handle attack if i got signal
	signal(SIGINT, handlerAttack);
	std::cout << getpid() << " : "
			"Points de vie : " << healthPoints << std::endl;
	if(healthPoints==0){
		std::cout << getpid() << " : I'm dead " << std::endl;
		exit(1);
	}

	//attack !nullptr
	if(kill(adversaire,SIGINT)==-1 || wait_till_pid(adversaire,0)==adversaire){
		std::cout << getpid() << " : I'm alive " << std::endl;
		exit(0);
	}

	randsleep();

}

void pr::defense(){
	sigset_t mask;
	sigfillset(&mask);
	sigdelset(&mask,SIGINT);
	sigdelset(&mask,SIGCHLD);
	sigdelset(&mask,SIGALRM);
	sigprocmask(SIG_SETMASK,&mask,nullptr);

	struct sigaction sig;
	sig.sa_handler = handlerDefense;
	sig.sa_mask = mask;

//	signal(SIGINT,handlerDefense);
	sigaction(SIGINT,&sig,nullptr);
	randsleep();
//	sigsuspend(&mask);

}

void pr::combat(pid_t adversaire){
	while(true){
		pr::defense();
		pr::attaque(adversaire);
	}
}

int pr::wait_till_pid(pid_t pid) {
	pid_t fils;
	fils = wait(0);
	while (true) {
		if (fils == -1) {
			alarm(0);
			return -1;
		}
		if (fils == pid) {
			alarm(0);
			return pid;
		}
	}
}


int pr::wait_till_pid(pid_t pid, int sec){
	waitingPid = -1;
	alarm(sec);
	signal(SIGCHLD, [](int sig) {
		alarm(0);
		waitingPid = wait(0);
	});
	signal(SIGALRM, [](int sig) {
		alarm(0);
		waitingPid = 0;
	});
	pause();
	return waitingPid;
}

#endif
