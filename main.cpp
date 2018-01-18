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


double rjesenjeGen( geneticAlgorithm *alg, graph *A )
{
	std::vector< double > distanceFromClosestWarehouse(A->n, -1);

	std::vector< std::vector< bool > > *pop = new std::vector< std::vector< bool > >;
	pop = alg->population;

	for(int j=0; j<A->n; j++) // iteriranje po svim "ne skladistima" te jedinke
	{
		if( (*pop)[ alg->indexVector[0] ][j] ) // preskacemo tocku ukoliko je ona "skladiste" 
			continue; 

		for(int z=0; z<A->n; z++ ) // iteriranje po svim "skladistima" te jedinke i odredivanje min.
								   // udaljenosti u jedinki izmedu "ne skladista" i "skladista"
		{
			if( !(*pop)[ alg->indexVector[0] ][z] ) 
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

	for( int i = 0; i < A->n; i++ )
	{
		if( maxDistance < distanceFromClosestWarehouse[i] && (*pop)[ alg->indexVector[0] ][i] == false )
			maxDistance = distanceFromClosestWarehouse[i];
	}

	std::cout << "Rjesenje u GENETSKOM ALGORITMU: " << maxDistance << std::endl;
	return maxDistance;
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

	std::ofstream myfile("resultFile.txt");
    
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
		graph *tempA = new graph( fileName, 500, 100 );
		
		A = tempA;
	}
	else
	{
		std::cout << "Trebas unijeti ili 3( za ucitavanje iz datoteke ) ili 4 argumenta( za random )!" << std::endl;
		exit(0);
	}
	/////////////////////////////////////////////////////////

	annealing algorithm(A); 
	double temp = 1000;

	// ispod su varijable potrebne za konstrukciju geneticAlgoritm instance
	int populationNumber_ = 100; //400
	int numberOfPoorUnits_ = 90;//300
	int numberOfGenerations = 100;//500
	std::vector< std::vector< bool > > *population_ = new std::vector< std::vector< bool > >;
	srand( time(NULL) * geneticAlgorithm::seeder ); 
	//

	A->countQuality();

	////////////////////////////////////////////////////////////////
	///////************ SAMO SIMULIRANO KALJENJE ************///////
	if( flag == '0' )
	{
		double ev;
		int tempi;
	
		for (int i = 0; i < populationNumber_; ++i)
		{
			// std::cout << "Obavljam: " << i << ". iteraciju SIMULIRANOG KALJENJA" << std::endl;

			ev = algorithm.repeat2( temp, 0.6 );

			if( temp > ev ) 
			{
				temp = ev;
				tempi = i;
			}
		}

		std::cout << "Rjesenje u SIMLULIRANOM KALJENJU: " << temp << std::endl;	
		if (myfile.is_open())
    	{
        myfile << temp;
        myfile.close();
    	}
    	else std::cout << "Unable to open file";	
	}


	////////////////////////////////////////////////////////////////
	///////************ SAMO GENETSKI ALGORITAM ************////////
	if( flag == '1' )
	{
		for(int i = 0; i < populationNumber_; i++)
		{
			std::vector< bool > temp(A->n, false);

			int numberOfWarehouses = A->k;

			while( numberOfWarehouses )
			{
				int indexOfWarehouse = rand() % A->n;
				geneticAlgorithm::seeder++;

				if( temp[indexOfWarehouse] == false )
					numberOfWarehouses--;

				temp[indexOfWarehouse] = true;
			}

			(*population_).push_back( temp );
		}

		geneticAlgorithm genAlg( A, populationNumber_, numberOfPoorUnits_, population_ );

		for(int i = 0; i < numberOfGenerations; i++)
		{
			std::cout << "Obavljam: " << i + 1 << ". generaciju GENETSKOG ALGORITMA" << std::endl;
			genAlg.selection();
			genAlg.crossing();
			genAlg.mutation(); 
		}

		// std::cout << "Rjesenje u GENETSKOM ALGORITMU: " << genAlg.maxDistanceInUnits[0] << std::endl;
		if (myfile.is_open())
    	{	
        myfile << rjesenjeGen(&genAlg, A);
        myfile.close();
    	}
    	else std::cout << "Unable to open file";
		
	}

	////////////////////////////////////////////////////////////////////////////////
	///////************ SIMULIRANO I GENETSKI ALGORITAM ZAJEDNO ************////////
	if( flag == '2' )
	{
		double ev;
		int tempi;

		for (int i = 0; i < populationNumber_; ++i)
		{
			// std::cout << "Obavljam: " << i << ". iteraciju SIMULIRANOG KALJENJA" << std::endl;

			ev = algorithm.repeat2( temp, 0.6 );

			if( temp > ev ) 
			{
				temp = ev;
				tempi = i;
			}

			(*population_).push_back( A->chosen );
		}

		geneticAlgorithm genAlg( A, populationNumber_, numberOfPoorUnits_, population_ );

		for(int i = 0; i < numberOfGenerations; i++)
		{
			std::cout << "Obavljam: " << i + 1 << ". generaciju GENETSKOG ALGORITMA" << std::endl;
			genAlg.selection();
			genAlg.crossing();
			genAlg.mutation(); 
		}

		std::cout << std::endl;
		std::cout << "Algoritmi su se izvodili skupa!" << std::endl;

		std::cout << "Rjesenje u SIMLULIRANOM KALJENJU: " << temp << std::endl;
		if (myfile.is_open())
    	{
        myfile << temp;
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
		double ev;
		int tempi;

		for (int i = 0; i < populationNumber_; ++i)
		{
			// std::cout << "Obavljam: " << i << ". iteraciju SIMULIRANOG KALJENJA" << std::endl;

			ev = algorithm.repeat2( temp, 0.6 );

			if( temp > ev ) 
			{
				temp = ev;
				tempi = i;
			}
		}

		for(int i = 0; i < populationNumber_; i++)
		{
			std::vector< bool > temp(A->n, false);

			int numberOfWarehouses = A->k;

			while( numberOfWarehouses )
			{
				int indexOfWarehouse = rand() % A->n;
				geneticAlgorithm::seeder++;

				if( temp[indexOfWarehouse] == false )
					numberOfWarehouses--;

				temp[indexOfWarehouse] = true;
			}

			(*population_).push_back( temp );
		}

		geneticAlgorithm genAlg( A, populationNumber_, numberOfPoorUnits_, population_ );

		for(int i = 0; i < numberOfGenerations; i++)
		{
			std::cout << "Obavljam: " << i + 1 << ". generaciju GENETSKOG ALGORITMA" << std::endl;
			genAlg.selection();
			genAlg.crossing();
			genAlg.mutation(); 
		}

		std::cout << std::endl;
		std::cout << "Algoritmi su se izvodili odvojeno!" << std::endl;

		std::cout << "Rjesenje u SIMLULIRANOM KALJENJU: " << temp << std::endl;
		if (myfile.is_open())
    	{
        myfile << temp;
        myfile << "\n";
        myfile << rjesenjeGen(&genAlg, A);
        myfile.close();
    	}
    	else std::cout << "Unable to open file";

	}

	return 0;

}