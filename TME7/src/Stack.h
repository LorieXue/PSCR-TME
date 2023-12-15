#pragma once

#include <cstring> // size_t,memset
#include <semaphore.h>
#include <ostream>
#include <iostream>

namespace pr {
using namespace std;

#define STACKSIZE 100

template<typename T>
class Stack {
	T tab [STACKSIZE];
	size_t sz;
	sem_t mutex, s_cons, s_prod;
public :
	Stack () : sz(0) { memset(tab,0,sizeof tab) ;
		sem_init(&mutex, 1, 1);
		sem_init(&s_cons, 1, 0);
		sem_init(&s_prod, 1, STACKSIZE);
	}

	T pop () {
		cout << "pop" << endl;
		sem_wait(&s_cons);
		sem_wait(&mutex);
		T toret = tab[sz-1];
		sz--;
		sem_post(&mutex);
		sem_post(&s_prod);
		return toret;
	}

	void push(T elt) {
		cout << "push " << elt << endl;
		sem_wait(&s_prod);
		sem_wait(&mutex);
		tab[sz++] = elt;
		sem_post(&mutex);
		sem_post(&s_cons);
	}

	~Stack (){
		sem_destroy(&mutex);
		sem_destroy(&s_cons);
		sem_destroy(&s_prod);
	}
};

}
