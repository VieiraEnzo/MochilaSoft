#ifndef EXACT_CPP_
#define EXACT_CPP_

#include "Exact.h"

Exact::Exact(ProblemInstance* _p)
{

	//######################################################################
	// Inicialização
	//######################################################################
	p = _p;

	modelo = IloModel(env);
	constraints = IloConstraintArray(env);

	//######################################################################
	// criando a variável e adicionando ao modelo
	//######################################################################

	x = IloNumVarArray(env, p->NbItens, 0, 1, ILOBOOL);

	for (unsigned int i = 0; i < p->NbItens; i++){
		stringstream var;
		var << "x[" <<i << "]";
		x[i].setName(var.str().c_str());
		modelo.add(x[i]);
	}

	//######################################################################
	// Função Objetivo
	//######################################################################

	IloExpr ObjFunction(env);

	for (unsigned int i = 0; i < p->NbItens; i++){
		ObjFunction+=p->Value[i]*x[i];
	}

	modelo.add(IloMaximize(env, ObjFunction));
	addConstraint_Mochila(env, modelo);

}

void Exact::addConstraint_Mochila(IloEnv& env, IloModel& modelo)
{
	//######################################################################
	// Restrição de capacidade da mochila
	//######################################################################

	IloExpr SumW(env);
	for (int i = 0; i < p->NbItens; i++) {
		SumW += p->Weight[i] * x[i];
	}

	
	IloConstraint Mochila = SumW <= p->knapsackCapacity;
	
	stringstream name;
	name << "Mochila: ";
	Mochila.setName(name.str().c_str());
	modelo.add(Mochila);
	constraints.add(Mochila);
	
	/*
	IloRange constr_Capacity(env,0,SumW-p->knapsackCapacity,0);
	stringstream name;
	name<< "constr_Capacity: ";
	constr_Capacity.setName(name.str().c_str());
	modelo.add(constr_Capacity);
	*/
}

Exact::~Exact()
{
	Knapsack.end();
	env.end();
}


Solution Exact::solve(int timelimit, const char *filename)
{

	cout<<"Running Exact...";

	std::string instanceName = filename;
	std::string str2 = instanceName.substr (8);  
	std::string delimiter = ".txt";
	std::string token = str2.substr(0, str2.find(delimiter)); 

	Knapsack = IloCplex(modelo);
	Knapsack.setParam(IloCplex::TiLim, timelimit);

	Knapsack.solve();

	Solution s_exact(p);

	ofstream outfile;

	outfile.open("../analysis/allResults/"+token+"/"+token+"_relatorio.txt", std::ios_base::app); // append instead of overwrite

	cout<<endl;
	//cout<<"--------------- Exact Results: ------------------"<<endl;
	outfile << "--------------- Exact Results: ------------------"<<endl;

	if(getStatus()==0){
		
		//cout<<"Optimum Found"<<endl;
		outfile << "Optimum Found"<<endl; 
		//cout<<"Value: "<<Knapsack.getObjValue()<<endl;
		outfile << "Value: "<<Knapsack.getObjValue()<<endl;
		for(int i = 0; i < p->NbItens; i++){
			if(Knapsack.getValue(x[i]) > 0){
				s_exact.add_item(i);
				//cout<<i<<" ";
			}
		}
		//cout<<"Used Capacity: "<<s_exact.getCapacity()<<endl;
		outfile << "Used Capacity: "<<s_exact.getCapacity()<<endl;

		//cout<<"GAP: "<<Knapsack.getMIPRelativeGap()<<endl;
		outfile << "GAP: "<<Knapsack.getMIPRelativeGap()<<endl;
	}else if(getStatus() == 1){
		//cout<<"Solution Found"<<endl;
		outfile << "Solution Found"<<endl;
		//cout<<"Value: "<<Knapsack.getObjValue()<<endl;
		outfile << "Value: "<<Knapsack.getObjValue()<<endl;
		//cout<<"GAP: "<<Knapsack.getMIPRelativeGap()<<endl;
		outfile << "GAP: "<<Knapsack.getMIPRelativeGap()<<endl;
		for(int i = 0; i < p->NbItens; i++){
			if(Knapsack.getValue(x[i]) > 0){
				s_exact.add_item(i);
			}
		}
	}else if(getStatus() == 2){
		//cout<<"Infeasible"<<endl;
		outfile << "Infeasible"<<endl;
	}else{
		//cout<<"Unknown"<<endl;
		outfile << "Unknown"<<endl;
	}

	//cout<<"--------------------------------------------------"<<endl;
	outfile << "--------------------------------------------------"<<endl;
	//cout<<endl;
	outfile <<endl;

	cout<<"Exact runned..."<<endl;

	return s_exact;
}

void Exact::setPrimalBound(double cost)
{
	Knapsack.setParam(IloCplex::CutLo, cost);
}

status Exact::getStatus()
{
	if (Knapsack.getStatus() == IloAlgorithm::Infeasible)
	{
		return INFEASIBLE;
	}
	else if(Knapsack.getStatus() == IloAlgorithm::Optimal){
		return OPTIMALFOUND;
	}
	else
	{
		return SOLUTIONFOUND;
	}
}

#endif //EXACT_CPP_