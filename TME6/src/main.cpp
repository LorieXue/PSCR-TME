#include "combat.h"
#include <wait.h>

using namespace pr;

//I am Vador
int main0(){
	pid_t luke = fork();

	if(luke==0){
		combat((int) getppid());
	} else {
		combat((int) luke);
	}
	wait(0);
	return 0;
}
