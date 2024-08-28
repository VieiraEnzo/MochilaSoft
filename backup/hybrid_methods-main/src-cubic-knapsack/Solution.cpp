#include "Solution.h"

Solution::Solution(ProblemInstance* _p){
	p =_p;	
	cost = 0;
	hash = 0 ;
    used_capacity = 0;
}

Solution::~Solution(){
	Sack.clear();
	p=NULL;
	delete(p);
}

double Solution::getCost(){
	return cost;
}

void Solution::add_item(int i){
	
	double sum_ind = p->Value[i];

	hash += (i*p->Value[i] + (i+1)*p->Weight[i]);

	Sack.push_back(i);
	sort (Sack.begin(), Sack.end());

	cost += (sum_ind);
    used_capacity+= p->Weight[i];
}

void Solution::remove_item(int i){
	double sum_ind = p->Value[i];
	
	hash -= (i*p->Value[i] + (i+1)*p->Weight[i]);
	Sack.erase(remove(Sack.begin(), Sack.end(), i), Sack.end());
	//sort (Sack.begin(), Sack.end());

	cost -= sum_ind;
    used_capacity-= p->Weight[i];
}


vector<int>& Solution::getKS(){
	return Sack;
}

ostream& operator<<(ostream& strm, Solution& s){
	for(vector<int>::iterator it = s.Sack.begin(); it!=s.Sack.end(); ++it){
			strm<<*it<<" ";
	}

	return strm;
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

void Solution::PrintSolution(const char *filename, const char* type)
{

	std::string instanceName = filename;
	std::string str2 = instanceName.substr (8);  
	std::string delimiter = ".txt";
	std::string token = str2.substr(0, str2.find(delimiter));   

	ofstream Solution_DM;

	if(type == "DM_GRASP"){
		cout << "Writing DM_GRASP solution into file... ";
		Solution_DM.open ("../analysis/allResults/"+token+"/"+token+"_solution_DM_GRASP.txt");
	}else{
		cout << "Writing exact solution into file... ";
		Solution_DM.open ("../analysis/allResults/"+token+"/"+token+"_solution_EXACT.txt");
	}

	vector<int> Solu = getKS();
	
	for(vector<int>::iterator it=Solu.begin(); it!=Solu.end(); it++){
		//cout<< *it <<" ";
		Solution_DM<< *it <<" ";
	}

	Solution_DM.close();

	if(type == "DM_GRASP"){
		cout<<"Solution written in file!"<<endl;
		cout<<endl;
	}else{
		cout<<"Solution written in file!"<<endl;
	}
}

