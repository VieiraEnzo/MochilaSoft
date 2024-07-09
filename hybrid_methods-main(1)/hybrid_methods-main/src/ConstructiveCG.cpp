#include "ConstructiveCG.h"
#include "Solution.h"
#include <cassert>

void ConstructiveCG::Greedy_Forfeits_Init(ProblemInstance* _p, Solution &solution){
    int bres = _p->budget; 
    int num_remaining_items = _p->num_items; 
    vector<int> sack;

    for(int i: solution.getKS()){
        bres = bres - _p->weights[i]; 
    }

    while(num_remaining_items != 0){
        vector<int> Xiter; 
        map<int,double> ratios;  
		sack = solution.getKS();

        for(int i = 0; i < _p->num_items; i++){
            if(_p->weights[i] <= bres && !solution.is_in_sack(i)){
                Xiter.push_back(i); 
            }
        }
        
        if(Xiter.empty()){
            break; 
        }

        for(int i: Xiter){
            profit = _p->profits[i];
            for(int j: sack){
                profit = profit - _p->mD[i][j]; 
            }
            ratios.insert({i, profit / _p->weights[i]}); 

        }

        pair<int, double> best_item_value = constructive.MaxValue(ratios); 
        int best_item = best_item_value.first; 

        if(ratios[best_item] < 0){
            break; 
        }

        solution.add_item(best_item); 
        bres = bres - _p->weights[best_item]; 
        num_remaining_items = num_remaining_items - 1; 
    }

}; 


int ConstructiveCG::Greedy_Forfeits_Single(ProblemInstance* _p, Solution &solution){
    int bres = _p->budget; 
    int best_item = INT_MIN; 
    vector<int> Xiter; 
    map<int,double> ratios;

    for(int i: solution.getKS()){
        bres = bres - _p->weights[i]; 
    }

    for(int i = 0; i < _p->num_items; i++){
        if(_p->weights[i] <= bres && !solution.is_in_sack(i)){
            Xiter.push_back(i); 
        }
    }

    if(Xiter.empty()){
        return best_item; 
    }

    for(int i: Xiter){
        profit = _p->profits[i];
        for(int j: solution.getKS()){
            profit = profit - _p->mD[i][j]; 
        }
        ratios.insert({i, profit / _p->weights[i]}); 

    }

    if(ratios.empty()){
        return best_item; 
    }

    pair<int, double> best_item_value = constructive.MaxValue(ratios);  
    best_item = best_item_value.first; 

    // assert(best_item_value.second >= 0);
    if(best_item_value.second < 0)
        return INT_MIN;

    return best_item;  

}; 


void ConstructiveCG::Carousel_Forfeits(ProblemInstance* _p, Solution &solution, double max_iter, double pct_rm){


    constructive.Greedy_Forfeits(_p, solution);

    cout << "--------- ACABOU O GULOSO ---------" << "\n";

    int num_items_removed = solution.getKS().size() * pct_rm;

    for(int i = 0; i < num_items_removed; i++){
	    cout << "Removeu ultimo " << solution.getKS().back() << "\n";
        solution.getKS().pop_back();    
    }

    int carousel_iter = max_iter * solution.getKS().size(); 

    for(int i = 0; i < carousel_iter; i++){
        solution.remove_oldest_choice(); 
        int greedy_enter_item = Greedy_Forfeits_Single(_p, solution);
        if(!(greedy_enter_item < 0)){
            solution.add_item(greedy_enter_item); 
        }
    }

    cout << "--------- ACABOU O CARROSEL ---------" << "\n";

    Greedy_Forfeits_Init(_p, solution);  

}; 


void ConstructiveCG::Carousel_Forfeits_Adaptive(ProblemInstance* _p, Solution &solution, double max_iter, double pct_rm, double p){

    constructive.Greedy_Forfeits(_p, solution);

    int num_items_removed = solution.getKS().size() * pct_rm;

    for(int i = 0; i < num_items_removed; i++){ 
        solution.getKS().pop_back();    
    }

    int carousel_iter = max_iter * solution.getKS().size(); 

    for(int i = 0; i < carousel_iter; i++){
        solution.remove_oldest_choice_adaptive(p); 
        int greedy_enter_item = Greedy_Forfeits_Single(_p, solution);
        if(!(greedy_enter_item < 0)){
            solution.add_item(greedy_enter_item); 
        }
    }

    Greedy_Forfeits_Init(_p, solution);  

}; 