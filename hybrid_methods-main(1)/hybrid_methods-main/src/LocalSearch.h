#ifndef LOCALSEARCH_H_
#define LOCALSEARCH_H_
#include <stdio.h>
#include <vector>
#include "Solution.h"

#include <algorithm> // for std::min and std::max
#include <climits>



using namespace std;

class LocalSearch
{
private:
	ProblemInstance* p;
	Solution solution (ProblemInstance* p);
    vector<int> sack;

public:

	LocalSearch(ProblemInstance* _p);

	virtual ~LocalSearch();

	Solution solve(ProblemInstance* _p, Solution &solution);
	// usar reserved no alpha list
    vector<int> generate_alpha_list(ProblemInstance* _p, Solution &solution); 
    int calculate_delta(int i, int j, vector<int> alpha_list, ProblemInstance* _p); 

	void swap(Solution &Mochila,int i, int j);
};

#endif /*LOCALSEARCH_H_*/
