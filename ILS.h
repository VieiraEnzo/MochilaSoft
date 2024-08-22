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
#include "ConstructiveCG.h"
#include "Model.h"
#include "Mining.h"
#include "Pattern.h"
#include <cassert>
#include <memory>

using namespace std;

class ILS{
  private:
    ProblemInstance* p;
    int iter_wo_impr;
  public:
    ILS(ProblemInstance* _p, int max_iter);
    virtual ~ILS();
    int solve(ProblemInstance* _p, Solution &solution, ConstructiveCG &constructive);
};

#endif /*ILS_H_*/