#include "BuscaLocal.h"

BuscaLocal::BuscaLocal(ProblemInstance* _p)
{
	p = _p;
}

BuscaLocal::~BuscaLocal()
{

}

Solution BuscaLocal::solve(Solution s)
{	
	Solution Mochila = s;

	int in, out;
	double BestImprovement, Capacity, Cost;
	bool Improved = true;


	while(Improved){
		//Criando as estruturas basicas para fazer a busca local
		
		Improved = false;
				
		Cost = Mochila.getCost();
		BestImprovement = Cost;
		Capacity = p->knapsackCapacity - Mochila.getCapacity();

		//cout<<Capacity<<endl;

		Candidate_Itens.clear();
		Current_Itens.clear();

		for(int i=0; i<p->NbItens; ++i){
			Candidate_Itens.push_back(i);
		}
		
		vector<int> Moch = Mochila.getKS();

		for(vector<int>::iterator it=Moch.begin(); it!=Moch.end(); it++){
			Current_Itens.push_back(*it);
			Candidate_Itens.erase(remove(Candidate_Itens.begin(), Candidate_Itens.end(), *it), Candidate_Itens.end());
		}
		// sort current and candidate
		for (int i = 0; i < Current_Itens.size(); i++)
		{
			for (int j = 0; j < Candidate_Itens.size(); j++)
			{

				if(p->knapsackCapacity - p->Weight[Current_Itens.at(i)] + p->Weight[Candidate_Itens.at(j)] <= p->knapsackCapacity){

					if (BestImprovement < Cost + p->Value[Candidate_Itens.at(j)] - p->Value[Current_Itens.at(i)])
					{
						BestImprovement = Cost + p->Value[Candidate_Itens.at(j)] - p->Value[Current_Itens.at(i)];
						in = Candidate_Itens.at(j);
						out = Current_Itens.at(i);
						Improved = true;
					}
				}
			}
		}

		//Realizando a troca
		if(Improved){
			swap(&Mochila,in,out);
		}
	}

	return Mochila;
}

void BuscaLocal::swap(Solution* Mochila,int i, int j){
	Mochila->remove_item(j);
	Mochila->add_item(i);

	//cout<<*Mochila<<endl;
	//getchar();
}
