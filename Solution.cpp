#include "Solution.h"
#include <cassert>

/*
Inicia as variáveis e gera o Hash de cada item
entrada: Instancia do problmea
saida: - 
tempo: O(n)
*/
Solution::Solution(ProblemInstance* _p){
	p =_p;	
	hash = 0 ;
    used_capacity = 0;
	num_items_in_sol = 0;
	cost = 0;
	ItemHash.resize(p->num_items);
	freq.resize(p->num_items);
	inside.resize(p->num_items);
	CostCalculated = true;

	mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

	for(int i = 0; i < p->num_items; i++){
		const long long B = uniform_int_distribution<long long> (1, (long long)1e14)(rng);
		ItemHash[i] = B;
	}
}

Solution::~Solution(){
	p=NULL;
	delete(p);
}


/*
Calcula o custo total da solução
entrada: -
saida: Lucro da solução
tempo: O(n^2)
*/
int Solution::getCost(){

	if(CostCalculated) return cost;

	cost = 0;
	vector<int> Sack;
	for(int i = 0; i < p->num_items; i++) if(inside[i]) Sack.push_back(i);

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

	CostCalculated = true;
	return cost;
}

/*
Retorna se um item está na solução
entrada: -
saida: booleado se o item está na solução
tempo: O(1)
*/
bool Solution::is_in_sack(int i){

	return inside[i];
	
}

/*
Adiciona o item na solução, atualiza o hashing, capacidade, estado, frequencia e numero de itens.
entrada: indice do item
saida: -
tempo: O(1)
*/
void Solution::add_item(int i){
		
	dq.push_back(i);
	hash += ItemHash[i];
	used_capacity += p->weights[i];
	inside[i] = 1;
	freq[i]++;
	num_items_in_sol++;
	CostCalculated = false;

	assert(used_capacity <= p->budget);

	// cout << "Adicionou " << i << "\n";

}

/*
Adiciona o item na solução, mantendo a corretude dos custos
entrada: indice do item i
saida: -
tempo: O(n)
*/
void::Solution::add_itemO(int i){
	assert(inside[i] == 0);
	dq.push_back(i);
	hash += ItemHash[i];
	used_capacity += p->weights[i];
	inside[i] = 1;
	freq[i]++;
	num_items_in_sol++;

	assert(used_capacity <= p->budget);

	cost += p->profits[i];
	for(int j = 0; j < p->num_items; j++) 
		if(is_in_sack(j))
			cost -= p->mD[i][j];

}

/*
Remove o item mais velho da mochila
entrada: -
saida: indicie do item removido
tempo: O(~1)
*/
int::Solution::remove_oldest_choice(){

	while(!dq.empty() && (!inside[dq.front()] || freq[dq.front()] > 1)){
		freq[dq.front()]--;
		dq.pop_front();
	}

	if(dq.empty()) return -1;

	int i = dq.front();
	remove_item(i);

	// cout << "Removeu da frente " << i << "\n";

	return i;
}

/*
Remove o item mais novo da mochila
entrada: -
saida: indicie do item removido
tempo: O(1) amortizado
*/
int::Solution::remove_newest_choice(){

	while(!dq.empty() && (!inside[dq.back()])){
		freq[dq.back()]--;
		dq.pop_back();
	}

	if(dq.empty()) return -1;

	int i = dq.back();
	remove_item(i);

	// cout << "Removeu de tras " << i << "\n";

	return i;

}

/*
Retorna o numero de itens na mochila
entrada: -
saida: número de itens na mochila
tempo: O(1)
*/
int::Solution::get_size(){
	return num_items_in_sol;
}

/*
Remove um item da soluçã0
entrada: indicie do item a ser removido
saida: -
tempo: O(1)
*/
void Solution::remove_item(int i){
	
	hash -= ItemHash[i];
	used_capacity -= p->weights[i];
	inside[i] = 0;
	num_items_in_sol--;
	CostCalculated = false;

}

