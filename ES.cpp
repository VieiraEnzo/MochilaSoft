#ifndef ES_CPP_
#define ES_CPP_

#include "ES.h"



ES::ES(size_t _MaxSize){
	MaxSize = _MaxSize;
}

ES::~ES(){

}

size_t ES::getESsize(){
	return Sol.size();
}

bool ES::add(Solution s){

	for(size_t i=0; i<Sol.size(); ++i){
		if(Sol[i] == s) return false;
	}

	Sol.push_back(s);
	return true;
}

vector<Solution> ES::getConjSol(){
	return Sol;
}


ostream& operator<<(ostream &strm, ES es){

	for(size_t i=0; i<es.Sol.size(); ++i){
		strm<<es.Sol.at(i)<<endl;
		//cout << sol<<endl;
	}
	return strm;
}


#endif // ES_CPP_

