#include <iostream>
#include <vector>
#include <utility>
#include <fstream>
#include <string>
#include <iomanip>
#include <random>
#include <chrono>
#include <cmath>
#include "graph.h"



double distance (std::pair<double,double> first, std::pair<double,double> second)
{
	return sqrt( pow(first.first - second.first,2) + pow(first.second - second.second, 2) );

}

unsigned graph::seed = std::chrono::system_clock::now().time_since_epoch().count();
graph::graph(int N, int K): n(N), k(K)
{	

	pointAsWarehouseQuality.assign(n, 0);

	std::vector<double> v(n,0);	
	for (int i = 0; i < n; ++i)
	{			
		matrix.push_back(v);
	}
	
}

graph::graph(std::string test_file, int K): k(K)
{
    std::fstream test;
	test.open(test_file);
	test >> n;

	pointAsWarehouseQuality.assign(n, 0);

	std::vector<double> v(n,0);
	
	for (int i = 0; i < n; ++i)
	{			
		matrix.push_back(v);
	}

	int tempi, tempj, tempk;

	test >> tempi >> tempj;

	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			if(i!=j)
				matrix[i][j]=9999999;
			else
				matrix[i][j]=0;
		}
	}

	while (test.good())
	{
		test >> tempi >> tempj >> tempk;
		matrix[tempi-1][tempj-1] = tempk;
		matrix[tempj-1][tempi-1] = tempk;
	}

	for (int k = 0; k < n; ++k)
	{
		for (int i = 0; i < n; ++i)
		{
			for (int j = 0; j < n; ++j)
			{
				if (matrix[i][k] + matrix[k][j] < matrix[i][j])
					matrix[i][j] = matrix[i][k] + matrix[k][j];
			}
		}
	}
}

graph::graph()
{}

graph::~graph()
{}

// ako se ukljuci ovaj countQuality, ne mora se nista nigdje mijenjati jer
// sada i ova heuristika daje manji rezultat ako je to bolje rjesenje
void graph::countQuality()
{
	std::vector< double > firstMins(n, -1);
	std::vector< double > secondMins(n, -1);

	// ova prva for petlja sa for petljom u sebi racuna prvu i drugu najblizu udaljenost
	// do ostalih tocaka u grafu
	for(int i = 0; i < n; i++)
	{
		for(int j = 0; j < n; j++)
		{
			if( j == i )
				continue; // nema smisla gledati udaljenost do samog sebe
			
			// pocetne inicijalizacije
			if( firstMins[i] == -1 )
			{
				firstMins[i] = matrix[i][j];
				continue;
			}
			
			if( secondMins[i] == -1 )
			{
				secondMins[i] = matrix[i][j];
				continue;
			}

			// trazenje prve i druge najblize tocke
			if( firstMins[i] > matrix[i][j] )
			{
				secondMins[i] = firstMins[i];
				firstMins[i] = matrix[i][j];
				continue;
			}
			if( secondMins[i] > matrix[i][j] )
			{
				secondMins[i] = matrix[i][j];
				continue;
			}
		}
	}

	for(int i = 0; i < n; i++)
	{
		for(int j = 0; j < n; j++)
		{
			if( j == i )
				continue; // nema smisla povecavati kvalitetu za udaljenost do samog sebe

			if( firstMins[i] == matrix[i][j] )
			{
				pointAsWarehouseQuality[j]-=5;
				continue;
			}
			if( secondMins[i] == matrix[i][j] )
			{
				pointAsWarehouseQuality[j]-=2;
				continue;	
			}
		}
	}
}


/*void graph::countQuality()
{
	
	int tempindex = 1;
	std::vector<bool> temp(n,false);
	bool flag;

	for (int i = 0; i < n; ++i) // po svim vrhovima
	{	
		temp.assign(n,false); // kao konstruktor za vektore s parametrima (n,false)
		temp[i] = true;
		
		for (int j = 0; j < n-1; ++j) // po svim rednim brojevima
		{
			
			for (int k = 0; k < n; ++k) // po ostalim vrhovima
			{		
				if(temp[k] == false)
				{
				
					if (matrix[i][k] < matrix[i][tempindex])
					{
						tempindex = k;
						
					}
				}

			} // nakon petlje u tempindex je spremljen index j.-tog po redu clana uzlazno
			
			
			temp[tempindex] = true;
			pointAsWarehouseQuality[tempindex] += j;

			for (int z = 0; z < n; ++z)
			{
				if(temp[z]==false)
					{
						tempindex=z;
						break;
					}
			}
		}
	}
}*/



void graph::uniform(double parametar1, double parametar2)
{

	std::default_random_engine generator(seed);
	seed++;

	std::uniform_real_distribution<double> distribution(parametar1, parametar2);

	
	std::pair<double, double> temp;
	for (int i = 0; i < n; ++i)
	{
		temp.first = distribution(generator);
		temp.second = distribution(generator);
		coordinates.push_back(temp);
	}	

	for (int i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j)
			matrix[i][j] = distance( coordinates[i], coordinates[j] );

}


void graph::poisson(double parametar)
{
	//unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);
	seed++;

	std::poisson_distribution<int> distribution(parametar);

	std::pair<double, double> temp;

	for (int i = 0; i < n; ++i)
	{
		temp.first = distribution(generator);
		temp.second = distribution(generator);
		coordinates.push_back(temp);
	}	

	for (int i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j)
			matrix[i][j] = distance( coordinates[i], coordinates[j] );


}


void graph::exponential(double parametar)
{
	//unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);
	seed++;
	std::exponential_distribution<double> distribution(parametar);

	std::pair<double, double> temp;

	for (int i = 0; i < n; ++i)
	{
		temp.first = distribution(generator);
		temp.second = distribution(generator);
		coordinates.push_back(temp);
	}	

	for (int i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j)
			matrix[i][j] = distance( coordinates[i], coordinates[j] );

}

void graph::gamma(double parametar1, double parametar2)
{
	//unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);
	seed++;

	std::gamma_distribution<double> distribution(parametar1, parametar2);

	std::pair<double, double> temp;

	for (int i = 0; i < n; ++i)
	{
		temp.first = distribution(generator);
		temp.second = distribution(generator);
		coordinates.push_back(temp);
	}	

	for (int i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j)
			matrix[i][j] = distance( coordinates[i], coordinates[j] );


}

void graph::weibull(double parametar1, double parametar2)
{
	//unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);
	seed++;
	std::weibull_distribution<double> distribution(parametar1, parametar2);

	std::pair<double, double> temp;

	for (int i = 0; i < n; ++i)
	{
		temp.first = distribution(generator);
		temp.second = distribution(generator);
		coordinates.push_back(temp);
	}	

	for (int i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j)
			matrix[i][j] = distance( coordinates[i], coordinates[j] );


}


void graph::normal(double parametar1, double parametar2)
{
	//unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);
	seed++;

	std::normal_distribution<double> distribution(parametar1, parametar2);

	std::pair<double, double> temp;

	for (int i = 0; i < n; ++i)
	{
		temp.first = distribution(generator);
		temp.second = distribution(generator);
		coordinates.push_back(temp);
	}	

	for (int i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j)
			matrix[i][j] = distance( coordinates[i], coordinates[j] );


}

void graph::student(int parametar)
{
	//unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);
	seed++;

	std::student_t_distribution<double> distribution(parametar);

	std::pair<double, double> temp;

	for (int i = 0; i < n; ++i)
	{
		temp.first = distribution(generator);
		temp.second = distribution(generator);
		coordinates.push_back(temp);
	}	

	for (int i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j)
			matrix[i][j] = distance( coordinates[i], coordinates[j] );


}
