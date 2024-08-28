#ifndef GRASP_DM_CPP_
#define GRASP_DM_CPP_

#include "GRASP_DM.h"

GRASP_DM::GRASP_DM(ProblemInstance* _p, int _MaxIter, double _alpha, int sup)
{
	p = _p;
	suporte = sup;
	MaxIter = _MaxIter;
	alpha = _alpha;

}

GRASP_DM::~GRASP_DM()
{

}

Solution GRASP_DM::solve(const char *filename)
{

	cout<<"Running GRASP_DM...";
	//Conjunto elite que armazenará 10 soluções
	ES EliteSet(10);

	//Construtivo e busca local são criados
	Construtivo C_GRASP_DM(p,alpha);
	BuscaLocal BL_GRASP_DM(p);

	//p->PrintInstance(); 129 capacidade

	//Melhor solução e solução s são criadas
	Solution s_best(p);
	Solution s(p);

	//Arquivo para escrevermos os resultados chamado results.txt

	std::string instanceName = filename;
	std::string str2 = instanceName.substr (8);  
	std::string delimiter = ".txt";
	std::string token = str2.substr(0, str2.find(delimiter));

	ofstream file;
	
	file.open ("../analysis/allResults/"+token+"/"+token+"_results.txt");

	//Constroi a primeira solução e roda a busca local, os resultados são escritos em um arquivo
	//cout<<"------------- Primeira Parte GRASP_DM ----------------"<<endl;
	s=C_GRASP_DM.solve();
	//cout<<"Construção solução: "<<endl;
	//cout<<s.getCost()<<" ";
	//cout<<s.getCapacity()<<" ";
	file << s.getCost()<< " ";
	s=BL_GRASP_DM.solve(s);
	s_best = s;
	//cout<<s_best.getCost()<<endl;
	file << s_best.getCost()<< " ";
	file << "\n";
	

	//Metade das iterações do GRASP são rodadas aqui
	for(int i=1; i<ceil(MaxIter/2); ++i){
		s=C_GRASP_DM.solve();
		//cout<<s.getCost()<<" ";
		file << s.getCost()<< " ";
		
		s=BL_GRASP_DM.solve(s);
		//cout<<s.getCost()<<endl;
		file << s_best.getCost()<< " ";
		file << "\n";
		EliteSet.add(s);
		//cout<<s.getCapacity()<<" ";
		s_best=UpdateBest(s,s_best);
		//cout<<s_best.getCost()<<" ";
		//cout<<s_best.getCapacity()<<" ";
 	}

	//cout<<"-------------Conjunto elite---------------"<<endl;

	//cout<<EliteSet<<endl;

	//cout<<"-------------Mineração----------------"<<endl;

	int randPos = 0;
	//Minerador criado, mapeamento é feito, são minerados os padrões e é feito o unmap
	Mining miner(EliteSet,suporte,5);
	miner.map_file();
	miner.mine();
	miner.unmapall_file();
	//Impressão dos padrões
	miner.printPatterns(filename);

	Pattern** Mined_Patterns = miner.getlistOfPatterns();
	int sizePatterns= miner.getSizePatterns();

	//randPos = rand()%sizePatterns;
	//cout<<"Posição aleatória escolhida:"<<randPos<<endl;

	//cout<<"------------- Segunda Parte GRASP_DM ----------------"<<endl;
	
	//Outra metade das iterações do GRASP, já com os padrões encontrados na mineração
	for(int i=ceil(MaxIter/2)+1; i<MaxIter; ++i){
		randPos = rand() % sizePatterns;
		s=C_GRASP_DM.solveDM(Mined_Patterns[randPos]);
		//cout<<s.getCost()<<" ";
		file << s.getCost()<< " ";
		s=BL_GRASP_DM.solve(s);
		//cout<<s.getCost()<<endl;
		file << s.getCost()<< " ";
		file << "\n";
		s_best=UpdateBest(s,s_best);
 	}
	
	file.close();
	
	cout<<"GRASP_DM Runned."<<endl;

	return s_best;
}

Solution GRASP_DM::UpdateBest(Solution s, Solution s_best){
	if(s.getCost()>s_best.getCost()){
		return s;
	}

	return s_best;
}

void GRASP_DM::Relatorio(Solution* s, const char *filename){
	
	std::string instanceName = filename;
	std::string str2 = instanceName.substr (8);  
	std::string delimiter = ".txt";
	std::string token = str2.substr(0, str2.find(delimiter));

	ofstream outfile;

	outfile.open("../analysis/allResults/"+token+"/"+token+"_relatorio.txt", std::ios_base::app); // append instead of overwrite

	//cout<<"-----------------GRASP-DM------------------"<<endl;
	outfile<<"-----------------GRASP-DM------------------"<<endl;
	//cout<<"Value: "<<s->getCost()<<endl;
	outfile<<"Value: "<<s->getCost()<<endl;
	//cout<<"Used Capacity: "<<s->getCapacity()<<endl;
	outfile<<"Used Capacity: "<<s->getCapacity()<<endl;
	//cout<<(*s)<<endl;
	//cout<<"-------------------------------------------"<<endl;
	outfile<<"-------------------------------------------"<<endl;
}

#endif /*GRASP_DM_CPP_*/		
