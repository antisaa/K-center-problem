#ifndef simulated_annealing_h_included
#define simulated_annealing_h_included

#include <vector>
#include <utility>
#include "graph.h"

class annealing
{
public:	

	graph * input;					//pointer na strukturu graph
	double evaulation;				//trenutna najveca udaljenost od nekog konzuma do najblizeg skladista

	int remove;						//redni broj vrha koji je skladiste za evaulation, mijenja se kad se pozove quality()

	annealing();
	annealing(graph *);				//popunjava pointer na strukturu input
	~annealing();

	std::pair<int, int> next2();	//mice skladiste 
	double quality();				//popunjava evaulation te vraca tu vrijednost
	void swap(int, int);			//redni broj prvog prestaje biti skladiste, redni broj drugog parametra postaje skaldiste
	double repeat2(int, double);

};


#endif