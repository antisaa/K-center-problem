#include <iostream>
#include <random>
#include <chrono>
#include <utility>
#include <algorithm>
#include "simulated_annealing.h"
#include <time.h>
#include <cstdlib>

unsigned annealing::seed = std::chrono::system_clock::now().time_since_epoch().count();

void annealing::reset()
{
	std::default_random_engine generator(seed);
	seed++;

	std::uniform_int_distribution<int> distribution(0, input->n - 1);

	int numberOfWarehouses = input->k;
	chosen.assign(input->n, false);
		

	while(numberOfWarehouses)
	{
		int tempindex = distribution(generator);
		if(chosen[tempindex] == false)
		{
			chosen[tempindex] = true;
			numberOfWarehouses--;				
		}
	}
}

double annealing::heuristic() 
{
	double sum = 0;
	for( int j = 0; j < input->n; j++ )
	{
		if( chosen[j] == true )
			sum += input->pointAsWarehouseQuality[j];
	}

	return sum;
}

annealing::annealing(graph * input_)
{
	input = input_;

	std::default_random_engine generator(seed);
	seed++;

	std::uniform_int_distribution<int> distribution(0, input->n - 1);

	int numberOfWarehouses = input->k;
	chosen.assign(input->n, false);
		

	while(numberOfWarehouses)
	{
		int tempindex = distribution(generator);
		if(chosen[tempindex] == false)
		{
			chosen[tempindex] = true;
			numberOfWarehouses--;				
		}
	}
}


std::pair<int,int> annealing::next()
{
	std::default_random_engine generator(seed);
	seed++;
	std::uniform_int_distribution<int> distribution(0, input->n - 1);

	int j;	

	do
	{
		j = distribution(generator);

	} while (  chosen[j] != false );

	int i;
	do
	{
		i = distribution(generator);
	} while (chosen[i] == false);
	

	return std::make_pair (i,j); // makni i-ti cvor, dodaj j-ti cvor
}



double annealing::quality()
{
	std::vector<int> temp;

	// pronalazak najblizeg skladista za svako ne skladiste
	for (int i = 0; i < input->n; ++i)
	{
		temp.push_back(-1);
		if (chosen[i] == false)					//samo gledaj redove od ne-skaldišta
		{
			for (int j = 0; j < input->n; ++j)
			{
				if(chosen[j] == true)				//nemoj gledat udaljenost izmedu dva ne-skladista
					if (temp[i] == -1 || input->matrix[i][j] < input->matrix[i][temp[i]])
						temp[i] = j;
			}
		}
	}


	// pronalazak naudaljenijeg najblizeg skladista
	int temporary = -1;

	for (int i = 0; i < input->n; ++i)
	{
		if (temp[i] != -1)
			if( temporary == -1 || input->matrix[i][temp[i]] > input->matrix[temporary][temp[temporary]])
				temporary = i;
	}

	worst.first = temp[temporary];
	worst.second = temporary;
	
	return input->matrix[worst.first][worst.second];

}



void annealing::swap (int i, int j) // prvi makne, drugi doda
{
	if(chosen[i] == true)
	{	
		chosen[i] = false;
		chosen[j] = true;

	}
	else
	{
		chosen[i] = true;
		chosen[j] = false;
	}
}

void annealing::SA(int temperature, double alpha)
{	
	std::default_random_engine generator(seed);
	std::uniform_real_distribution<double> distribution(0, 1);
	double q1,q2;

	while (temperature > 1)
	{	
		
		q1 = heuristic();
		std::pair<int, int> temp = next();
		swap(temp.first, temp.second);
		q2 = heuristic();


		if (q1 < q2)
			if( distribution(generator) >= exp( -1*(q2-q1)/temperature ) )
				swap(temp.second, temp.first);

		temperature *= alpha;
		
	}

	
	return; // ako return quality() onda vaki put napravi taj alg koji je spor pa bolje da se on poziva ako je fakat nuzno
	

}

annealing::annealing()
{

}

annealing::~annealing()
{


}