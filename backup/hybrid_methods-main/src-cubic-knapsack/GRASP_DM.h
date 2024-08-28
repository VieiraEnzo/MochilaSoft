#ifndef GRASP_DM_H_
#define GRASP_DM_H_

#include "Construtivo.h"
#include "BuscaLocal.h"
#include "ES.h"
#include "Mining.h"
#include "Pattern.h"
#include <math.h> 
#include <iostream>
#include <fstream>

using namespace std;

class GRASP_DM
{
private:
	ProblemInstance* p;
	int MaxIter;
	int suporte;
	double alpha;

public:

	GRASP_DM(ProblemInstance* _p, int MaxIter, double _alpha, int sup);

	virtual ~GRASP_DM();

	Solution solve(const char *filename);

	Solution UpdateBest(Solution s, Solution s_best);
	void Relatorio(Solution* s, const char *filename);
};

#endif /*GRASP_H_*/
