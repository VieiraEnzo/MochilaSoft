#ifndef SOLUTION_H
#define SOLUTION_H

#include "ProblemInstance.h"

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <algorithm>    // std::sort

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
	double getCapacity(){return used_capacity;};
	void add_item(int i);
	void remove_item(int i);
	vector<int>& getKS();

	bool operator()(const Solution& a,const Solution& b) const{
		return(a.cost<b.cost);
	}	
	void PrintSolution(const char *filename, const char* type ); //Function to print the solution

	 
};

#endif // SOLUTION_H