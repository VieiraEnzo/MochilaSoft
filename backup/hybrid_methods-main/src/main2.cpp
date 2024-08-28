#include <iostream>
#include <filesystem>
#include <iomanip>
#include <chrono>
#include <algorithm> // For std::max_element
#include <iterator>  // For std::distance
#include "ProblemInstance.h"
#include "Solution.h"
#include "Constructive.h"
#include "ConstructiveCG.h"
#include "ILS.h"
#include "SemiGreedy_Forfeits.h"
// #include "Model.h"
// #include "Mining.h"
// #include "Pattern.h"

using namespace std;

void usage(char *argv[])
{
    cout << "Usage:" << endl;
    cout << "\t" << argv[0] << " <input_folder_name> " << endl;
    cout << "\n\t"
         << "<input_folder_name>: nome da pasta de entrada" << endl;
}

bool is_valid_directory(const string &path)
{
    namespace fs = std::filesystem;
    return fs::exists(path) && fs::is_directory(path);
}

std::pair<double, double> process_file(const string &file_path)
{
    // Instantiate and load instance
    ProblemInstance *p = new ProblemInstance();
    p->load_file(file_path.c_str());


	auto start = std::chrono::high_resolution_clock::now();

    // phase to generate a semigreedy solution with data mining
    // use this solution to start the carousel

    // Semigreedy Constructive (chamar no mínimo 500x)
    // ES *EliteSet = new ES(15);

        
 //   ES EliteSet(15); 
 //   SG_Constructive *sg_constructive = new SG_Constructive;

 //   for(int i = 0; i < 200; i++){
 //       Solution temp_solution(p);
 //       double alpha = 0.90; 
 //       sg_constructive->Semigreedy_Forfeits(p, temp_solution, alpha); 
 //       delete sg_constructive; 
 //       sg_constructive = new SG_Constructive;
 //       EliteSet.add(temp_solution);  // ----------> mudar para ponteiro
 //   }
 
    ES EliteSet(15); 
    ConstructiveCG *adaptive_cg = new ConstructiveCG;
    double percentage = 0.30; 
    double max_iter = 2;
    double pct_rm = 0.05;

    for(int i = 0; i < 200; i++){
        Solution temp_solution(p);
        adaptive_cg->Carousel_Forfeits_Adaptive(p, temp_solution, max_iter, pct_rm, percentage); 
        delete adaptive_cg; 
        adaptive_cg = new ConstructiveCG;
        EliteSet.add(temp_solution);  // ----------> mudar para ponteiro
    }


    Solution mined_solution(p);  

    int randPos = 0;
    int suporte = 2; 
    //Minerador criado, mapeamento é feito, são minerados os padrões e é feito o unmap
    Mining miner(EliteSet,suporte,15);
    miner.map_file();
    miner.mine();
    miner.unmapall_file();
    //Impressão dos padrões
    // miner.printPatterns(filename);

    Pattern** Mined_Patterns = miner.getlistOfPatterns();
    int sizePatterns= miner.getSizePatterns();

    randPos = rand()%sizePatterns;
    // cout<<"Posição aleatória escolhida:"<<randPos<<endl;

    Pattern* Mined_Itens=Mined_Patterns[randPos];
    for(long unsigned int i=0; i<Mined_Itens->elements.size();i++){
        mined_solution.add_item((*Mined_Itens).elements[i]);	
    }    

    // cout << "Solution cost after mining: " << mined_solution.getCost() << endl; 

    // // Apply Carousel Greedy Constructive
    ConstructiveCG constructive;
    constructive.Carousel_Forfeits(p, mined_solution, max_iter, pct_rm);

    // cout << "Solution cost after carousel: " << mined_solution.getCost() << endl; 


    // Apply Iterated Local Search (ILS)
    int iter_wo_impr = 1000;
    // ES EliteSet(15);
    ILS ils(p, iter_wo_impr);
    // EliteSet = ils.solve(p, solution);
    pair<double, vector<int>> ils_result = ils.solve(p, mined_solution);
    double best_ils_cost = ils_result.first;
    vector<int> best_ils_solution = ils_result.second;

    double best_cost = best_ils_cost;  
    // cout << "ils solution cost: " << ils_result.first << endl; 

    // Model kpf_lb_model(p);
    // pair<Solution, double> lb_model = kpf_lb_model.Build_Model_with_LB(p, best_ils_solution); 
    // Solution best_solution = lb_model.first; 
    // double best_cost = lb_model.second; 


    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> time_taken = end - start;

    delete p;
	return {best_cost, time_taken.count()};
}

float average(std::vector<float> const& v){
    if(v.empty()){
        return 0;
    }

    auto const count = static_cast<float>(v.size());
    return std::reduce(v.begin(), v.end()) / count;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        usage(argv);
        return 1;
    }

    string directory_path = argv[1];

    if (!is_valid_directory(directory_path))
    {
        cerr << "Directory does not exist or is not a directory\n";
        return 1;
    }

    namespace fs = std::filesystem;
    for (const auto &entry : fs::directory_iterator(directory_path))
    {
        if (entry.is_regular_file())
        {   

            int NUM_EXEC = 10; 
            vector<float> costs;
            vector<float> times;

            double total_cost = 0.0;
            double total_time = 0.0;

            string filename =  entry.path().string(); 
            size_t last_slash_pos = filename.find_last_of('/');
            size_t last_dot_pos = filename.find_last_of('.');
            string i_name = filename.substr(last_slash_pos + 1, last_dot_pos - last_slash_pos - 1);

            std::ofstream outfile("results/exec_"+i_name+"_.txt");

            std::filesystem::path currentPath = std::filesystem::current_path();

            outfile << "File: " << entry.path().string() << endl;
            // Check if the file was opened successfully
            if (outfile.is_open()) {
                for (int i = 0; i < NUM_EXEC; ++i)
                {

                    cout << "Executing " << i << " for " << entry.path().string() << "..."  << endl;
                    auto [cost, time_taken] = process_file(entry.path().string());
                    costs.push_back(cost);
                    times.push_back(time_taken);
                    total_cost += cost;
                    total_time += time_taken;
                }


                outfile << "Execution Costs: ";
                for (const auto& cost : costs)
                {
                    outfile << cost << " ";
                }
                outfile << endl;

                outfile << "Execution Times: ";
                for (const auto& time : times)
                {
                    outfile << fixed << setprecision(6) << time << " ";
                }
                outfile << endl;

                // Calculate the cost_average
                auto const average_cost = average(costs);
                auto const average_time = average(times); 

                outfile << "Average Cost: " << average_cost << endl;
                outfile << "Average Elapsed Time: " << fixed << setprecision(6) << average_time << " seconds" << endl;

                auto maxElement = std::max_element(costs.begin(), costs.end());
                int index = std::distance(costs.begin(), maxElement);
                
                outfile << "Best Cost: " <<  *maxElement << endl;
                outfile << "Time: " <<  times[index] << endl; 
            
                outfile.close();
            
                std::cout << "Data successfully written to the file." << std::endl;
            } else {
                std::cerr << "Failed to open the file for writing." << std::endl;
            }
        }

    }

    return 0;
}
