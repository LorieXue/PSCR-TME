/*
 * Barrier.h
 *
 *  Created on: 20 oct. 2023
 *      Author: 28705252
 */

#ifndef BARRIER_H_
#define BARRIER_H_

#include <condition_variable>
#include <mutex>
#include <cstdlib>
#include <iostream>

namespace pr{

class Barrier {
	mutable std::mutex m;
	std::condition_variable cond;
	int cpt;
	const int goal;
public:
	Barrier(int goal):cpt(0),goal(goal){}
	virtual ~Barrier(){}
	void done();
	void waitFor();
};



void Barrier::done(){
	std::unique_lock<std::mutex> lg(m);
	cpt++;
	if(cpt==goal){
		std::cout << "goal atteint" << std::endl;
		cond.notify_all();
	}
}

void Barrier::waitFor(){
	std::unique_lock<std::mutex> lg(m);
	while(cpt != goal){
		cond.wait(lg);
	}
}

}
#endif /* BARRIER_H_ */
