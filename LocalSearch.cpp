#include "LocalSearch.h"
#include <set>
#include <list>
#include <cassert>

LocalSearch::LocalSearch(ProblemInstance* _p)
{
	p = _p;
}

LocalSearch::~LocalSearch()
{

}


Solution LocalSearch::solve(ProblemInstance* _p, Solution &solution)
{	

	bool Improved = true;
    int best_delta = INT_MIN, delta = INT_MIN; 

    list<int> sack;     //Testar set tbm
    list<int> not_sack; //Testar set tbm

    for(int i = 0; i < p->num_items; i++){
        if(solution.is_in_sack(i)) sack.push_back(i);
        else not_sack.push_back(i);
    }


	while(Improved){
        
        int cost_before = solution.getCost();
        auto itIN = sack.end();
        auto itOUT = not_sack.end();

        for(auto itSack = sack.begin(); itSack != sack.end(); itSack++){
            int i = *itSack;
            solution.remove_itemO(i);
            for(auto itNotSack = sack.begin(); itNotSack != sack.end(); itNotSack++){
                int j = *itNotSack;
                solution.add_itemO(j);
                int cost_after = solution.getCost();
                int delta = cost_before - cost_after;
                if(delta > best_delta && delta > 0 && solution.can_add(j)){
                    Improved = true;
                    best_delta = delta;
                    itOUT = itSack;
                    itIN = itNotSack;
                }
                solution.remove_itemO(j);
            }
            solution.add_itemO(i);
        }

		if(Improved){
            assert(best_delta != INT_MIN);
			swap(solution,*itOUT,*itIN);
            sack.erase(itOUT);
            sack.push_back(*itIN);
            not_sack.erase(itIN);
            not_sack.push_back(*itOUT);
		}
	}

	return solution;
}

void LocalSearch::swap(Solution &solution, int out_item, int in_item){
	solution.remove_itemO(out_item);
	solution.add_itemO(in_item);
}
