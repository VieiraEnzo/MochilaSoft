#ifndef ILS_H_
#define ILS_H_
#include <stdio.h>
#include <vector>
#include <iostream>
#include <random>
#include <utility> 
#include <cmath>
#include "ES.h"
#include "Solution.h"
#include "LocalSearch.h"
#include "Mining.h"
#include "Pattern.h"

using namespace std;

class ILS
{
private:
	ProblemInstance* p;
	Solution solution (ProblemInstance* p);
    Solution best_solution (ProblemInstance* p); 
    vector<int> sack;
	int iter_wo_impr; 
public:

	ILS(ProblemInstance* _p, int max_iter);

	virtual ~ILS();

	pair<double, vector<int>> solve(ProblemInstance* _p, Solution &solution);
};

#endif /*ILS_H_*/
