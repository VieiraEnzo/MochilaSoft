#ifndef SEMICONSTRUCTIVE_H_
#define SEMICONSTRUCTIVE_H_

#include "ProblemInstance.h"
#include "Solution.h"
 
#include <iostream>
#include <vector>
#include <map>
#include <climits>

using namespace std;

class SG_Constructive
{
    private:
        ProblemInstance* p;
		Solution solution (ProblemInstance* p);  

    public:
        int pair1, pair2; 
        double profit; 
        
        void Semigreedy_Forfeits(ProblemInstance* _p, Solution &solution, double alpha); 
        int* removeElement(int* arr, int& size, int element); 
        pair<int, double> MaxValue(map<int, double> sampleMap); 

}; 


#endif /*CONSTRUCTIVE_H_*/