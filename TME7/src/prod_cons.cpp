#include "Stack.h"
#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <vector>
#include <semaphore.h>


using namespace std;
using namespace pr;

#define N 1
#define M 2
bool b = true;
Stack<char> * s;
sem_t* sem;

void producteur (Stack<char> * stack) {
	char c ;
	while (cin.get(c) && b) {
		cout << b << endl;
		sem_wait(sem);
		stack->push(c);
		sem_post(sem);
	}
}

void consomateur (Stack<char> * stack) {
	while (b) {
		cout << b << endl;
		sem_wait(sem);
		char c = stack->pop();
		cout << c << flush ;
		sem_post(sem);
	}
}

void signalHandler(int sig) {
	b= false;
	sem_post(sem);
}

int main () {
	signal(SIGINT, signalHandler);

	// Crée mem partagé
	int shm_fd = shm_open("/shm", O_CREAT | O_RDWR, 0666);
	ftruncate(shm_fd, sizeof(Stack<char>)); // alloue mémoire

	// crée sem
	sem = sem_open("/sem", O_CREAT | O_EXCL, 0666, 1);

	s = (Stack<char>*)mmap(NULL, sizeof(Stack<char>), PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0);
	if (s == MAP_FAILED) {
	    // Handle the error
	    perror("mmap");
	    exit(EXIT_FAILURE);
	}
	vector<pid_t> sons;
	sons.reserve(N+M);
	for(int i=0;i<N;i++){
		pid_t pp = fork();
		if (pp==0) {
			producteur(s);
			return 0;
		} else {
			sons.push_back(pp);
		}
	}

	for(int j=0;j<M;j++){
		pid_t pc = fork();
		if (pc==0) {
			consomateur(s);
			return 0;
		}else {
			sons.push_back(pc);
		}
	}

	while(b){}

	for(int i=0;i<N+M;i++)
		kill(sons[i],SIGINT);

	for(int i=0;i<N+M;i++)
		wait(0);

	// libère la case de la mémoire partag
	munmap(s, sizeof(Stack<char>));
	sem_close(sem);
	shm_unlink("/shm");
	sem_unlink("/sem");
	return 0;
}

