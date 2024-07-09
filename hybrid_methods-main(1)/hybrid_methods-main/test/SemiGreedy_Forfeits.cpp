#include "SemiGreedy_Forfeits.h"
#include "Solution.h"


pair<int, double> SemiGreedy::MaxValue(map<int, double> sampleMap){ 
 
    pair<int, double> entryWithMaxValue = make_pair(INT_MIN, INT_MIN); 
 
    map<int, double>::iterator currentEntry; 
    for (currentEntry = sampleMap.begin(); currentEntry != sampleMap.end(); ++currentEntry){ 
        if(currentEntry->second > entryWithMaxValue.second){ 
            entryWithMaxValue = make_pair(currentEntry->first, currentEntry->second); 
        } 
    } 
 
    return entryWithMaxValue; 
} 


void SemiGreedy::Constructive(ProblemInstance* _p, Solution &solution){
    

    int bres = _p->budget; 
    int num_remaining_items = _p->num_items; 
    vector<int> sack;

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

        pair<int, double> best_item_value = MaxValue(ratios); 
        int best_item = best_item_value.first; 

        if(ratios[best_item] < 0){
            break; 
        }

        solution.add_item(best_item); 
        bres = bres - _p->weights[best_item]; 
        num_remaining_items = num_remaining_items - 1; 
    }
    
}; 
