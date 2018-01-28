#ifndef GENETICALGORITHM_H
#define GENETICALGORITHM_H

#include <iostream>
#include <vector>
#include <new>
#include "graph.h"

class geneticAlgorithm
{
public:

	geneticAlgorithm(graph* A_, int populationNumber_, int numberOfPoorUnits_);
	~geneticAlgorithm();

	static unsigned seed;

	graph* A;
	int populationNumber;							// broj populacije
	int numberOfPoorUnits;							// broj jedinki koje odbacujemo
	std::vector< std::vector< bool > > population;	// reprezentacija populacije
	std::vector< int > indexVector;				// vektor koji pamti indekse od najbolje do najgore jedinke	  

	void selection();
	void crossing();
	void mutation();

	double heuristic(int);
};

#endif