#ifndef ES_CPP_
#define ES_CPP_

#include "ES.h"


/*
- Precisa usar double no Cost????????
- É a mesa comparação a inversão
*/
struct compare_sol {
    bool operator()(Solution a, Solution b) const {
    	    	if(a.getCost()-b.getCost()<1E-5 || b.getCost()-a.getCost()>-1E-5)return false; 

		return a.getCost()>b.getCost();
    }
};


ES::ES(size_t _MaxSize){
	MaxSize = _MaxSize;
}

ES::~ES(){

}

size_t ES::getESsize(){
	return HeapSol.size();
}

bool ES::add(Solution s){
	// cout<<"inserindo no ES ...."<<endl;

	for(size_t i=0; i<HeapSol.size(); ++i){
		if(HeapSol[i] == s)
			return false;
		//cout << sol<<endl;
	}

	// cout<<"nao achou igual, vou tentar inserir"<<endl;

	//if(find(HeapSol.begin(),HeapSol.end(),*s) != HeapSol.end())
	//	return false;

	Solution* WorstSol;
	// cout<<"adding: "<<*s<<endl;
	if(HeapSol.size() == 0){
		WorstSol = NULL;
	}else{
		WorstSol = &HeapSol.front();
	}

	/*
		////- Como sabe que o WorstSoltion está np .front(), heap nn é uma estrutura ordenada.
		- Qual é  a do HealpSOl.pop_back() e push_back()
		- Voce que fez o Elite Set? Pode muda? 
		- Oque voce quer que o Elite Set fassa
	*/
	if(HeapSol.size() < MaxSize ){

		HeapSol.push_back(s);
		push_heap (HeapSol.begin(),HeapSol.end(),compare_sol());

		return true;
	}else if(s.getCost() > WorstSol->getCost()){

		pop_heap (HeapSol.begin(),HeapSol.end(),compare_sol());
		HeapSol.pop_back(); //????????????

		HeapSol.push_back(s);
		push_heap (HeapSol.begin(),HeapSol.end(),compare_sol());

		return true;
	}
		
	return false;
}

vector<Solution> ES::getConjSol(){
	return HeapSol;
}


ostream& operator<<(ostream &strm, ES es){

	for(size_t i=0; i<es.HeapSol.size(); ++i){
		strm<<es.HeapSol.at(i)<<endl;
		//cout << sol<<endl;
	}
	return strm;
}


#endif // ES_CPP_

