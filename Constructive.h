#ifndef CONSTRUCTIVE_H_
#define CONSTRUCTIVE_H_

#include "ProblemInstance.h"
#include "Solution.h"
 
#include <iostream>
#include <vector>
#include <map>
#include <climits>

using namespace std;

class Constructive{
  public:        
    void Greedy_Forfeits(ProblemInstance* _p, Solution &solution);
}; 

#endif //CONSTRUCTIVE_H_