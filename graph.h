#ifndef graph_h_included
#define graph_h_included

#include <iostream>
#include <vector>
#include <utility>
#include <string>


class graph
{
public:
	int n,k;													// vrhovi u grafu
	std::vector< std::vector<double> > matrix;				// matrica n*n; Aij je udaljenost izmedu vrha "i" i vrha "j"
	std::vector< std::pair<double,double> > coordinates;	// popis 2D koordinata vrhova grafa
	static unsigned seed;

	std::vector< int > pointAsWarehouseQuality;				// vektor koji govori koliko je tocka na i-tom mjestu kvalitetna kao skladiste
	void countQuality();

	graph(std::string, int);
	graph(int, int);										// konstruktor koji inicijalizira vectore, parametar je n i k		// konstruktor koji inicijalizira vektor pomocu .txt koordinata
	graph();												// prazan konstruktor
	~graph();												// prazan destruktor


	//generatori slucajnih grafova, argumenti su parametri distribucija
	void uniform(double, double);
	void poisson(double);
	void exponential(double);
	void gamma(double, double);
	void weibull(double, double);
	void normal(double, double);
	void student(int);
	

};


#endif