#ifndef GENETICALGORITHM_H
#define GENETICALGORITHM_H

#include <iostream>
#include <vector>
#include <new>
#include "graph.h"

class geneticAlgorithm
{
public:

	geneticAlgorithm(graph* A_, int populationNumber_, int numberOfPoorUnits_, std::vector< std::vector< bool > >* population_);
	~geneticAlgorithm();

	static int seeder; // potreban za rand

	graph* A;
	int populationNumber;							// broj populacije
	int numberOfPoorUnits;							// broj jedinki koje odbacujemo
	std::vector< std::vector< bool > > *population = new std::vector< std::vector< bool > >;	// reprezentacija populacije
	std::vector< double > maxDistanceInUnits;		// vektor maksimalnih udaljenosti iz svake jedinke
	std::vector< int 	> indexVector;				// vektor koji pamti indekse od najbolje do najgore jedinke	  

	void selection();
	void crossing();
	void mutation();

	double heuristic(int i);
};

#endif