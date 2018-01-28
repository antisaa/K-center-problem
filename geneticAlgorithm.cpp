#include <iostream>
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <cmath>
#include <time.h>
#include <random>
#include <chrono>

#include "geneticAlgorithm.h"

unsigned geneticAlgorithm::seed = std::chrono::system_clock::now().time_since_epoch().count();

geneticAlgorithm::geneticAlgorithm(graph* A_, int populationNumber_, int numberOfPoorUnits_) : A(A_), populationNumber(populationNumber_),numberOfPoorUnits(numberOfPoorUnits_)

{
	// pretpostavka je da bar dvije jedinke prezive selekciju zbog krizanja
	assert( populationNumber_ - numberOfPoorUnits_ > 1 );

	std::default_random_engine generator(seed);
	seed++;
	std::uniform_int_distribution<int> distribution(0, A->n -1);

	std::vector<bool> temp;

	for (int i = 0; i < populationNumber; ++i)
	{
		temp.assign(A->n,false);
		int numberOfWarehouses = A->k;
		int index;

		while(numberOfWarehouses)
		{
			index = distribution(generator);
			if(temp[index] == false)
			{
				temp[index] = true;
				numberOfWarehouses--;
			}

		}
		population.push_back(temp);

	}

	// indexVector initialization
	indexVector.resize(populationNumber);		
}

geneticAlgorithm::~geneticAlgorithm()
{
	// nothing to do
}


/*** HEURISTIKA ***///
double geneticAlgorithm::heuristic(int i)
{
	double sum = 0;
	for( int j = 0; j < A->n; j++ )
	{
		if( population[i][j] == true )
			sum += A->pointAsWarehouseQuality[j];
	}

	return sum;
}


///*** SELEKCIJA ***///
/// OPIS: sortiraj jedinke (sto se svodi na sortiranje indexVectora) po heuristici kako bi znali
/// koja je jedinka bolja od neke dvije i na kraju zapravo mogli provesti krizanje i mutaciju
void geneticAlgorithm::selection()
{
	// odredivanje vectora sa boolean vrijednostima koji odreduje koje jedinke izbacujemo
	std::iota( indexVector.begin(), indexVector.end(), 0 ); // funkcija koja popunjava vector sa 0,1,2,3,....


	// zapisivanje heuristika kako svaki put ne bi morali zvati funkciju
	std::vector< int > vectorOfHeuristicValues( populationNumber, 0 );
	for(int i = 0; i < populationNumber; i++) 
	{
		vectorOfHeuristicValues[i] = heuristic(i);
	}

	// sortiranje vektora indexVector u odnosu na heuristike jedinki
	int minHeuristic;
	int indexOfMinHeuristicInIndexVector;

	for(int i = 0; i < populationNumber / 2; i++)
	{
		minHeuristic = vectorOfHeuristicValues[ indexVector[i] ];
		indexOfMinHeuristicInIndexVector = i;

		for( int j = i + 1; j < populationNumber; j++ )
		{
			if( vectorOfHeuristicValues[ indexVector[j] ] < minHeuristic ) 
			{
				minHeuristic = vectorOfHeuristicValues[ indexVector[j] ];
				indexOfMinHeuristicInIndexVector = j;
			}
		}

		int temp = indexVector[i];
		indexVector[i] = indexVector[indexOfMinHeuristicInIndexVector];
		indexVector[indexOfMinHeuristicInIndexVector] = temp;
	}

	// ovo sluzi samo za pomoc pri kodiranju - moze se zakomentirati
	//std::cout << std::endl;
	//for (int i = 0; i < populationNumber / 4; ++i)
	//{
	//	std::cout << vectorOfHeuristicValues[ indexVector[ i ] ] << " ";
	//}
	//std::cout << std::endl;

}


