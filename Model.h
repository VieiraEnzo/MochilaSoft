// #pragma once
#ifndef Model_H_
#define Model_H_

enum status {OPTIMALFOUND, SOLUTIONFOUND, INFEASIBLE};


#include "ProblemInstance.h"
#include "ilcplex/ilocplex.h"
#include <iostream>
#include <vector>
#include "Solution.h"



using namespace std;

class Model {
    private:
        ProblemInstance* p;
	    // Solution kp_solution (ProblemInstance* p);
        IloEnv env;
        IloModel model;
        IloCplex knapsack;
        IloCplex knapsack_lb;
        IloBoolVarArray x; 
        IloNumVarArray v; 
        IloBoolVarArray z; 
        time_t start, end;
        void addConstraintTotalCapacity(IloEnv &env,IloModel &model,IloBoolVarArray &x, ProblemInstance* _p);
        void addConstraintLinearization(IloEnv &env,IloModel &model,IloBoolVarArray &x, IloNumVarArray &v, ProblemInstance* _p);
        void addObjective(IloEnv &env, IloModel &model, IloBoolVarArray &x, IloNumVarArray &v, ProblemInstance* _p);
        void addConstraintofChoosePattern(IloEnv &env, IloModel &model, IloBoolVarArray &z, int num_patterns); 
        void addConstraintofFixPattern(IloEnv &env, IloModel &model, IloBoolVarArray &x, IloBoolVarArray &z, const std::vector<std::vector<int>>& pattern_matrix, int num_patterns, ProblemInstance* _p); 
        void addConstraintLocalBranching(IloEnv &env,IloModel &model,IloBoolVarArray &x,std::vector<int> &solution, ProblemInstance* _p);
    public:
        Model(ProblemInstance* _p);
        ~Model();
        void Build_Model(ProblemInstance* _p);
        std::pair<Solution, double> Build_Model_with_LB(ProblemInstance* _p, vector<int> best_solution);
        std::pair<Solution, double> Build_Model_with_Patterns(ProblemInstance* _p, int num_patterns, const std::vector<std::vector<int>>& pattern_matrix, vector<int> best_solution);
        status getStatus();
        // std::tuple <std::vector<int>,int> solveLB(std::vector<int> &solution,int timebound,double upperbound);
        void setMIPstart(vector<int> &Solution, IloCplex &cplex, IloBoolVarArray &x, ProblemInstance* _p);
        // IloEnv getEnv(){return env;};
    
        
};

#endif /*Model_H_*/