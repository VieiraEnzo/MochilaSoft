#ifndef ES_CPP_
#define ES_CPP_

#include "ES.h"

ES::ES(size_t _MaxSize){
	MaxSize = _MaxSize;
}

ES::~ES(){

}

size_t ES::getESsize(){
	return HeapSol.size();
}

bool ES::add(Solution s){

	if(HeapSol.find(s) != HeapSol.end()) return false;
	
	Solution Worst = *HeapSol.begin();

	if(HeapSol.size() < MaxSize ){

		HeapSol.insert(s);
		return true;

	}else if(s.getCost() > Worst.getCost()){

		HeapSol.erase(HeapSol.begin());
		HeapSol.insert(s);

		return true;
	}
		
	return false;
}

vector<Solution> ES::getConjSol(){
	vector<Solution> conj;
	for(auto s : HeapSol) conj.push_back(s);
	return conj;
}


ostream& operator<<(ostream &strm, ES es){

	for(auto s : es.HeapSol){
		strm<< s <<endl;
	}
	return strm;
}


#endif