///*** KRIZANJE ***///
/// OPIS: 3 puta uzmi 3 random jedinke iz bolje populacije, i najlosiju pretvori u
/// dijete najbolje i druge najbolje na nacin da sa random izabranim indexom
/// odlucis koji dio ide od tate, koji od mame
void geneticAlgorithm::crossing() 
{
	// METODA 2trioFantastico ---> isto kao METODA 2 samo sa triomFantasicom

	for(int i = 0; i < 3 ; i++)
	{
		// izabiranje 3 random jedinke iz prve polovice populacije
		std::default_random_engine generator1(seed);
		seed++;
		std::uniform_int_distribution<int> distribution1(0, populationNumber / 2);

		int randIndex0 = distribution1(generator1);
		int randIndex1 = distribution1(generator1);
		int randIndex2 = distribution1(generator1);

		std::vector< std::pair<double, int> > trioFantastico(3);

		trioFantastico[0] = std::make_pair( heuristic(indexVector[randIndex0]), indexVector[randIndex0] );
		trioFantastico[1] = std::make_pair( heuristic(indexVector[randIndex1]), indexVector[randIndex1] );
		trioFantastico[2] = std::make_pair( heuristic(indexVector[randIndex2]), indexVector[randIndex2] );

		// sortiranje tih jedinki od najbolje do najgore
		std::sort( trioFantastico.begin(), trioFantastico.end() );

		int numberOfWarehouses = 0;
		std::vector< int > indexesOfWarehouses;
		std::vector< int > indexesOfNotWarehouses;

		// krizanje na nacin da se izabere random indexom koji oznacava sto naslijeduje od oca a sto od majke
		std::default_random_engine generator(seed);
		seed++;
		std::uniform_int_distribution<int> distribution(0, A->n - 2);

		int randIndex = distribution(generator);
		
		for(int j = 0; j < A->n; j++)
		{
			if( j <= randIndex )
			{
				population[ trioFantastico[2].second ][j] = population[ trioFantastico[0].second ][j];
			}
			else
			{
				population[ trioFantastico[2].second ][j] = population[ trioFantastico[1].second ][j];
			}

			if( population[ trioFantastico[2].second ][j] == true )
			{
				numberOfWarehouses++;
				indexesOfWarehouses.push_back( j );
			}
			else
			{
				indexesOfNotWarehouses.push_back( j );
			}
		}

		// ova dva if-a su tu ako bude vise ili manje skladista od dopustenog
		// tada se tu to korigira na nacin da se random odaberu neka skladista i pretvore
		// se u ne skladista ili obrnuto
		if( numberOfWarehouses < A->k )
		{
			std::random_shuffle ( indexesOfNotWarehouses.begin(), indexesOfNotWarehouses.end() );

			int l = 0;
			while( numberOfWarehouses != A->k )
			{
				population[ trioFantastico[2].second ][ indexesOfNotWarehouses[l] ] = true;
				numberOfWarehouses++;
				l++;
			}
		}

		if( numberOfWarehouses > A->k )
		{
			std::random_shuffle ( indexesOfWarehouses.begin(), indexesOfWarehouses.end() );

			int l = 0;
			while( numberOfWarehouses != A->k )
			{
				population[ trioFantastico[2].second ][ indexesOfWarehouses[l] ] = false;
				numberOfWarehouses--;
				l++;
			}
		}
	}
}

///*** MUTACIJA ***///
/// OPIS: losiju polovicu populacije mutiraj tako da potpuno druge jedinke stvoris
void geneticAlgorithm::mutation()
{
	for(int i = populationNumber / 2; i < populationNumber; i++)
	{
		std::default_random_engine generator(seed);
		seed++;
		std::uniform_int_distribution<int> distribution2(0, A->n - 1);

		std::vector< bool > temp(A->n, false);
		population[ indexVector[i] ] = temp;

		int numberOfWarehouses = A->k;
		int randomIndex;

		while( numberOfWarehouses )
		{
			// seeder trebamo povecati zbog stvarnog random-a
			randomIndex = distribution2(generator);
			if( population[ indexVector[i] ][ randomIndex ] == false ){
				population[ indexVector[i] ][ randomIndex ] = true;
				numberOfWarehouses--;
			}
		}
	}
}