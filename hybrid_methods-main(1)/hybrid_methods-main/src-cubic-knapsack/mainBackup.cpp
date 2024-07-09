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
		//srand(time(NULL));

		const char *filename = argv[1];
		int alpha = argc > 2 ? atof(argv[2]) : atof("0.7");
		int MaxIterGrasp = argc > 3 ? atoi(argv[3]) : atoi("700");
		int suporte = argc > 4 ? atoi(argv[4]) : atoi("5");

		//Criação de Instância
		ProblemInstance *p = new ProblemInstance();

		FWChrono Tempo;
		Tempo.start();

		p->load_file(filename);

		Tempo.stop();
		
		cout << "Tempo Leitura + Estruturas Básicas: " << Tempo.getStopTime() << "s" << endl;
		cout << "----------------------------------------------" << endl;

		Exact * modelo = new Exact (p);

		modelo->solve(3600);

		//Criação da Instância da melhor solução
		Solution s_best(p);
		
		Tempo.start();
		
		//GRASP
		GRASP Gr(p, MaxIterGrasp, alpha);
		//Melhor solução através do GRASP
		s_best = Gr.solve();
		//Impressão do relatório na tela
		Gr.Relatorio(&s_best);

		Tempo.stop();
		
		cout << "Best " << s_best.getCost() << endl;
		cout << "GRASP Tempo: " << Tempo.getStopTime() << "s" << endl;
		cout << "----------------------------------------------" << endl;
		
		Tempo.start();

		//Cria o GRASP que roda a mineração de dados no meio
		GRASP_DM MH(p,MaxIterGrasp,alpha, suporte);

		//A variável da melhor solução recebe o resultado do GRASP 
		s_best = MH.solve();

		//Impressão do relatório na tela
		MH.Relatorio(&s_best);

		Tempo.stop();
		int t_ls = ceil(Tempo.getStopTime());
		
		//Tempo que demorou para o GRASP DM rodar
		cout<<"GRASP DM: "<< Tempo.getStopTime()<<"s"<<endl;
		cout<<"----------------------------------------------"<<endl;
		//Exato

		delete (p);
		
	}
	return 0;
}
