#ifndef BUSCALOCAL_H_
#define BUSCALOCAL_H_
#include <stdio.h>
#include <vector>
#include "Solution.h"

using namespace std;

class BuscaLocal
{
private:
	ProblemInstance* p;
	vector<int> Candidate_Itens;
	vector<int> Current_Itens;

public:

	BuscaLocal(ProblemInstance* _p);

	virtual ~BuscaLocal();

	Solution solve(Solution s);

	void swap(Solution* Mochila,int i, int j);
};

#endif /*BUSCALOCAL_H_*/
