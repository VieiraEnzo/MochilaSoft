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

class ConstructiveCG{
  private:
    int n;
    int cap;
    vector <double> profits;
    Constructive constructive; 

  public:
    int Greedy_Forfeits_Single(ProblemInstance* _p, Solution &solution);
    void Greedy_Forfeits_Init(ProblemInstance* _p, Solution &solution); 
    void Carousel_Forfeits(ProblemInstance* _p, Solution &solution, 
                            double max_iter, double pct_rm); 
}; 
#endif //ConstructiveCG_H_