#ifndef CONSTRUTIVO_H_
#define CONSTRUTIVO_H_

#include <vector>
#include <cmath>

#include "Solution.h"
#include "Pattern.h"

using namespace std;

class Construtivo
{
private:
	ProblemInstance* p;
	double alpha;
public:

	Construtivo(ProblemInstance* _p, double _alpha);

	virtual ~Construtivo();

	vector<int> GenerateRLC(vector<int> Candidate_Itens, Solution Mochila);
	Solution solve();
	Solution solveDM(Pattern* Mined_Itens);

};

#endif /*CONSTRUTIVO_H_*/
