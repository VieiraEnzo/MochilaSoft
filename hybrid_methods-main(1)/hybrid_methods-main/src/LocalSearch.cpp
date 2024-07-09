#include "LocalSearch.h"

LocalSearch::LocalSearch(ProblemInstance* _p)
{
	p = _p;
}

LocalSearch::~LocalSearch()
{

}

int LocalSearch::calculate_delta(int i, int j, vector<int> alpha_list, ProblemInstance* _p){
    return alpha_list[j] - alpha_list[i] + _p->mD[i][j]; 
}

vector<int> LocalSearch::generate_alpha_list(ProblemInstance* _p, Solution &solution) {
    vector<int> alpha_list;

    for (int i = 0; i < _p->num_items; i++){
        int value = _p->profits[i];
        int forfeits_sum = 0;
        for (int j : solution.getKS()) {
            forfeits_sum += _p->mD[i][j];
        }
        value -= forfeits_sum;
        alpha_list.emplace_back(value);
    }

    return alpha_list;
}


Solution LocalSearch::solve(ProblemInstance* _p, Solution &solution)
{	

	int in_item, out_item;
	bool Improved = true;
    int best_delta = INT_MIN, delta = INT_MIN; 


	while(Improved){
		Improved = false;
        sack = solution.getKS();

        // vector of not solution items
        vector<int> items(_p->items, _p->items + _p->num_items);
        
        sort(sack.begin(), sack.end()); 
        sort(items.begin(), items.end());

        vector<int> not_sack; 
        set_difference(items.begin(), items.end(), sack.begin(), sack.end(), back_inserter(not_sack)); 

        // generate alpha list
        vector<int> alpha_list = generate_alpha_list(_p, solution); 

        // remaining capacity
        int bres = _p->budget;
        for(int i: sack){
            bres = bres - _p->weights[i]; 
        }

        for(int i: sack){
            for(int j: not_sack){
                delta = calculate_delta(i, j, alpha_list, _p); 
                if(delta > 0){
                    if((bres - _p->weights[j] + _p->weights[i]) > 0){
                        if(delta > best_delta){
                            Improved = true; 
                            in_item = j; 
                            out_item = i; 
                            best_delta = delta; 
                        }
                    }
                }
            }
        }

		if(Improved){
			swap(solution,out_item,in_item);
		}
	}

	return solution;
}

void LocalSearch::swap(Solution &solution, int i, int j){
	solution.remove_item(i);
	solution.add_item(j);
}
