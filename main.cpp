#include <bits/stdc++.h>
#include "ProblemInstance.h"
#include "Solution.h"
#include "Constructive.h"
#include "ConstructiveCG.h"
#include "ILS.h"
// #include "Model.h"
// #include "Mining.h"
// #include "Pattern.h"

using namespace std;
/*
instancias:

*/
//custo esperado instancia 8 da O: 2471
//solucao otima: 2556
//tempo: 1,28 segundos
//flag O3
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

std::vector<double> process_file(const string &file_path)
{
    // Instantiate and load instance
    ProblemInstance *p = new ProblemInstance();
    p->load_file(file_path.c_str());

    Solution solution(p);

	auto start = std::chrono::high_resolution_clock::now();

    // Apply Carousel Greedy Constructive
    ConstructiveCG constructive;
    double max_iter = 2;
    double pct_rm = 0.05;
    constructive.Carousel_Forfeits(p, solution, max_iter, pct_rm);

    // cout << solution.getCost() << " ";

    int iter_wo_impr = 1000;
    // ES EliteSet(15);
    ILS ils(p, iter_wo_impr);
    // EliteSet = ils.solve(p, solution);
    int best_cost = ils.solve(p, solution);

    cout << best_cost << "\n";

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> time_taken = end - start;
    
    assert(solution.CheckSol() == 1);

    // delete p; TA DANDO SEG FAULT
	return {(double)best_cost, time_taken.count(), (double)solution.CountForfeit()};
}

float average(std::vector<float> const& v){
    if(v.empty()){
        return 0;
    }

    auto const count = static_cast<float>(v.size());
    return std::reduce(v.begin(), v.end()) / count;
}

int main(int argc, char *argv[]){
    if (argc != 2){
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

            string cp = directory_path.substr(directory_path.find_first_of('/'), directory_path.find_last_of('/') - directory_path.find_first_of('/'));
            std::ofstream outfile("results" + cp + "/exec_"+i_name+"_.txt");

            std::filesystem::path currentPath = std::filesystem::current_path();

            outfile << "File: " << entry.path().string() << endl;
            // Check if the file was opened successfully
            if (outfile.is_open()) {
                for (int i = 0; i < NUM_EXEC; ++i)
                {

                    cout << "Executing " << i << " for " << entry.path().string() << "..."  << endl;
                    vector<double> a =  process_file(entry.path().string());
                    double cost = a[0], time_taken = a[1], forfeitPairs = a[2]; 
                    costs.push_back(cost);
                    times.push_back(time_taken);
                    total_cost += cost;
                    total_time += time_taken;
                    // outfile << "Number of Forfeit Pairs = " << forfeitPairs << endl;

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
                cout << "results" + cp + "/exec_"+i_name+"_.txt" << "\n";
            }
        }

    }

    return 0;
}