#ifndef GRASP_CPP_
#define GRASP_CPP_

#include "GRASP.h"

GRASP::GRASP(ProblemInstance* _p, int _MaxIter, double _alpha)
{
	p = _p;
	MaxIter = _MaxIter;
	alpha = _alpha;

}

GRASP::~GRASP()
{

}

Solution GRASP::solve()
{

	FWChrono Tempo;
	
	Tempo.start();

	//Etapa de construção da solução
	Construtivo C_GRASP(p,alpha);
	//Etapa de busca local
	BuscaLocal BL_GRASP(p);
	//Variável que armazena melhor solução
	Solution s_best(p);
	//Variável que armazena a solução atual
	Solution s(p);
	//Roda o construtivo pela primeira vez
	s=C_GRASP.solve();
	//Roda a busca local pela primeira vez
	s=BL_GRASP.solve(s);
	//A melhor solução é esta, pos é a primeira
	s_best = s;


	for(int i=1; i<MaxIter; ++i){
		//Se o tempo executando for maior que 3600 dá um break
		if (Tempo.span() > 3600) break;
		//Constroi a solução
	 	s=C_GRASP.solve();
		//MAtualiza a solução
		s_best=UpdateBest(s,s_best);
	 
 	}

	Tempo.stop();
	//Retorna a melhor solução
	return s_best;
}

Solution GRASP::UpdateBest(Solution s, Solution s_best){
	if(s.getCost()>s_best.getCost()){
		return s;
	}

	return s_best;
}

void GRASP::Relatorio(Solution* s){
	cout<<"-------------------GRASP-------------------"<<endl;
	cout<<"Value: "<<s->getCost()<<endl;
	cout<<"Used Capacity: "<<s->getCapacity()<<endl;
	cout<<(*s)<<endl;
	cout<<"-------------------------------------------"<<endl;
}

#endif /*GRASP_CPP_*/		
