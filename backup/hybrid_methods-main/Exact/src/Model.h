// #pragma once
#ifndef Model_H_
#define Model_H_

enum status {OPTIMALFOUND, SOLUTIONFOUND, INFEASIBLE};


#include "ProblemInstance.h"
#include "ilcplex/ilocplex.h"
#include <iostream>
#include <vector>



using namespace std;

class Model {
    private:
        ProblemInstance* p;
	    // Solution kp_solution (ProblemInstance* p);
        IloEnv env;
        IloModel model;
        IloCplex knapsack;
        IloNumVarArray x; 
        IloNumVarArray v; 
        time_t start, end;
        void addConstraintTotalCapacity(IloEnv &env,IloModel &model,IloNumVarArray &x, ProblemInstance* _p);
        void addConstraintLinearization(IloEnv &env,IloModel &model,IloNumVarArray &x, IloNumVarArray &v, ProblemInstance* _p);
        void addObjective(IloEnv &env, IloModel &model, IloNumVarArray &x, IloNumVarArray &v, ProblemInstance* _p);
    public:
        Model(ProblemInstance* _p);
        ~Model();
        void Build_Model(ProblemInstance* _p);
        status getStatus();   
        
};

#endif /*Model_H_*/
