#ifndef ES_H_
#define ES_H_

#include <set>
#include <vector>
#include <iostream>
#include <algorithm>
#include "Solution.h"

using namespace std;


class ES
{
public:
	
	set<Solution> HeapSol;
	size_t MaxSize;

	friend ostream& operator<<(ostream&, ES);

public:

	ES(size_t MaxSize);

	~ES();

	size_t getESsize();

	bool add(Solution s);

	vector<Solution> getConjSol();

	void PrintES();


};


#endif //ES_H_
