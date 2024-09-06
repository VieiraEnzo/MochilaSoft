#include "Solution.h"

Solution::Solution(ProblemInstance* _p){
	p =_p;	
	hash = 0 ;
    used_capacity = 0;
}

Solution::~Solution(){
	Sack.clear();
	p=NULL;
	delete(p);
}

double Solution::getCost(){
	cost = 0;
	for (int item: Sack){
        cost = cost + p->profits[item];
    }

	for(int item_i: Sack){
		for(int item_j: Sack){
			if(item_i < item_j){
				cost = cost - p->mD[item_i][item_j]; 
			}
		}
	}

	return cost;
}

bool Solution::is_in_sack(int i){

	for(int item: Sack){
		if(item == i){
			return true; 
		}
	}
	return false; 
	
}

void Solution::add_item(int i){
	
	Sack.push_back(i);

	hash += (i*p->profits[i] + (i+1)*p->weights[i]);

	used_capacity+= p->weights[i];

	// cout << "Adicionou " << i << "\n";

}

void::Solution::remove_oldest_choice(){
	// cout << "Removeu da frente " << *Sack.begin() << "\n";
	Sack.erase(Sack.begin());
}


void::Solution::remove_oldest_choice_adaptive(double percentage)
{	

    // Step 1: Calculate the number of elements to select based on percentage
    int numElementsToSelect = static_cast<int>(Sack.size() * percentage);

    // Step 2: Ensure numElementsToSelect is within bounds of Sack size
    numElementsToSelect = std::min(numElementsToSelect, static_cast<int>(Sack.size()));

    // Step 3: Select the first numElementsToSelect elements from Sack
    std::vector<int> subset(Sack.begin(), Sack.begin() + numElementsToSelect);

    // Step 4: Choose a random element from the subset
    if (!subset.empty()) {
        int randomIndex = rand() % subset.size(); // Random index within subset
        int randomElement = subset[randomIndex];

        // std::cout << "Randomly selected element: " << randomElement << std::endl;
            
        // Remove the randomly selected element from the Sack
        auto it = std::find(Sack.begin(), Sack.end(), randomElement);
        if (it != Sack.end()) {
            Sack.erase(it);
        }
    }

}

void Solution::remove_item(int i){

	hash -= (i*p->profits[i] + (i+1)*p->weights[i]);
	Sack.erase(remove(Sack.begin(), Sack.end(), i), Sack.end());
	//sort (Sack.begin(), Sack.end());

    used_capacity-= p->weights[i];

	// cout << "Removeu do meio" << i << "\n";
}


vector<int>& Solution::getKS(){
	return Sack;
}

bool operator== (Solution &a, Solution &b){
	if (a.cost != b.cost) return false;
	if(a.Sack.size() != b.Sack.size()) return false;
	if(a.hash != b.hash) return false;
	vector<int> lista=a.Sack;
	vector<int> lista2=b.Sack;
	

	bool control;

	//cout<<a.Sack.size()<<" "<<a.hash<<endl;
	//cout<<b.Sack.size()<<" "<<b.hash<<endl;

	for(vector<int>::iterator it = lista.begin(); it!=lista.end(); ++it){
		control = false;
		for(vector<int>::iterator it2 = lista2.begin(); it2 != lista2.end(); ++it2){
			if(*it == *it2){
				control = true;
				break;
			}
		}
		if(control == false)break; 
	}


	return control;
}

bool operator< (Solution &a, Solution &b){
	//cout<<"Testa Menor"<<endl;

	 if (a.cost < b.cost)
		 return true;

	 return false;
}

bool operator> (Solution &a, Solution &b){
	//cout<<"Testa Maior"<<endl;
	 if (a.cost > b.cost)
		 return true;

	 return false;
}

ostream& operator<<(ostream& strm, Solution& s){
	for(vector<int>::iterator it = s.Sack.begin(); it!=s.Sack.end(); ++it){
			strm<<*it<<" ";
	}

	return strm;
}

vector<int>  Solution::convertToBinaryVector() {
    vector<int> binaryVec(p->num_items, 0); 

    for (int item: Sack) {
        if (item >= 0 && item < p->num_items) {
            binaryVec[item] = 1; 
        }
    }

    return binaryVec;
}


// check if solution has repeated items


// void Solution::PrintSolution(const char *filename, const char* type)
// {

// 	std::string instanceName = filename;
// 	std::string str2 = instanceName.substr (8);  
// 	std::string delimiter = ".txt";
// 	std::string token = str2.substr(0, str2.find(delimiter));   

// 	vector<int> Solu = getKS();

// }

