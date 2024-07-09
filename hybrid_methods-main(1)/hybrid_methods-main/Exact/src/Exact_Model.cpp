#include <iostream>
#include <filesystem>
#include <iomanip>
#include <chrono>
#include <algorithm> // For std::max_element
#include <iterator>  // For std::distance
#include "ProblemInstance.h"
#include "Model.h"

// cmd: g++ Model.cpp -o model_test -I/opt/ibm/ILOG/CPLEX_Studio2211/cplex/include -I/opt/ibm/ILOG/CPLEX_Studio2211/concert/include -L/opt/ibm/ILOG/CPLEX_Studio2211/cplex/lib/x86-64_linux/static_pic_linux/static_pic

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

double process_file(const string &file_path)
{
    // Instantiate and load instance
    ProblemInstance *p = new ProblemInstance();
    p->load_file(file_path.c_str());

	auto start = std::chrono::high_resolution_clock::now();

    Model kpf_model(p);
    kpf_model.Build_Model(p);  

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> time_taken = end - start;

    delete p;
	return time_taken.count();
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

            string filename =  entry.path().string(); 
            size_t last_slash_pos = filename.find_last_of('/');
            size_t last_dot_pos = filename.find_last_of('.');
            string i_name = filename.substr(last_slash_pos + 1, last_dot_pos - last_slash_pos - 1);

            cout << "Executing " << i_name << " for " << entry.path().string() << "..."  << endl;
            double time_mdl = process_file(entry.path().string());
            cout << "Time taken: " << time_mdl << endl; 
        }

    }

    return 0;
}
