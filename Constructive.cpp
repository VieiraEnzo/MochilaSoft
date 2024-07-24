#include "Constructive.h"
#include "Solution.h"

/*
cria uma solução gulosa usando o critério da mochila fracionária
tempo: O(n^2)
*/
void Constructive::Greedy_Forfeits(ProblemInstance* _p, Solution &solution){ //O(n^2)

  int n = _p->num_items;

  vector <int> profit(n);
  for(int i = 0; i < n; i++){
    profit[i] = _p->profits[i];
  }

  auto change_best = [&](int& a, int b){
    if(a == -1){
      return b;
    }else{
      double r1 = (double)profit[a]/_p->weights[a];
      double r2 = (double)profit[b]/_p->weights[b];
      if(r2 > r1){
        return b;
      }
    }
    return a;
  };

  for(int i = 0; i < n; i++){
    int best = -1;
    for(int j = 0; j < n; j++){
      if(solution.can_add(j)){ //trocar por um set?
        best = change_best(best, j);
      }
    }
    if(best == -1 || (double)profit[best]/_p->weights[best] < 0){
      break;
    }

    solution.add_item(best);
    for(int j = 0; j < n; j++){ //um cara pode ter conflito com ele mesmo?
      if(j != best){
        profit[j] -= _p->mD[best][j];
      }
    }
  }
};