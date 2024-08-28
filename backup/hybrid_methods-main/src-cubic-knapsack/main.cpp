#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string>
#include <sstream>
#include <string>
#include <limits>

#include "ProblemInstance.h"
#include "Solution.h"
#include "Exact.h"
#include "FWChrono.h"
#include "GRASP.h"
#include "GRASP_DM.h"
#include <filesystem>
using namespace std;

void usage(char *argv[])
{
	cout << "Usage:" << endl;
	cout << "\t" << argv[0] << " <input_instance_name> <alpha> <MaxIterGrasp> <SupMin>" << endl;
	cout << "\n\t"
		 << "<input_instance_name>: nome do arquivo de entrada" << endl;
	cout << "\n\t"
		 << "<alpha>: alpha construtivo" << endl;
	cout << "\n\t"
		 << "<MaxIterGrasp>: numero de iteracoes GRASP" << endl;
	cout << "\n\t"
		 << "<SupMin>: Suporte Minimo" << endl;
}

int main(int argc, char *argv[])
{
	if (argc != 5)
	{
		usage(argv);
	}
	else
	{

		const char *filename = argv[1];
		int alpha = argc > 2 ? atof(argv[2]) : atof("0.7");
		int MaxIterGrasp = argc > 3 ? atoi(argv[3]) : atoi("700");
		int suporte = argc > 4 ? atoi(argv[4]) : atoi("5");
    	
		//Criação e carregamento de Instância
		ProblemInstance *p = new ProblemInstance();
		p->load_file(filename);

		//Solução através do algoritmo exato
	
		Solution s_exact(p);

		Exact * modelo = new Exact(p);
		s_exact = modelo->solve(3600, filename);

		s_exact.PrintSolution(filename, "exact");

		//Criação da Instância da melhor solução
		Solution s_best(p);
		
		//Cria o GRASP que roda a mineração de dados no meio
		GRASP_DM MH(p,MaxIterGrasp,alpha, suporte);

		//A variável da melhor solução recebe o resultado do GRASP_DM
		s_best = MH.solve(filename);

		s_best.PrintSolution(filename, "DM_GRASP");

		//Impressão do relatório na tela
		MH.Relatorio(&s_best, filename);

		delete (p);
		
	}
	return 0;
}