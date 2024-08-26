#include "ILS.h"

ILS::ILS(ProblemInstance* _p, int max_iter){
	p = _p;
  iter_wo_impr = max_iter; 
}

ILS::~ILS(){}

/*
gera um PDF de cauchy
entrada:
- parâmetros da distribuição
saída:
- pdf de cauchy
tempo: O(1)
*/
double cauchy_pdf(double x, double loc, double scale) {
  double denom = M_PI * scale * (1 + std::pow((x - loc) / scale, 2));
  return 1.0 / denom;
}

/*
gera uma variável aleatória de cauchy
entrada:
- parâmetros da distribuição
saída: a variável gerada
tempo: O(1)
*/
double cauchy_rvs(double loc, double scale) {
  static std::default_random_engine generator;
  std::cauchy_distribution<double> distribution(loc, scale);
  return distribution(generator);
}

/*
gera # itens que serão removidos
entrada:
- parâmetros da distribuição
saída:
- #itens que serão removidos
tempo: O(1)
*/
double cauchy_flight_step(int iter, int max_iter) {
  double initial_scale = 1.0;
  double final_scale = 0.1;
  double progress = iter / max_iter;
  double scale = initial_scale * progress + final_scale * (1 - progress);
  double x = cauchy_rvs(0, scale);  // Random variate (sample)
  return cauchy_pdf(x, 0, scale);
}

enum choice{
  add,
  rem
};

/*
perturba uma solução parcial (remove alguns itens e adiciona outros aleatoriamente)
entrada:
- solução parcial (que será modifica)
- instância do problema
- parâmetros da distribuição de cauchy
saída: -
tempo: O(n * #itens removidos) ou O(#itens removidos)
*/
void perturbate(
  Solution &solution,
  ProblemInstance* _p,
  int iter,
  int max_iter,
  string &flight_step
) {
  double flight_step_value;
  if(flight_step == "cauchy"){
    flight_step_value = abs(cauchy_flight_step(iter, max_iter));
  }else{
    cout << "Choose a valid flight step." << endl;
    assert(0);
  }

  mt19937 rng((uint32_t)chrono::steady_clock::now().time_since_epoch().count());
  auto get_random = [&](int l, int r){
    return uniform_int_distribution<int>(l, r)(rng);    
  };

  int n = _p->num_items;
  int amnt_chg = max(1, (int)flight_step_value);
  for(int i = 0; i < amnt_chg; i++){
    int type = get_random(0, 1);
    if(type == add){
      vector <int> can_add;
      for(int i = 0; i < n; i++){
        if(solution.can_add(i)){
          can_add.push_back(i);
        }
      }
      if(can_add.size() > 0){
        int id = get_random(0, can_add.size()-1);
        id = can_add[id];
        assert(solution.can_add(id));
        solution.add_itemO(id); //adicionar em O(1) ou O(n)?
      }
    }else if(type == rem){
      vector <int> can_rem;
      for(int i = 0; i < n; i++){
        if(solution.is_in_sack(i)){
          can_rem.push_back(i);
        }
      }
      if(can_rem.size() > 0){
        int id = get_random(0, can_rem.size()-1);
        id = can_rem[id];
        solution.remove_itemO(id); //remover em O(1) ou O(n)?
      }
    }
  }
}

/*
faz a iterated local search em uma solução parcial gerada pelo construtivo
entrada:
- instância do problema
- solução parcial
saída:
- custo da melhor solução encontrada
tempo: O(#total de iterações) * O(perturbate)
*/
int ILS::solve(ProblemInstance* _p, Solution &solution, ConstructiveCG &constructive){
  Solution best_sol(_p); 

  LocalSearch localsearch(p); 
  localsearch.solve(p, solution);
  
  int best_cost = solution.getCost();

  int current_cost = best_cost;
  best_sol = solution; 

  std::unique_ptr<ES> EliteSet = std::make_unique<ES>(15);
  int no_change = 0;
  int iter = 0;

  while(iter < iter_wo_impr){
    iter++;
    string flight_step = "cauchy"; 
    perturbate(solution, _p, iter, iter_wo_impr, flight_step); 
    localsearch.solve(_p, solution);

    current_cost = solution.getCost();

    no_change += 1;
    if(EliteSet->add(solution)){
      no_change = 0;
    }

    assert(no_change <= 15);
    if(no_change == 15){
      no_change = 0;

      // int suporte = 2;
      const int suporte = 0.01 * (double) _p->num_items;
      Mining miner(*EliteSet, suporte, 15);
      miner.map_file();
      miner.mine();
      miner.unmapall_file();

      Pattern **Mined_Patterns = miner.getlistOfPatterns();
      int pattern_size = miner.getSizePatterns();

      if(pattern_size == 0){
        cout << iter << " " << no_change << " "  << solution.get_size()  << " " << suporte << "\n";
        cout << "EliteSetSiz: " << (*EliteSet).getESsize() << "\n"; 
        for(auto a : (*EliteSet).HeapSol){
          for(auto b : a.getKS()){
            cout << b << " ";
          }
          cout << "\n";
        }
        exit(1);
      }


      vector <vector<int>> pattern_matrix(_p->num_items, vector<int>(pattern_size));
      for(int i = 0; i < pattern_size; i++){
        Pattern *Mined_Items = Mined_Patterns[i];
        for(int tmp : Mined_Items->elements){
          pattern_matrix[tmp][i] = 1;
        }
      }

      vector <int> elements;
      int num = 0;
      for(bool tmp : best_sol.inside){
        if(tmp == 1){
          elements.push_back(num);
        }
        num += 1;
      }

      Model kpf_model(_p);
      pair<Solution, int> model_result = kpf_model.Build_Model_with_Patterns(_p, pattern_size, pattern_matrix, elements, best_cost);
      solution = model_result.first;
      current_cost = model_result.second;

      if(current_cost == 0){ // Infeasible
          solution = best_sol;
          current_cost = best_cost;
      }
    

      EliteSet = std::make_unique<ES>(15);

      
    }

    if(current_cost > best_cost){
      best_sol = solution; 
      best_cost = current_cost;
      iter = 0; 
    }

  }

  solution = best_sol;
	return best_cost;
}