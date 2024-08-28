#include "Construtivo.h"

double avg(double a, double b){
	return (a+b)/2;
}


int partition(double A[], int B[], int p,int q)
{
    double x= A[p];
    int i=p;
    int j;

    for(j=p+1; j<q; j++)
    {
        if(A[j]>x)
        {
            i=i+1;
            swap(A[i],A[j]);
			swap(B[i],B[j]);
        }

	    }

    swap(A[i],A[p]);
	swap(B[i],B[p]);
    return i;
}


void quickSort(double A[], int B[], int p,int q)
{
    int r;
    if(p<q)
    {
        r=partition(A,B,p,q);
        quickSort(A,B,p,r);  
        quickSort(A,B,r+1,q);
    }
}



Construtivo::Construtivo(ProblemInstance* _p, double _alpha)
{
	p =_p;
	alpha = _alpha;
}

Construtivo::~Construtivo()
{
	p=NULL;
	delete(p);
}

//Metodo para gerar a lista de candidatos restrita, entender essa parte
vector<int> Construtivo::GenerateRLC(vector<int> Candidate_Itens, Solution Mochila){

	vector<int> RLC;

	int* itens_ID = new int [Candidate_Itens.size()];
	double* suporte = new double [Candidate_Itens.size()];
	double trans_elem;

	vector<int> Moch = Mochila.getKS();

	for(int i=0; i< Candidate_Itens.size();++i){
		itens_ID[i] = Candidate_Itens.at(i);
		trans_elem = ((p->Value[itens_ID[i]]*(1.0)) / (p->Weight[itens_ID[i]]*(1.0)));
		suporte[i] = trans_elem;
	}

	quickSort(suporte, itens_ID, 0, Candidate_Itens.size());

	int MinBarrier = ceil(suporte[Candidate_Itens.size()-1]*alpha);

	for(int i=0; i<Candidate_Itens.size(); ++i){
		if(suporte[i]>=MinBarrier){

			RLC.push_back(itens_ID[i]);

		}
	}

	delete[] itens_ID;
	delete[] suporte;

	return RLC;
}

//Solve do construtivo sem a mineração
Solution Construtivo::solve()
{
	//Cria uma solução chamada mochila, a lista de candidatos e mais algumas variáveis necessárias
	Solution Mochila(p);
	vector<int> RLC;
	vector<int> Candidate_Itens;
	int item, Capacity, randPos;

	Capacity = p->knapsackCapacity;

	//Todos os itens são colocados na lista de items candidatos
	for(int i=0; i<p->NbItens; ++i){
		Candidate_Itens.push_back(i);
	}

	//Enquanto a capacidade for maior que 0 e a lista de candidatos tiver tamanho maior que 0
	while(Capacity>0 and Candidate_Itens.size()>0){
		//Lista de candidatos restrita é gerada, ao passar os itens candidatos e a mochila
		RLC = GenerateRLC(Candidate_Itens,Mochila);
		//Aleatoriamente escolhe um indice de uma posição
		randPos = rand()%RLC.size();
		item=RLC[randPos];
		//Se o item escolhido aleatoriamente satisfazer a restrição de não ultrapassar a capacidade,
		//Ele é adicionado na mochila e é removido da lista de candidatos, se não, somente é removido
		if(p->Weight[item]<=Capacity){
			Mochila.add_item(item);
			Capacity-=p->Weight[item];
			Candidate_Itens.erase(remove(Candidate_Itens.begin(), Candidate_Itens.end(), item), Candidate_Itens.end());
		}else if(p->Weight[item]>Capacity){
			Candidate_Itens.erase(remove(Candidate_Itens.begin(), Candidate_Itens.end(), item), Candidate_Itens.end());
		}
	}
	//Retorna a mochila
	return Mochila;
}

//Construtivo apos a mineracao, ja inicializa os padroes frequentes
Solution Construtivo::solveDM(Pattern* Mined_Itens)
{

	Solution Mochila(p);
	
	vector<int> RLC;
	vector<int> Candidate_Itens;
	int item, Capacity, randPos;

	//Adiciona na solução os padroes frequentes

	for(int i=0; i<Mined_Itens->elements.size();i++){
		Mochila.add_item((*Mined_Itens).elements[i]);
	}

	for(int i=0; i<p->NbItens; ++i){
		Candidate_Itens.push_back(i);
	}
	//Retira da lista de candidatos, os itens que já foram colocados na mochila
	for(vector<int>::iterator it = Mochila.getKS().begin(); it!=Mochila.getKS().end();++it){
		Candidate_Itens.erase(remove(Candidate_Itens.begin(), Candidate_Itens.end(), *it), Candidate_Itens.end());
	}
	
	//Atualiza a capacidade, pois ja tem alguns itens na mochila

	Capacity = p->knapsackCapacity - Mochila.getCapacity();
	
	//Mesma coisa que o construtivo normal
	while(Capacity>0 and Candidate_Itens.size()>0){
		RLC = GenerateRLC(Candidate_Itens,Mochila);
		randPos = rand()%RLC.size();
		item=RLC[randPos];
		if(p->Weight[item]<=Capacity){
			Mochila.add_item(item);
			Capacity-=p->Weight[item];
			Candidate_Itens.erase(remove(Candidate_Itens.begin(), Candidate_Itens.end(), item), Candidate_Itens.end());
		}else if(p->Weight[item]>Capacity){
			Candidate_Itens.erase(remove(Candidate_Itens.begin(), Candidate_Itens.end(), item), Candidate_Itens.end());
		}
	}

	return Mochila;
}
