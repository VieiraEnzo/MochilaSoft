#include "ProblemInstance.h"
#include <cassert>

void ProblemInstance::load_file(const char *const f)
{
	ifstream file(f);
	if (!file.is_open())
	{
		cout << "ERROR: File " << f << " not found!" << endl;
		exit(0);
	}

	filename = f; 
	// Find the position of the last slash
    size_t last_slash_pos = filename.find_last_of('/');
    // Find the position of the last dot
    size_t last_dot_pos = filename.find_last_of('.');
    // Extract the substring between the last slash and the last dot
    i_name = filename.substr(last_slash_pos + 1, last_dot_pos - last_slash_pos - 1);

	num_items = 0;
	num_forfeits_pairs = 0; 
	budget = 0; 

	//read basic info about kpf
	file >> num_items;
	file >> num_forfeits_pairs;
	file >> budget;
	
    items = new int[num_items]; 
	profits = new double [num_items];
	weights = new double[num_items];

	forfeits_costs = new double[num_forfeits_pairs]; 
	
	mD = new double*[num_items];
    for (int i = 0; i < num_items; ++i) {
        mD[i] = new double[num_items]();
    }

	//Initiate mD with 0
	for(int i = 0; i < num_items; i++) 
		for(int j = 0; j < num_items; j++) 
			mD[i][j] = 0;


	// save items
	for (int i = 0; i < num_items; i++){
		items[i] = i;
	}
    
	// read profits
    for (int i = 0; i < num_items; ++i)
	{
		file >> profits[i];
	}

	// read weights
	for (int i = 0; i < num_items; ++i)
	{
		file >> weights[i];
	}

	// read forfeits pairs and costs
	int num1 = 0 , num2 = 0, pair1 = 0, pair2  = 0; 
	for(int i = 0; i < num_forfeits_pairs; ++i){
		file >> num1 >> forfeits_costs[i] >> num2;

		file >> pair1 >> pair2;

		// symetric matrix
        mD[pair1][pair2] = forfeits_costs[i];
        mD[pair2][pair1] = forfeits_costs[i];

		assert(pair1 != pair2);

		forfeits_pairs.push_back(make_pair(pair1, pair2)); 

	}

	file.close();
}

void ProblemInstance::PrintInstance()
{
	cout << "Instance starts here"<<endl;

	cout << num_items << " " << num_forfeits_pairs << " " << budget; 
    cout << endl; 

	// check items
	cout << "knapsack items: " << endl; 
    for (int i = 0; i < num_items; i++){
		cout << items[i] << " "; 
	}
    cout << endl; 

	// check profits
	cout << "knapsack profits: " << endl; 
    for (int i = 0; i < num_items; i++){
		cout << profits[i] << " "; 
	}
    cout << endl; 

	// check weights
	cout << "knapsack weights: " << endl; 
    for (int i = 0; i < num_items; i++){
		cout << weights[i] << " "; 
	}
    cout << endl; 

	// check forfeits cost pairs matrix
	cout << "knapsack forfeits symmetric matrix: " << endl; 
    for (int i = 0; i < num_items; i++) {
		for(int j = 0; j < num_items; j++){

        	std::cout << mD[i][j] << " ";
		}
		cout << endl; 
    }
	cout << endl;

	// check forfeits costs
	cout << "knapsack forfeits costs: " << endl; 
    for (int i = 0; i < num_forfeits_pairs; i++){
		cout << forfeits_costs[i] << " "; 
	}
    cout << endl; 

	cout << "forfeits pairs: " << endl; 
	for (const auto& pair : forfeits_pairs) {
        std::cout << "(" << pair.first << ", " << pair.second << ")" << "--";
    }
	cout << endl; 

	cout<<"Instance ends here" << endl;
}

ProblemInstance::~ProblemInstance()
{
	// cout << "Deleting instance...";
	delete[] forfeits_costs; 
	delete[] items; 
	delete[] profits; 
	delete[] weights; 

	for (int i = 0; i < num_items; ++i) {
        delete[] mD[i];
    }
    delete[] mD;
	// cout << "Instance deleted. \n";
}