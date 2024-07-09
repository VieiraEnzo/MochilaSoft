#ifndef SOLUTION_H
#define SOLUTION_H

#include "ProblemInstance.h"

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <algorithm>    // std::sort, std::shuffle
#include <random>    // for std::default_random_engine

using namespace std;

class Solution
{
private:

	vector<int> Sack;
	double cost;
	double used_capacity;
	unsigned long hash;
	ProblemInstance* p;

	friend ostream& operator<<(ostream&, Solution&);

public:

	friend bool operator< (Solution &a, Solution &b);
	friend bool operator> (Solution &a, Solution &b);
	friend bool operator== (Solution &a, Solution &b);

	Solution(ProblemInstance* _p);

	~Solution();

	double getCost();
	int size(); // number of items in solution 

	void add_item(int i);
	void remove_item(int i);
	void remove_oldest_choice();  // remove the first item added to the solution
	void remove_oldest_choice_adaptive(double p); // remove a random item from a subset of the first item added
	bool is_in_sack(int i); // check if an item is in solution
	vector<int>& getKS();
	vector<int> convertToBinaryVector(); 


	// void PrintSolution(const char *filename); //Function to print the solution

	 
};

#endif // SOLUTION_H