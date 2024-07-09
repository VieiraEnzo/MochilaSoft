#ifndef PROBLEMINSTANCE_H_
#define PROBLEMINSTANCE_H_

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <exception>
#include <utility>
#include <vector>

using namespace std;

class ProblemInstance
{
public:

	int NbItens; //Number of Itens in the knapsack

	int * Value; //Values of the items
	int * Weight; //Weights of thse items
	int knapsackCapacity; //Knapsack Capacity

	void load_file(const char *const f); //Function to load the file

	virtual ~ProblemInstance(); 

	void PrintInstance(); //Function to print the instance
};

#endif /*PROBLEMINSTANCE_H*/