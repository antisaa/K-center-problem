#include <iostream>

#include "graph.h"
#include "simulated_annealing.h"
#include "geneticAlgorithm.h"

#include <cassert>
#include <vector>
#include <utility>
#include <string>
#include <sstream>
#include <fstream>

std::ofstream myfile("resultFile.txt");

double rjesenjeGen( geneticAlgorithm *alg, graph *A )
{
	// annealing simulirano(A);

	// simulirano.chosen = alg->population[ alg->indexVector[0] ];

	// std::cout << "Rjesenje u GENETSKOM ALGORITMU sa qualitiem: " << simulirano.quality() << std::endl;


	std::vector< double > maxDistances;


	for( int i = 0; i < alg->populationNumber / 2; i++ )
	{
		std::vector< double > distanceFromClosestWarehouse(A->n, -1);

		for(int j=0; j<A->n; j++) // iteriranje po svim "ne skladistima" te jedinke
		{
			if( alg->population[ alg->indexVector[i] ][j] ) // preskacemo tocku ukoliko je ona "skladiste" 
				continue; 

			for(int z=0; z<A->n; z++ ) // iteriranje po svim "skladistima" te jedinke i odredivanje min.
									   // udaljenosti u jedinki izmedu "ne skladista" i "skladista"
			{
				if( !alg->population[ alg->indexVector[i] ][z] ) 
					continue; // preskacemo ako ta tocka nije "skladiste" jer samo 
							  // nas zanima udaljenost "ne skladista" i "skladista"
				
				if( distanceFromClosestWarehouse[j] == -1 )
				{
					distanceFromClosestWarehouse[j] = A->matrix[j][z];
				}

				if( distanceFromClosestWarehouse[j] > A->matrix[j][z] )
				{
					distanceFromClosestWarehouse[j] = A->matrix[j][z];		
				}
			}
		}

		double maxDistance = 0;

		for( int j = 0; j < A->n; j++ )
		{
			if( maxDistance < distanceFromClosestWarehouse[j] && alg->population[ alg->indexVector[i] ][j] == false )
				maxDistance = distanceFromClosestWarehouse[j];
		}

		maxDistances.push_back(maxDistance);
	}

	double minDistance = maxDistances[0];

	for( int i = 0; i < alg->populationNumber / 2; i++ )
	{
		if( minDistance > maxDistances[i] )
		{
			minDistance = maxDistances[i];
		}
	}

	std::cout << "Rjesenje u GENETSKOM ALGORITMU: " << minDistance << std::endl;
	return minDistance;
}

