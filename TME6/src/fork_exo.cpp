#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

int main () {
	const int N = 3;
	std::cout << "main pid=" << getpid() << std::endl;
	int i,j;
	// i le nombre de fils que va créer le processus
	// j le nombre max de fils crée
	for (i=1, j=N, cpt = 1; i<=N && j==N && fork()==0 ; i++ ) {
		std::cout << " i:j " << i << ":" << j << std::endl;
		// k le numéro du fils (feuille) créé
		for (int k=1; k<=i && j==N ; k++) {
			if ( fork() == 0) {
				j=0;
				std::cout << " k:j " << k << ":" << j << std::endl;
				exit(0);
			}
		}
	}
	for(int l = 0; l < std::min(i,j); l++){
		if (wait(0) == -1){
			cout << "J'ai trop attendu =(" << i << j << endl;
		}
	}
	if (wait(0) != -1){
		cout << "J'ai pas assez attendu =(" << getpid() << endl;
	}
	std::cout << " ---------- "<< std::endl;
	std::cout << " i:j " << i << ":" << j  << " " << min(i,j) << " " << std::endl;
	return 0;
}
