#include "ILS.h"

ILS::ILS(ProblemInstance* _p, int max_iter)
{
	p = _p;
    iter_wo_impr = max_iter; 
}

ILS::~ILS() {}

template<typename T>
T getRandomElement(const std::vector<T>& vec) {
    if (vec.empty()) {
        throw std::out_of_range("Vector is empty");
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, vec.size() - 1);
    return vec[dis(gen)];
}


string randomMove(const vector<string>& choices) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, choices.size() - 1);
    return choices[dis(gen)];
}

vector<int> remove_random_items(vector<int> sol, int nelems){
	vector<int> out; 
	sample(
        sol.begin(),
        sol.end(),
        back_inserter(out),
        nelems,
        mt19937{random_device{}()}
    );

	for(auto i : out){
		sol.erase(remove(sol.begin(), sol.end(), i), sol.end());
	}

	return sol; 
}

// Function to calculate the Cauchy PDF
double cauchy_pdf(double x, double loc, double scale) {
    double denom = M_PI * scale * (1 + std::pow((x - loc) / scale, 2));
    return 1.0 / denom;
}

// Function to generate a Cauchy random variate
double cauchy_rvs(double loc, double scale) {
    static std::default_random_engine generator;
    std::cauchy_distribution<double> distribution(loc, scale);
    return distribution(generator);
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
    // return cauchy_dist(gen);
}

bool can_add_item(vector<int> &solution, ProblemInstance* _p, int item) {
    double new_weight = 0.0;
    for(int i: solution){
        new_weight += _p->weights[i]; 
    }

    new_weight += _p->weights[item];
    return new_weight <= _p->budget; 
    
}


//Permutação
vector<int> pertubation(
    vector<int> &solution,
    ProblemInstance* _p,
    int iter,
    int max_iter,
    double current_cost,
    double best_cost,
    string flight_step
) {
   
    double flight_step_value;
    if (flight_step == "hybrid_cauchy") {
        flight_step_value = abs(hybrid_cauchy_flight_step(iter, max_iter, current_cost, best_cost));
    } else {
        cout << "Choose a valid flight step." << endl;
    }

    int num_changes = max(1, static_cast<int>(flight_step_value));
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0, 1);


    for (int i = 0; i < num_changes; ++i) {
        vector<string> move_types = {"add", "remove"};
        string move_type = randomMove(move_types);

        if (move_type == "add") {
            vector<int> addable_items;
            vector<int> items(_p->items, _p->items + _p->num_items);
            
            sort(solution.begin(), solution.end()); 
            sort(items.begin(), items.end());

            vector<int> not_sack; 
            set_difference(items.begin(), items.end(), solution.begin(), solution.end(), back_inserter(not_sack)); 

            for(int j: not_sack){
                if(can_add_item(solution, _p, j)){
                    addable_items.push_back(j);
                }
            }

            if(!addable_items.empty()){
                int randomItem = getRandomElement(addable_items); 
                solution.push_back(randomItem); 
            }
        } else if (move_type == "remove") {
            vector<int> removable_items;
            for(int j: solution){
                removable_items.push_back(j);
            }

            if (!removable_items.empty()) {
                int randomItem = getRandomElement(removable_items); 
                // cout << "removed item: " << randomItem << endl; 
                solution.erase(remove(solution.begin(), solution.end(), randomItem), solution.end());
            }
        } 
        

    }
                
    return solution; // return a neighbor solution
}

std::pair<double, vector<int>> ILS::solve(ProblemInstance* _p, Solution &solution){

    vector<int> best_sol; 
    vector<int> current_sol;  
    int iter = 0; 

    // busca local
    LocalSearch localsearch(p); 
    solution = localsearch.solve(p, solution); ;
    
    int best_cost = solution.getCost(); 
    int current_cost = best_cost; 
    best_sol = solution.getKS(); 

    while(iter < iter_wo_impr){
        iter++; 

        // initializeEliteSet(15);
        // perturbation - remove 3 random items (test);
        // current_sol = remove_random_items(solution.getKS(), 2);
        string flight_step = "hybrid_cauchy"; 
        solution.getKS() = pertubation(solution.getKS(), _p, iter, iter_wo_impr, current_cost, best_cost, flight_step);
        
        solution = localsearch.solve(_p, solution);
        current_cost = solution.getCost();


        if(current_cost > best_cost){
            best_sol = solution.getKS(); 
            best_cost = current_cost;
            
            iter = 0; 
        }

    }
    // cout << "Best ILS cost: " << best_cost << endl; 
	return make_pair(best_cost, best_sol);

}