////////////////////////////////////////////
///////************ MAIN ************///////
////////////////////////////////////////////
int main(int argc, char **argv)
{	

	////// inicijalizacija podataka iz komandne linije //////
	//////   UNOSITI IH REDOM KAKO SU INICIJALIZIRANI  //////
	char flag; 				// inic. bez .txt	// inic. sa .txt
	int n;     				// inic. bez .txt
	int k;					// inic. bez .txt	// inic. sa .txt
	int range;				// inic. bez .txt	
	std::string fileName;						// inic. sa .txt

	graph *A = new graph();
	if( argc == 5 )
	{
		flag = *(argv[1]);
		std::istringstream streamN		( argv[2] );
		std::istringstream streamK		( argv[3] );
		std::istringstream streamRange	( argv[4] );

		if ( !(streamN >> n) )
        {
    		std::cout << "Los unos n varijable! Unosi se na 2. mjestu." << std::endl;
			exit(0);        
        }

        if ( !(streamK >> k) )
        {
    		std::cout << "Los unos k varijable! Unosi se na 3. mjestu." << std::endl;
			exit(0);        
        }

        if ( !(streamRange >> range) )
        {
    		std::cout << "Los unos range varijable! Unosi se na 4. mjestu." << std::endl;
			exit(0);        
        }

        if( n == k )
        {
        	std::cout << "Rjesenje je trivijalno..." << std::endl;
        	return 0;
        }

        if( n < k )
        {
        	std::cout << "Ne mozes unijeti n varijablu koja je manja od k varijable" << std::endl;
        	exit(0);	
        }

		graph *tempA = new graph( n, k ); 	// prvi argument je n drugi je k
		tempA->gamma( 1, range ); 			// argument je parametar distribucije
		
		A = tempA;
	}
	else if( argc == 4 )
	{
		flag = *(argv[1]);
		
		std::istringstream streamK( argv[2] );
		if ( !(streamK >> k) )
        {
    		std::cout << "Los unos k varijable! Unosi se na 2. mjestu." << std::endl;
			exit(0);        
        }

		fileName = argv[3];	
		graph *tempA = new graph( fileName, k );
		
		A = tempA;
	}
	else
	{
		std::cout << "Trebas unijeti ili 3( za ucitavanje iz datoteke ) ili 4 argumenta( za random )!" << std::endl;
		exit(0);
	}
	/////////////////////////////////////////////////////////

	// ispod su varijable potrebne za konstrukciju geneticAlgoritm instance
	int populationNumber_ = 3000;	
	int numberOfPoorUnits_ = 0;
	int numberOfGenerations = 500;
	std::vector< std::vector< bool > > population_;
	//PROMIJENITI OVO DA SE UCITAVA PREKO KONSOLE

	annealing algorithm(A); 
	double temp = 5000;
	A->countQuality();

	geneticAlgorithm genAlg( A, populationNumber_, numberOfPoorUnits_ );	

	double best = -1;
	double ev;

	////////////////////////////////////////////////////////////////
	///////************ SAMO SIMULIRANO KALJENJE ************///////
	if( flag == '0' )
	{
		for (int i = 0; i < populationNumber_; ++i)
		{
			std::cout << "Obavljam: " << i << ". iteraciju SIMULIRANOG KALJENJA" << std::endl;

			algorithm.SA( temp, 0.6 );
			genAlg.population[i] = algorithm.chosen;
			
			
			ev = algorithm.quality();
			
			if( best == -1 || best > ev )
			{
				best = ev;
			}

			algorithm.reset();


		}

		std::cout << "Rjesenje u SIMLULIRANOM KALJENJU: " << best << std::endl;

	if (myfile.is_open())
    {
        myfile << best;
        myfile << "\n";
        myfile.close();
    }
    else std::cout << "Unable to open file";	
	}


	////////////////////////////////////////////////////////////////
	///////************ SAMO GENETSKI ALGORITAM ************////////
	if( flag == '1' )
	{
		for(int i = 0; i < numberOfGenerations; i++)
		{
			std::cout << "Obavljam: " << i + 1 << ". generaciju GENETSKOG ALGORITMA" << std::endl;
			genAlg.selection();
			genAlg.crossing();
			genAlg.mutation(); 
		}
		genAlg.selection();

		if (myfile.is_open())
    	{
        	        	
        	myfile << rjesenjeGen(&genAlg, A);
        	myfile << "\n";
        	myfile.close();
    	}
    	else std::cout << "Unable to open file";
		
	}

	////////////////////////////////////////////////////////////////////////////////
	///////************ SIMULIRANO I GENETSKI ALGORITAM ZAJEDNO ************////////
	if( flag == '2' )
	{
		for (int i = 0; i < populationNumber_; ++i)
		{
			std::cout << "Obavljam: " << i << ". iteraciju SIMULIRANOG KALJENJA" << std::endl;

			algorithm.SA( temp, 0.6 );
			genAlg.population[i] = algorithm.chosen;
			
			
			ev = algorithm.quality();
			
			if( best == -1 || best > ev )
			{
				best = ev;
			}

			algorithm.reset();
		}

		for(int i = 0; i < numberOfGenerations; i++)
		{
			std::cout << "Obavljam: " << i + 1 << ". generaciju GENETSKOG ALGORITMA" << std::endl;
			genAlg.selection();
			genAlg.crossing();
			genAlg.mutation(); 
		}
		genAlg.selection();

		std::cout << std::endl;
		std::cout << "Algoritmi su se izvodili skupa!" << std::endl;

		std::cout << "Rjesenje u SIMLULIRANOM KALJENJU: " << best << std::endl;
		

    if (myfile.is_open())
    {
        myfile << best;
        myfile << "\n";
        myfile << rjesenjeGen(&genAlg, A);
        myfile.close();
    }
    else std::cout << "Unable to open file";
		
	}


	/////////////////////////////////////////////////////////////////////////////////
	///////************ SIMULIRANO I GENETSKI ALGORITAM ODVOJENO ************////////
	if( flag == '3' )
	{
		for (int i = 0; i < populationNumber_; ++i)
		{
			std::cout << "Obavljam: " << i << ". iteraciju SIMULIRANOG KALJENJA" << std::endl;

			algorithm.SA( temp, 0.6 );
			genAlg.population[i] = algorithm.chosen;
			
			
			ev = algorithm.quality();
			
			if( best == -1 || best > ev )
			{
				best = ev;
			}

			algorithm.reset();
		}

		for(int i = 0; i < numberOfGenerations; i++)
		{
			std::cout << "Obavljam: " << i + 1 << ". generaciju GENETSKOG ALGORITMA" << std::endl;
			genAlg.selection();
			genAlg.crossing();
			genAlg.mutation(); 
		}
		genAlg.selection();

		std::cout << std::endl;
		std::cout << "Algoritmi su se izvodili odvojeno!" << std::endl;

		std::cout << "Rjesenje u SIMLULIRANOM KALJENJU: " << best << std::endl;
		
	if (myfile.is_open())
    {
        myfile << best;
        myfile << "\n";
        myfile << rjesenjeGen(&genAlg, A);
        myfile.close();
    }
    else std::cout << "Unable to open file";
	}

	return 0;

}
