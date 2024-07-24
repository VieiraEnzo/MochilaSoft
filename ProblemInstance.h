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
	
	int num_items; //Number of Itens in the knapsack
	int num_forfeits_pairs; //Number of forfeits pairs in the knapsack
	int budget; //Knapsack Capacity
	string filename, i_name; 
	
	int* items; // Items in the knapsack
	int* profits; //Profits of the items
	int* weights; //Weights of thse items
	int** mD;  // forfeits costs and pairs
	int* forfeits_costs; // forfeits costs
	vector<pair<int, int>> forfeits_pairs; // forfeits pairs

	void load_file(const char *const f); //Function to load the file

	virtual ~ProblemInstance(); 
	
	void PrintInstance(); //Function to print the instance

};

#endif /*PROBLEMINSTANCE_H*/