#ifndef GRASP_H_
#define GRASP_H_

#include "Construtivo.h"
#include "BuscaLocal.h"
#include "FWChrono.h"

using namespace std;

class GRASP
{
private:
	ProblemInstance* p;
	int MaxIter;
	double alpha;

public:

	GRASP(ProblemInstance* _p, int MaxIter, double _alpha);

	virtual ~GRASP();

	Solution solve();

	Solution UpdateBest(Solution s, Solution s_best);
	void Relatorio(Solution* s);
};

#endif /*GRASP_H_*/
