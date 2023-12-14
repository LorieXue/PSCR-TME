#include "Banque.h"
#include <thread>
#include <iostream>
#include <unistd.h>

using namespace std;

const int NB_THREAD = 10;
const int SOLDEINITIAL = 100;
const int NB_COMPTES = 100; 

int main () {
	pr::Banque bank(NB_COMPTES,SOLDEINITIAL);
	vector<thread> threads;
	int m,i,j;
	// TODO : creer des threads qui font ce qui est demandé
	threads.reserve(NB_THREAD);
	for(int k=0;k<1000;k++){
		m=rand()%100+1;
		i=rand()%NB_COMPTES;
		do
		{
			j=rand()%NB_COMPTES;
		} while (i==j);
		while (threads.size() >= NB_THREAD) {
            for (auto& t : threads) {
                t.join();
            }
            threads.clear(); 
        }
		threads.emplace_back([&bank, i, j, m]() {
            bank.transfert(i, j, m);
            usleep(20000); // Pause 20 ms
        });
	}


	for (auto & t : threads) {
		t.join();
	}

	// TODO : tester solde = NB_THREAD * JP
	return 0;
}
