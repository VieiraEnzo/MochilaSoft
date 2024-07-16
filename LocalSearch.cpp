#include "LocalSearch.h"
#include <set>
#include <list>
#include <cassert>

LocalSearch::LocalSearch(ProblemInstance* _p)
{
	p = _p;
}

LocalSearch::~LocalSearch() {}


int LocalSearch::calculate_delta(int i, int j, vector<int> &alpha_list, ProblemInstance* _p){
    return alpha_list[j] - alpha_list[i] + _p->mD[i][j];
}

void LocalSearch::generate_alpha_list(ProblemInstance* _p, Solution &solution, vector<int> &alpha_list) {

    for (int i = 0; i < p->num_items; i++){
        alpha_list[i] += p->profits[i];
        for(int j = 0; j < p->num_items; j++){
            if(solution.is_in_sack(j)) alpha_list[i] -= p->mD[i][j];
        }
    }

}



void LocalSearch::solve(ProblemInstance* _p, Solution &solution)
{	
    // cout << "Entrou Local \n";
    int best_delta = 0; bool Improved = true;
    vector<int> alpha_lista(p-> num_items);
    generate_alpha_list(p, solution, alpha_lista);

    // int iter = 0;
	while(Improved){    

        // cout << iter++ << ": " << solution.getCost() << " " << solution.num_items_in_sol << "\n";

        vector<int> sack;     //Testar set tbm
        vector<int> not_sack; //Testar set tbm
        int in_item = -1, out_item = -1;

        for(int i = 0; i < p->num_items; i++){
            if(solution.is_in_sack(i)) sack.push_back(i);
            else not_sack.push_back(i);
        }

        Improved = false;


        for(auto i : sack){
            for(auto j : not_sack){
                if( -p->weights[i]  +  p->weights[j] + solution.used_capacity > p->budget) continue;
                int delta = calculate_delta(i,j, alpha_lista, p);
                if(delta > best_delta && delta > 0){
                    Improved = true;
                    best_delta = delta;
                    out_item = i;
                    in_item = j;
                }
            }
        }

		if(Improved){		
			swap(solution,out_item, in_item);
            for(int i = 0; i < p->num_items; i++){
                alpha_lista[i] += p->mD[i][out_item];
                alpha_lista[i] -= p->mD[i][in_item];
            }
		}

	}

}

void LocalSearch::swap(Solution &solution, int out_item, int in_item){
	solution.remove_itemO(out_item);
	solution.add_itemO(in_item);
}
