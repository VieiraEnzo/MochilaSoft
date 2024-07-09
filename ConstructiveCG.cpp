#include "ConstructiveCG.h"
#include "Solution.h"

/*
preenche gulosamente uma mochila parcialmente cheia
entrada: instancia do problema, solução parcial
saída: -
tempo: O(n^2)
*/
void ConstructiveCG::Greedy_Forfeits_Init(ProblemInstance* _p, Solution &solution){
  int new_item;
  do{
    new_item = Greedy_Forfeits_Single(_p, solution);
  }while(new_item != -1);
};

/*
adiciona gulosamente um item em uma mochila parcialmente cheia
entrada: instância do problema, solução parcial
saída: elemento adicionado
tempo: O(n)
*/
int ConstructiveCG::Greedy_Forfeits_Single(ProblemInstance* _p, Solution &solution){
  int best = -1;

  auto change_best = [&](int& a, int b){
    if(a == -1){
      best = b;
    }else{
      double r1 = (double)profits[a]/_p->weights[a];
      double r2 = (double)profits[b]/_p->weights[b];
      if(r2 > r1){
        best = b;
      }
    }
  };

  for(int j = 0; j < n; j++){
    if(solution.can_add(j)){
      change_best(best, j);
    }
  }

  if(best == -1 || (double)profits[best]/(double)_p->weights[best] < 0){
    return -1;
  }

  solution.add_item(best);

  for(int j = 0; j < n; j++){ //um cara pode ter conflito com ele mesmo?
    profits[j] -= _p->mD[best][j];
  }
  
  return best;
}; 

/*
construtivo carrosel para o ILS
entrada: instância do problema, solução vazia (que será modificada)
saída: -
tempo: O(n^2)
*/
void ConstructiveCG::Carousel_Forfeits(ProblemInstance* _p, Solution &solution, 
                                       double max_iter, double pct_rm){
  n = _p->num_items; //mochila chega vazia
  profits.resize(n);

  constructive.Greedy_Forfeits(_p, solution);

  // cout << "--------- ACABOU O GULOSO ---------" << "\n";

  int removals = solution.get_size() * pct_rm;
  for(int i = 0; i < removals; i++){ 
    int rem = solution.remove_newest_choice();    
  }

  for(int i = 0; i < n; i++){
    profits[i] = _p->profits[i];
  }
  for(int i = 0; i < n; i++){ 
    if(solution.is_in_sack(i)){
      for(int j = 0; j < n; j++){
          profits[j] -= _p->mD[i][j];        
      }
    }
  }
  //profits estão certos :)

  int it = solution.get_size() * max_iter; 
  for(int i = 0; i < it; i++){
    int rem = solution.remove_oldest_choice();
    for(int j = 0; j < n; j++){
      if(rem != -1) profits[j] += _p->mD[rem][j];
    }
    Greedy_Forfeits_Single(_p, solution);
  }

  // cout << "--------- ACABOU O CARROSEL ---------" << "\n";

  Greedy_Forfeits_Init(_p, solution);
}; 