/*
Remove um item da solução, mantendo a corretude do custo
entrada: indicie do item a ser removido
saida: -
tempo: O(n)
*/
void Solution::remove_itemO(int i){


	hash -= ItemHash[i];
	used_capacity -= p->weights[i];
	inside[i] = 0;
	num_items_in_sol--;

	cost -= p->profits[i];
	for(int j = 0; j < p->num_items; j++)
		if(is_in_sack(j))
			cost += p->mD[i][j];
}

/*
Compara duas soluçãoes
entrada: solução 'a' e solução 'b´
saida: booleano dizendo se são iguais
tempo: O(1)
*/
bool operator== (Solution &a, Solution &b){

	return a.hash == b.hash;

}

/*
Compara duas soluçãoes
entrada: solução 'a' e solução 'b´
saida: booleano dizendo se é menor
tempo: O(n^2)
*/
bool operator< (Solution &a, Solution &b){
	return a.getCost() < b.getCost();
}

/*
Retorna vetor de booleano dos itens na solução
entrada: -
saida: vetor de booleano
tempo: O(1)
*/
vector<bool>  Solution::convertToBinaryVector() {
    return inside;
}

bool::Solution::can_add(int i){
	if(inside[i]) return false;
	if(p->weights[i] + used_capacity > p->budget) return false;
	return true;

}

bool::Solution::CheckSol(){
	int peso  = 0;
	for(int i = 0; i < p->num_items; i++){
		if(is_in_sack(i)) peso += p->weights[i];
	}
	assert(peso == used_capacity);
	return peso <= p->budget;
}

int::Solution::CountForfeit(){
	int count = 0;
  	for(auto pr : p->forfeits_pairs){
    	count += (is_in_sack(pr.first) && is_in_sack(pr.second));
  	}
	return count;
}

vector<int> Solution::getKS(){
	vector<int> MORTE;
	for(int i = 0; i < p->num_items; i++){
		if(inside[i]) MORTE.push_back(i);
	}
	return MORTE;
}

ostream& operator<<(ostream& strm, Solution& s){
	auto Sack = s.getKS();
	for(vector<int>::iterator it = Sack.begin(); it!=Sack.end(); ++it){
			strm<<*it<<" ";
	}

	return strm;
}

void Solution::updateCapacity(){
	CostCalculated = false;
	
}

void Solution::clear(){
	
	CostCalculated = true;
	hash = 0;
    used_capacity = 0;
	num_items_in_sol = 0;
	cost = 0;
	
	fill(freq.begin(), freq.end(), 0);
	fill(inside.begin(), inside.end(), 0);
	dq.clear();

}

void::Solution::remove_oldest_choice_adaptive(double percentage)
{
    // Step 1: Calculate the number of elements to select based on percentage
    int numElementsToSelect = (get_size() * percentage);

    // Step 2: Ensure numElementsToSelect is within bounds of Sack size
    numElementsToSelect = min(numElementsToSelect, get_size());

    // Step 3: Select the first numElementsToSelect elements from Sack
	//Subset = (0, numElementsToSelect(

    // Step 4: Choose a random element from the subset
    if (numElementsToSelect != 0) {
		
		// Random index within subset
		int randomIndex = rand() % numElementsToSelect; //trocar?

		while(!dq.empty() && (!inside[dq.front()] || freq[dq.front()] > 1)){
			freq[dq.front()]--;
			dq.pop_front();
		}

		int curIndex = 0;
		auto it = dq.begin();
		while(curIndex != randomIndex){
			auto toRemove = next(it);
			while(!dq.empty() && (!inside[*toRemove] || freq[*toRemove] > 1)){
				freq[*toRemove]--;
				dq.erase(toRemove);
				toRemove = next(it);
			}
			curIndex++;
			it = next(it);
		}

		int randomElement = *it;
		remove_item(randomElement);
    }

}