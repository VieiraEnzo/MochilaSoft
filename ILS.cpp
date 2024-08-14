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

    double iteration_factor = static_cast<double>(iter) / max_iter;
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
  double current_cost,
  double best_cost
) {
  double flight_step_value;
  if(flight_step == "cauchy"){
    flight_step_value = abs(hybrid_cauchy_flight_step(iter, max_iter, current_cost, best_cost));
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
Substitui a solução atual por uma aleatória no vetor
entrada:
- vetor de soluções
saída: -
tempo: O(n * #itens removidos) ou O(#itens removidos)
*/
void getRandomVector(Solution solution, const vector<vector<int>>& vec) {
    // Check if the input vector of vectors is empty
    if (vec.empty()) {
        throw invalid_argument("Input vector of vectors is empty");
    }
    
    // Create a random number generator
    random_device rd;  // Seed generator
    mt19937 gen(rd()); // Mersenne Twister generator
    uniform_int_distribution<> dis(0, vec.size() - 1); // Uniform distribution

    // Generate a random index
    int randomIndex = dis(gen);

    // Solutions become the new solution
    for(auto num : vec[randomIndex]){
      // assert(solution.can_add(num));
      if(!solution.can_add(num)) cout << "toma\n";
      solution.add_itemO(num);
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
int ILS::solve(ProblemInstance* _p, Solution &solution){
  
  Solution best_sol(_p); 
  ConstructiveCG constructive;
  LocalSearch localsearch(p); 

  std::unique_ptr<ES> EliteSet = std::make_unique<ES>(15);
  
  int no_change = 0;
  
  vector<vector<int>> patterns_reused;
  int maxStart = 5;
  
  //Constructive Parameters
  double max_iter = 2;
  double pct_rm = 0.05;
  double k = 0.30;

  constructive.Carousel_Forfeits(p, solution, max_iter, pct_rm);
  localsearch.solve(p, solution);
  int best_cost = solution.getCost();
  int current_cost = best_cost;
  best_sol = solution;

  /* 
    Grandes problemas: 
    1- tem vezes que ele nn minera nenhum padrao (ES size é pequeno)
    2- O padrão minerado, por vezes, não é uma solução feasible
  */


  for(double s=0; s <= maxStart; s++){

      cout << "entrou \n";
      int iter = 0; 
      if(s != 0){

          solution.clear();

          getRandomVector(solution, patterns_reused); 

          constructive.Carousel_Forfeits_Adaptive(p, solution, max_iter, pct_rm, k);

          assert(solution.CheckSol());

          localsearch.solve(p, solution);

          cout << "opa4 \n";
          patterns_reused.clear();

      }  
      cout << "saiu1 \n";


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

          // set<vector<int>> k;
          // for(auto a :(*EliteSet).HeapSol){
          //   k.insert(a.getKS()); 
          // }
          // cout << "ES size: " << k.size() << " \n"; 

          int suporte = 0.01 * _p->num_items;
          Mining miner(*EliteSet, suporte, 15);
          miner.map_file();
          miner.mine();
          miner.unmapall_file();

          Pattern **Mined_Patterns = miner.getlistOfPatterns();
          int pattern_size = miner.getSizePatterns();

          //Get random mined pattern to reuse
          int randPos = 0;
          // cout << "ptsize " << pattern_size << "\n";
          // if(pattern_size <= 15){
          //   set<vector<int>> s;
          //   for(int i = 0; i < pattern_size; i++){
          //     s.insert(Mined_Patterns[i]->elements);
          //   }
          //   cout << "Tamanho de diferentes : " << s.size() << " " << pattern_size << "\n"; 
          // }
          assert(pattern_size != 0);

          randPos = rand()%pattern_size;
          Pattern *Mined_Itens_reused = Mined_Patterns[randPos];
          int cost = 0;
          for(auto num : Mined_Itens_reused->elements){
            cost += _p->weights[num];
          }
          cout << "tamanho do grupo minerado " <<  (Mined_Itens_reused->elements).size() << " " << cost << "\n";
          // assert(cost <= _p->budget);
          // if(cost >)
          patterns_reused.push_back(Mined_Itens_reused->elements);


          //Construct pattern matrix
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
  }

  solution = best_sol;
	return best_cost;
}