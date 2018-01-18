#include <iostream>
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <cmath>
#include <time.h>

#include "geneticAlgorithm.h"

int geneticAlgorithm::seeder = 0;

geneticAlgorithm::geneticAlgorithm
(
	graph* A_,
	int populationNumber_,
	int numberOfPoorUnits_,
	std::vector< std::vector< bool > >* population_
) : 
	A(A_),
	populationNumber(populationNumber_),
	numberOfPoorUnits(numberOfPoorUnits_),
	population(population_)
{
	// pretpostavka je da bar dvije jedinke prezive selekciju zbog krizanja
	assert( populationNumber_ - numberOfPoorUnits_ > 1 );

	// maxDistanceInUnits initialization
	std::vector< double > v1( populationNumber_, -1 );
	maxDistanceInUnits = v1;

	// indexVector initialization
	std::vector< int > v3( populationNumber_ );
	indexVector = v3;
}

geneticAlgorithm::~geneticAlgorithm()
{
	// nothing to do
	delete population;
}


/*** HEURISTIKA 1 ***///
double geneticAlgorithm::heuristic(int i)
{
	double sum = 0;
	for( int j = 0; j < A->n; j++ )
	{
		if( (*population)[i][j] == true )
			sum += A->pointAsWarehouseQuality[j];
	}

	return sum;
}


///*** SELEKCIJA ***///
/// OPIS: izbacujemo numberOfPoorUnits < populationNumber jedinki koje imaju  
/// najvecu maks. udaljenost nekog "ne skladista" do nekog "skladista". Sve
/// se zapravo svodi na sortiranje idexVectora koji je sortiran po kakvoci jedinki   
void geneticAlgorithm::selection()
{
	// odredivanje vectora sa boolean vrijednostima koji odreduje koje jedinke izbacujemo
	std::iota( indexVector.begin(), indexVector.end(), 0 ); // funkcija koja popunjava vector sa 0,1,2,3,....
	
	// QUICK SORT IPAK IMPLEMENTIRATI
	// bubble sort sortiranje silazno da nam na pocetku budu najbolje jedinke
	bool succ;
	for(int i=0; i<populationNumber - 1; i++)
	{
		succ = true;
		for(int j=0; j<populationNumber - i - 1; j++)
		{
			if( heuristic( indexVector[j] ) < heuristic( indexVector[j+1] ) )
			{
				int tempIndex 	= indexVector[j];
				indexVector[j] 	= indexVector[j+1];
				indexVector[j+1]= tempIndex;
				succ = false;
			} 
		}
		if( succ ) // ako nisi nista zamijenio, gotovo  je
			break;
	}
}


///*** KRIZANJE ***///
/// OPIS: krizamo redom dva po dva (1.-2. pa 2.-3. pa 3.-4. ...) krenuvsi od najbolje
/// jedinke koja je prezivjela selekciju preko indexVector-a, ukoliko ima manje
/// prezivjelih jedinki od onih koje trebamo stvoriti, nastavljamo krizanje opet
/// sa prvom najboljom jedinkom. Dijete naslijeduje sve iste tocke svoga roditelj
/// i ostale na random dobiva
void geneticAlgorithm::crossing() 
{
	int numberOfWarehouses; // potreban nam je ovaj broj kako se ne bi zeznuli pa stavili
							// vise ili manje jedinica tj. skladista od stvarnog broja k
	int numberOfNotWarehouses;
	
	int z = 0; // za iteraciju po jedinkama koje su ostale nakon selekcije

	// pretpostavka je da barem dvije jedinke prezive selekciju
	// ovo je iteracija po jedinkama koje nisu prezivjele selekciju
	for(int i=populationNumber - numberOfPoorUnits; i<populationNumber; i++)
	{
		numberOfWarehouses 	  = 0;
		numberOfNotWarehouses = 0;

		for(int j=0; j<A->n; j++)
		{
			// kod krizanja ostavljamo tocke koje su iste kod oba roditelja
			if((*population)[ indexVector[z] ][j] == (*population)[ indexVector[z + 1] ][j] )
			{
				(*population)[ indexVector[i] ][j] = (*population)[ indexVector[z] ][j];
				
				if( (*population)[ indexVector[i] ][j] == true )
					numberOfWarehouses++;
				else
					numberOfNotWarehouses++; 

			}
		}

		for(int j=0; j<A->n; j++)
		{
			// inace prvo izabiremo tocku koja je "skladiste" kod nekog roditelja
			// nakon sto dodemo do k broja "skladista", uzimamo "ne skladista"
			if((*population)[ indexVector[z] ][j] != (*population)[ indexVector[z + 1] ][j] )
			{
				if( numberOfWarehouses == A->k )
				{
					(*population)[ indexVector[i] ][j] = false;
					numberOfNotWarehouses++;
					continue;
				}
				if( numberOfNotWarehouses == A->n - A->k )
				{
					(*population)[ indexVector[i] ][j] = true;
					numberOfWarehouses++;
					continue;
				}

				seeder++;
				if( rand() % 2 == 1 )
				{
					(*population)[ indexVector[i] ][j] = true;
				 	numberOfWarehouses++;
				}
				else
				{
					(*population)[ indexVector[i] ][j] = false;
					numberOfNotWarehouses++;
				}
			}
		}


		// ovaj if-else blok mi je potreban ukoliko ima vise jedinki koje
		// treba stvoriti od onih koje su prezivjele selekciju
		if(z == populationNumber - numberOfPoorUnits - 2) 
			z = 0;
		else
			z++;
	}
}

///*** MUTACIJA ***///
/// OPIS: svaka jedinka ima svoju vjerojatnost za mogucnost mutacije. Sto je jedinka
/// bolja, to ima manju vjerojatnost. Mutacija je skroz novi vector booleana
void geneticAlgorithm::mutation()
{
	int numberOfEliteUnits = 2; // mozda da to stavimo u klasu

	for(int i = numberOfEliteUnits; i < populationNumber; i++)
	{
		double percentOfOneUnitMutation = (double)(i - numberOfEliteUnits + 1)/(double)(populationNumber - numberOfEliteUnits + 1) * 50.0;
		int randomIndex;
		std::vector< int > indexesOfMutatedPoints;

		seeder++; // seeder trebamo povecati zbog stvarnog random-a ovog dolje u if-u
		if( (double)(rand() % 10000)/100.0 < percentOfOneUnitMutation )
		{
			std::vector< bool > temp(A->n, false);
			(*population)[ indexVector[i] ] = temp;

			int numberOfWarehouses = A->k;
			int randomIndex;

			while( numberOfWarehouses )
			{
				// seeder trebamo povecati zbog stvarnog random-a
				randomIndex = rand() % A->n;
				seeder++;

				if( (*population)[ indexVector[i] ][ randomIndex ] == false )
					numberOfWarehouses--;

				(*population)[ indexVector[i] ][ randomIndex ] = true;
			}
		}
	}
}