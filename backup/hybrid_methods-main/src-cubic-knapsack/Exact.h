#ifndef EXACT_H_
#define EXACT_H_

enum status {OPTIMALFOUND, SOLUTIONFOUND, INFEASIBLE};

#include <ilcplex/ilocplex.h>

#include "ProblemInstance.h"
#include "Solution.h"
#include "Pattern.h"

typedef IloArray<IloNumVarArray> IloNumVarMatrix;
typedef IloArray<IloNumArray> IloNumMatrix;

extern Solution s_best;

class Exact
{
private:

	ProblemInstance* p;
	IloConstraintArray constraints;
	IloEnv env;
	IloModel modelo;
	IloCplex Knapsack;
	IloNumVarArray x;
	
	void addConstraint_Mochila(IloEnv& env, IloModel& modelo);

public:

	Exact(ProblemInstance* _p);
	
	~Exact();

	Solution* getSolution();

	void setPrimalBound(double cost);

	Solution solve(int timelimit, const char *filename);

	status getStatus();

	IloEnv getEnv(){return env;};

};
#endif // EXACT_H_
