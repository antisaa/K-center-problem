#include <iostream>
#include <random>
#include <chrono>
#include <cmath>
#include <vector>
#include <utility>
#include <fstream>
#include <string>
#include <iomanip>
#include "graph.h"



double distance (std::pair<double,double> first, std::pair<double,double> second)
{
	return sqrt( pow(first.first - second.first,2) + pow(first.second - second.second, 2) );

}

graph::graph(int N, int K)
{	
	n = N;
	k = K;
	std::vector<double> v(N,0);
	std::pair<double, double> p = std::make_pair(0.0, 0.0);
	for (int i = 0; i < N; ++i)
	{	
	
		matrix.push_back(v);
		if(i < K)
			chosen.push_back(true); 
		else
			chosen.push_back(false);

	}
	// dodano - Ravle
	for(int i = 0; i < n; i++)
	{
		pointAsWarehouseQuality.push_back(0);
	}
	//
}

graph::graph(std::string test_file, int N, int K) : graph(N,K)
{
    std::fstream test;
    test.open(test_file);
    int tempi, tempj, tempk;

    test >> n;

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


    std::ofstream graph_file;
    graph_file.open ("graph.txt");
    graph_file<< n << std::endl;


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

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            graph_file << matrix[i][j] << " ";
        }
        graph_file << std::endl;
    }

}

graph::graph()
{}

graph::~graph()
{}


// OPIS: ako je i-ta tocka barem druga najbliza nekoj drugoj tocki, 
// povecavamo joj kvalitetu na osnovu toga je li prva ili druga najbliza
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
				pointAsWarehouseQuality[j]+=2;
				continue;
			}
			if( secondMins[i] == matrix[i][j] )
			{
				pointAsWarehouseQuality[j]+=1;
				continue;	
			}
		}
	}
}

void graph::uniform(double parametar1, double parametar2)
{

	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);

	std::uniform_real_distribution<double> distribution(parametar1, parametar2);

	std::ofstream coordinates_file;
	coordinates_file.open ("coordinates.txt");
	std::pair<double, double> temp;
	coordinates_file << n << std::endl;
	for (int i = 0; i < n; ++i)
	{
		temp.first = distribution(generator);
		temp.second = distribution(generator);
		coordinates.push_back(temp);

		coordinates_file << temp.first << " " << temp.second << std::endl;
	}


	std::ofstream graph_file;
	graph_file.open ("graph.txt");
	graph_file << std::setprecision(5);
	graph_file<< n << std::endl;

	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			matrix[i][j] = distance( coordinates[i], coordinates[j] );

			graph_file << matrix[i][j] << " ";
		}


		graph_file << std::endl;
	}

	coordinates_file.close();
	graph_file.close();

}


void graph::poisson(double parametar)
{
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);

	std::poisson_distribution<int> distribution(parametar);

	std::ofstream coordinates_file;
	coordinates_file.open ("coordinates.txt");
	std::pair<double, double> temp;
	coordinates_file << n << std::endl;
	for (int i = 0; i < n; ++i)
	{
		temp.first = distribution(generator);
		temp.second = distribution(generator);
		coordinates.push_back(temp);

		coordinates_file << temp.first << " " << temp.second << std::endl;
	}


	std::ofstream graph_file;
	graph_file.open ("graph.txt");
	graph_file<< n << std::endl;

	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			matrix[i][j] = distance( coordinates[i], coordinates[j] );

			graph_file << matrix[i][j] << " ";
		}


		graph_file << std::endl;
	}

	coordinates_file.close();
	graph_file.close();


}


