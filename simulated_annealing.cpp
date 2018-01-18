#include <iostream>
#include <random>
#include <chrono>
#include <utility>
#include <algorithm>
#include "simulated_annealing.h"
#include <time.h>
#include <cstdlib>

static int seeder = 0;

annealing::annealing(graph * input_)
{
	input = input_;
}


std::pair<int,int> annealing::next2()
{
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);
	std::uniform_int_distribution<int> distribution(0, (*input).n - 1);

	int j;	

	do
	{
		j = distribution(generator);

	} while ( (*input).chosen[j] != false );
	
	//std::cout << "Vrhovi: " << input->n << "Remove: " << remove <<  "j: " << j << std::endl;

	return std::make_pair (remove,j); // makni i-ti cvor, dodaj j-ti cvor	
}



double annealing::quality()
{
	// double temp = 0;
	// for (int i = 0; i < (*input).n; ++i)
	// {
	// 	if ((*input).chosen[i] == true)					//samo gledaj redove od odabranih skaldišta
	// 		for (int j = 0; j < (*input).n; ++j)
	// 		{
	// 			if((*input).chosen[j] == false)				//nemoj gledat udaljenost izmedu dva skladista
	// 				if ((*input).matrix[i][j] > temp)
	// 					{
	// 						remove = i;
	// 						temp = (*input).matrix[i][j];
	// 					}
	// 		}
	// }

	//return temp;

	std::vector< double > distanceFromClosestWarehouse(input->n, -1);
	std::vector< int > indexesForRemoveVariable(input->n, -1);

	for(int j=0; j<input->n; j++) // iteriranje po svim "ne skladistima" te jedinke
	{
		if( (*input).chosen[j] ) // preskacemo tocku ukoliko je ona "skladiste" 
			continue; 

		for(int z=0; z<input->n; z++ ) // iteriranje po svim "skladistima" te jedinke i odredivanje maks.
								   // udaljenosti u jedinki izmedu "ne skladista" i "skladista"
		{
			if( !(*input).chosen[z] ) 
				continue; // preskacemo ako ta tocka nije "skladiste" jer samo 
						  // nas zanima udaljenost "ne skladista" i "skladista"
			
			if( distanceFromClosestWarehouse[j] == -1 )
			{
				distanceFromClosestWarehouse[j] = (*input).matrix[j][z];
				indexesForRemoveVariable[j] = z;
			}

			if( distanceFromClosestWarehouse[j] > (*input).matrix[j][z] )
			{
				distanceFromClosestWarehouse[j] = (*input).matrix[j][z];
				indexesForRemoveVariable[j] = z;		
			}
		}
	}

	double maxDistance = 0;
	for( int i = 0; i < input->n; i++ )
	{
		if( maxDistance < distanceFromClosestWarehouse[i] && input->chosen[i] == false )
		{
			maxDistance = distanceFromClosestWarehouse[i];
			remove = indexesForRemoveVariable[i];
		}
	}

	return maxDistance;

}



void annealing::swap (int i, int j) // prvi makne, drugi doda
{
	(*input).chosen[i] = false;
	(*input).chosen[j] = true;
	evaulation = quality();
}

double annealing::repeat2( int temperature, double alpha)
{	
	srand (time(NULL) * seeder);


	//unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	//std::default_random_engine generator(seed+seeder);
	//std::uniform_real_distribution<double> distribution(0, 1);




	double q1,q2;

	while (temperature > 1)
	{	
		q1 = quality();
		std::pair<int, int> temp = next2();
		swap(temp.first, temp.second);
		q2 = quality();
		if (q1 < q2)
			//if( distribution(generator) >= exp( (q2-q1)/temperature ) )
			if( (double)((rand()%10000)/10000) >= exp( (q2-q1)/temperature ) )
				swap(temp.second, temp.first);

		temperature *= alpha;
	}

	quality();
	seeder++;
	/*std::cout << "annealing ended, max is:" << quality() << std::endl << "Chosen are :" << std::endl;
	for (int i = 0; i < (*input).n; ++i)
	{
		if((*input).chosen[i] == true)
			std::cout << i <<" ";
	}
	std::cout<<std::endl;
	*/

	return evaulation;

}

annealing::annealing()
{

}

annealing::~annealing()
{


}