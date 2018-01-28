#ifndef simulated_annealing_h_included
#define simulated_annealing_h_included

#include <vector>
#include <utility>
#include "graph.h"

class annealing
{
public:	

	graph * input;					//pointer na strukturu graph
	std::vector<bool> chosen;
	std::pair<int,int> worst;		// preko njega se pristupa egzaktnoj kvaliteti = matrix[worst.first,worst.second]
	static unsigned seed;


	annealing(graph *);				//popunjava pointer na strukturu input
	annealing();
	~annealing();


	double heuristic();
	std::pair<int, int> next();		//vraca index nekog skladista te nekog ne skladista 
	double quality();				//popunjava worst te vraca tu vrijednost
	void swap(int, int);			//redni broj prvog prestaje biti skladiste, redni broj drugog parametra postaje skaldiste
	void SA(int, double);
	void reset();

};


#endif