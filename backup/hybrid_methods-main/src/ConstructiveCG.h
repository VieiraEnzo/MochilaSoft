#ifndef ConstructiveCG_H_
#define ConstructiveCG_H_

#include "ProblemInstance.h"
#include "Solution.h"
#include "Constructive.h"

#include <climits>
#include <map> 
#include <iostream>
#include <vector>

using namespace std;

class ConstructiveCG
{
    private:
        ProblemInstance* p;
		Solution solution (ProblemInstance* p); 
        Constructive constructive; 
 

    public:
        double profit, max_iter, pct_rm; 
        int pair1, pair2; 
        vector<int> ratios; 

        int Greedy_Forfeits_Single(ProblemInstance* _p, Solution &solution);
        void Greedy_Forfeits_Init(ProblemInstance* _p, Solution &solution); 
        void Carousel_Forfeits(ProblemInstance* _p, Solution &solution, 
                                double max_iter, double pct_rm); 
        void Carousel_Forfeits_Adaptive(ProblemInstance* _p, Solution &solution, 
                                double max_iter, double pct_rm, double p); 
}; 


#endif /*ConstructiveCG_H_*/