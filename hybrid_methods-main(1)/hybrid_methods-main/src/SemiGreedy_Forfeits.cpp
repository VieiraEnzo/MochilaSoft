#include "SemiGreedy_Forfeits.h"
#include "Solution.h"


pair<int, double> SG_Constructive::MaxValue(map<int, double> sampleMap){ 
 
    pair<int, double> entryWithMaxValue = make_pair(INT_MIN, INT_MIN); 
 
    map<int, double>::iterator currentEntry; 
    for (currentEntry = sampleMap.begin(); currentEntry != sampleMap.end(); ++currentEntry){ 
        if(currentEntry->second > entryWithMaxValue.second){ 
            entryWithMaxValue = make_pair(currentEntry->first, currentEntry->second); 
        } 
    } 
 
    return entryWithMaxValue; 
} 


void SG_Constructive::Semigreedy_Forfeits(ProblemInstance* _p, Solution &solution, double alpha){
    

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

        // Obtain RCL bounds
        auto max_it = max_element(ratios.begin(), ratios.end(),
                                  [](const pair<int, double>& a, const pair<int, double>& b) {
                                      return a.second < b.second;
                                  });
        auto min_it = min_element(ratios.begin(), ratios.end(),
                                  [](const pair<int, double>& a, const pair<int, double>& b) {
                                      return a.second < b.second;
                                  });

        double ub = max_it->second;
        double lb = min_it->second + alpha * (max_it->second - min_it->second);

        // Construct restricted candidate list (RCL)
        vector<int> rcl;
        for (auto& ratio : ratios) {
            if (ratio.second >= lb && ratio.second <= ub) {
                rcl.push_back(ratio.first);
            }
        }

        if(rcl.empty()){
            break;
        }

        // Choose a random element from RCL
        srand(time(0));  // Initialize random seed
        int rand_index = rand() % rcl.size();
        int candidate = rcl[rand_index];

        if(ratios[candidate] < 0){
            break; 
        }

        solution.add_item(candidate); 
        bres = bres - _p->weights[candidate]; 
        num_remaining_items = num_remaining_items - 1; 
    }
    
}; 