void graph::exponential(double parametar)
{
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);

	std::exponential_distribution<double> distribution(parametar);

	std::ofstream coordinates_file;
	coordinates_file.open ("coordinates.txt");
	std::pair<double, double> temp;
	coordinates_file << n << std::endl;
	for (int i = 0; i < n; ++i)
	{
		temp.first = distribution(generator);
		temp.second = distribution(generator);
		coordinates.push_back(temp);

		coordinates_file << temp.first << " " << temp.second << std::endl;
	}


	std::ofstream graph_file;
	graph_file.open ("graph.txt");
	graph_file<< n << std::endl;

	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			matrix[i][j] = distance( coordinates[i], coordinates[j] );

			graph_file << matrix[i][j] << " ";
		}


		graph_file << std::endl;
	}

	coordinates_file.close();
	graph_file.close();


}

void graph::gamma(double parametar1, double parametar2)
{
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);

	std::gamma_distribution<double> distribution(parametar1, parametar2);

	std::ofstream coordinates_file;
	coordinates_file.open ("coordinates.txt");
	std::pair<double, double> temp;
	coordinates_file << n << std::endl;
	for (int i = 0; i < n; ++i)
	{
		temp.first = distribution(generator);
		temp.second = distribution(generator);
		coordinates.push_back(temp);

		coordinates_file << temp.first << " " << temp.second << std::endl;
	}


	std::ofstream graph_file;
	graph_file.open ("graph.txt");
	graph_file<< n << std::endl;

	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			matrix[i][j] = distance( coordinates[i], coordinates[j] );

			graph_file << matrix[i][j] << " ";
		}


		graph_file << std::endl;
	}

	coordinates_file.close();
	graph_file.close();


}

void graph::weibull(double parametar1, double parametar2)
{
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);

	std::weibull_distribution<double> distribution(parametar1, parametar2);

	std::ofstream coordinates_file;
	coordinates_file.open ("coordinates.txt");
	std::pair<double, double> temp;
	coordinates_file << n << std::endl;
	for (int i = 0; i < n; ++i)
	{
		temp.first = distribution(generator);
		temp.second = distribution(generator);
		coordinates.push_back(temp);

		coordinates_file << temp.first << " " << temp.second << std::endl;
	}


	std::ofstream graph_file;
	graph_file.open ("graph.txt");
	graph_file<< n << std::endl;

	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			matrix[i][j] = distance( coordinates[i], coordinates[j] );

			graph_file << matrix[i][j] << " ";
		}


		graph_file << std::endl;
	}

	coordinates_file.close();
	graph_file.close();


}


void graph::normal(double parametar1, double parametar2)
{
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);

	std::normal_distribution<double> distribution(parametar1, parametar2);

	std::ofstream coordinates_file;
	coordinates_file.open ("coordinates.txt");
	std::pair<double, double> temp;
	coordinates_file << n << std::endl;
	for (int i = 0; i < n; ++i)
	{
		temp.first = distribution(generator);
		temp.second = distribution(generator);
		coordinates.push_back(temp);

		coordinates_file << temp.first << " " << temp.second << std::endl;
	}


	std::ofstream graph_file;
	graph_file.open ("graph.txt");
	graph_file<< n << std::endl;

	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			matrix[i][j] = distance( coordinates[i], coordinates[j] );

			graph_file << matrix[i][j] << " ";
		}


		graph_file << std::endl;
	}

	coordinates_file.close();
	graph_file.close();


}

void graph::student(int parametar)
{
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);

	std::student_t_distribution<double> distribution(parametar);

	std::ofstream coordinates_file;
	coordinates_file.open ("coordinates.txt");
	std::pair<double, double> temp;
	coordinates_file << n << std::endl;
	for (int i = 0; i < n; ++i)
	{
		temp.first = distribution(generator);
		temp.second = distribution(generator);
		coordinates.push_back(temp);

		coordinates_file << temp.first << " " << temp.second << std::endl;
	}


	std::ofstream graph_file;
	graph_file.open ("graph.txt");
	graph_file<< n << std::endl;

	for (int i = 0; i < n; ++i)
	{


		for (int j = 0; j < n; ++j)
		{
			matrix[i][j] = distance( coordinates[i], coordinates[j] );
			graph_file << matrix[i][j] << " ";
		}

		graph_file << std::endl;
	}

	coordinates_file.close();
	graph_file.close();


}
