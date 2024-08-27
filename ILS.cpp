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

double hybrid_cauchy_flight_step(int iter, int max_iter, double current_cost, double best_cost) { 
    double initial_scale = 1.0;
    double final_scale = 0.1;
    double progress = iter / max_iter;
    double scale = initial_scale * progress + final_scale * (1 - progress);

    double improvement_factor = std::max(
        0.0, (best_cost - current_cost) / std::max(std::abs(best_cost), 1e-10)
    );

    double iteration_factor = (double)(iter) / (double)max_iter;
    scale = (scale) * (1 - iteration_factor) + (improvement_factor * iteration_factor);  

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
  string &flight_step,
  int current_cost,
  int best_cost
) {
  double flight_step_value;
  if(flight_step == "cauchy"){
    flight_step_value = abs(hybrid_cauchy_flight_step(iter, max_iter, current_cost, best_cost));
    // flight_step_value = abs(cauchy_flight_step(iter, max_iter));
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


void getRandomVector(Solution &solution, vector<vector<int>>& vec) {
    // Check if the input vector of vectors is empty
    if (vec.empty()) {
        throw invalid_argument("Input vector of vectors is empty");
    }
    
    // Create a random number generator
    mt19937 rng((uint32_t)chrono::steady_clock::now().time_since_epoch().count());    
    int randomIndex = uniform_int_distribution<int>(0, vec.size() - 1)(rng);

    // Solutions become the new solution
    for(auto num : vec[randomIndex]){
      solution.add_itemO(num);
    }
}

/*
Diagnostico depois de infinitos testes:
  -> Versões antigas ainda davam esse erro
  -> Erro gerado principalmente com um valor de suporte maior (mais provavel de encontrar n_sol < suporte)
  -> Erro pode acontecer em qualquer iteração do algoritmo (nn tem padrao)
*/

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

  // std::chrono::duration<double> total_duration = std::chrono::duration<double>::zero();

  LocalSearch localsearch(p); 
  localsearch.solve(p, solution);
  
  int best_cost = solution.getCost();

  int current_cost = best_cost;
  best_sol = solution;

  std::unique_ptr<ES> EliteSet = std::make_unique<ES>(15);

  //Multstart
  vector<vector<int>> patterns_reused;
  const int maxStart = 5;

  for(int s=0; s < maxStart; s++){
    
    int no_change = 0;
    int iter = 0;

    if(s!= 0){
        solution.clear();
        getRandomVector(solution, patterns_reused);
        const double iterAdapt = 2, pct_rm = 0.05, k = 0.30;
        constructive.Carousel_Forfeits_Adaptive(p, solution, iterAdapt, pct_rm, k);
        localsearch.solve(p, solution);
        patterns_reused.clear();
        current_cost = solution.getCost();
    }

    while(iter < iter_wo_impr){
      iter++;
      string flight_step = "cauchy"; 
      perturbate(solution, _p, iter, iter_wo_impr, flight_step, current_cost, best_cost);
      localsearch.solve(_p, solution);

      current_cost = solution.getCost();

      no_change += 1;
      if(EliteSet->add(solution)){
        no_change = 0;
      }

      assert(no_change <= 15);
      if(no_change == 15){
        no_change = 0;

        const int suporte = min((int)(0.01*(double)_p->num_items), (int) (*EliteSet).getESsize());
        // auto start = std::chrono::high_resolution_clock::now();
        Mining miner(*EliteSet, suporte, 15);
        miner.map_file();
        miner.mine();
        miner.unmapall_file();
        Pattern **Mined_Patterns = miner.getlistOfPatterns();
        // auto end = std::chrono::high_resolution_clock::now();
        // std::chrono::duration<double> duration = end - start;
        // total_duration += duration;

        int pattern_size = miner.getSizePatterns();

        int randPos = rand()%pattern_size;
        Pattern *Mined_Itens_reused = Mined_Patterns[randPos];
        patterns_reused.push_back(Mined_Itens_reused->elements); 

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
        pair<Solution, int> model_result = kpf_model.Build_Model_with_Patterns(_p, pattern_size, pattern_matrix, elements, best_cost);//,best_cost
        solution = model_result.first;
        current_cost = model_result.second;

        if(current_cost == 0){ 
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
  }
  solution = best_sol;
  // cout << "total time mining: " << total_duration.count() << "\n";
  return best_cost;
}