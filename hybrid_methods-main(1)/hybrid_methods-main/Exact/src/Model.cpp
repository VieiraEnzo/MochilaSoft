#ifndef MODEL_CPP_
#define MODEL_CPP_

#include "Model.h"

Model::Model(ProblemInstance* _p){
    p = _p;
}
Model::~Model(){ 
    env.end();
}

vector<int> convertToBinaryVector(vector<int> &solution, ProblemInstance* _p) {
    vector<int> binaryVec(_p->num_items, 0); 

    for (int item: solution) {
        if (item >= 0 && item < _p->num_items) {
            binaryVec[item] = 1; 
        }
    }

    return binaryVec;
}


void Model::addObjective(IloEnv &env, IloModel &model, IloNumVarArray &x, IloNumVarArray &v, ProblemInstance* _p){
    IloExpr summation(env);
    for(int i=0;i<_p->num_items;++i){
        summation += _p->profits[i]*x[i];
    }

    for(int k=0;k<_p->num_forfeits_pairs;++k){
        summation -= _p->forfeits_costs[k]*v[k];
    }

    model.add(IloMaximize(env,summation));
}


void Model::addConstraintTotalCapacity(IloEnv &env,IloModel &model,IloNumVarArray &x, ProblemInstance* _p){
    IloExpr summation(env);
    for(int i= 0; i<_p->num_items;++i){
        summation += x[i]*_p->weights[i];       
    }
    IloRange total_capacity(env, -IloInfinity, summation, _p->budget); 
    stringstream name;
    name << "Capacity: ";
    total_capacity.setName(name.str().c_str());
    model.add(total_capacity); 
}

void Model::addConstraintLinearization(IloEnv &env,IloModel &model, IloNumVarArray &x, IloNumVarArray &v, ProblemInstance* _p){
    int idx_pair = 0;
    for (const auto& pair : _p->forfeits_pairs) {
        model.add(IloRange(env, -IloInfinity, x[pair.first] + x[pair.second] - v[idx_pair], 1, "linearizationConstraint")); 
        idx_pair = idx_pair + 1;   
    }
    
}

void Model::Build_Model(ProblemInstance* _p){
    
    env = IloEnv();

    try{
    
        model = IloModel(env);
        
        // Declaring Variables
        x = IloNumVarArray(env, _p->num_items, 0, 1, IloNumVar::Bool); 
        
        for (int i = 0; i < p->num_items ; ++i){
            stringstream varx;
            varx << "x_" << i;
            x[i].setName(varx.str().c_str());
            model.add(x[i]);
        }


        v = IloNumVarArray(env, _p->num_forfeits_pairs, 0.0, 1.0, IloNumVar::Float);

        for (int k = 0; k < p->num_forfeits_pairs ; ++k){
            stringstream varv;
            varv << "v_" << k;
            v[k].setName(varv.str().c_str());
            model.add(v[k]);
        }

        //Add Constraints    
        addConstraintTotalCapacity(env, model, x, _p); 
        addConstraintLinearization(env, model, x, v, _p); 
        addObjective(env,model,x,v,_p);

        ofstream outfile;
        string log_filename = "./cplex_log/report_"+_p->i_name+"_.txt"; 
        outfile.open(log_filename, std::ios_base::app);

        knapsack = IloCplex(model);
        // int timelimit = 30;  
        // knapsack.setParam(IloCplex::Param::TimeLimit,timelimit);
        knapsack.setParam(IloCplex::Param::Emphasis::Memory, IloTrue);
        knapsack.setOut(outfile);
        // knapsack.exportModel("KPFModel.lp")

		time(&start);
        knapsack.solve();
		time(&end);
        double total_time = double(end - start); 

        if(getStatus()==0){
		    outfile<<"Optimum Found"<<endl;
            outfile<<"Value: "<<knapsack.getObjValue()<<endl;
            outfile<<"Exact Solution: "; 
            for(int i = 0; i < _p->num_items; i++){
                if(knapsack.getValue(x[i]) > 0){
                    outfile<<i<<" ";
                }
            }
            outfile << endl; 
            outfile<<"GAP: "<<knapsack.getMIPRelativeGap()<<endl;
            outfile << "Model Solve Total Time: " << total_time << endl; 
        }else if(getStatus() == 1){
            outfile<<"Solution Found"<<endl;
            outfile<<"Value: "<<knapsack.getObjValue()<<endl;
            outfile<<"Exact Solution: "; 
            for(int i = 0; i < _p->num_items; i++){
                if(knapsack.getValue(x[i]) > 0){
                    outfile<<i<<" ";
                }
            }
            outfile << endl; 
            outfile<<"GAP: "<<knapsack.getMIPRelativeGap()<<endl;
            outfile << "Model Solve Total Time: " << total_time << endl; 
        }else if(getStatus() == 2){
            outfile<< "Infeasible"<< endl;
            outfile << "Model Solve Total Time: " << total_time << endl; 
        }else{
            outfile<<"Unknown"<<endl;
            outfile << "Model Solve Total Time: " << total_time << endl; 
        }
        outfile << "---------------------\n\n";
        // cout<<"Model Solution Cost: "<<knapsack.getObjValue()<<endl;
        outfile.close(); 
    }
    catch (IloException& e) {
        cerr << "Concert exception caught: " << e << endl;
    }
    catch (...) {
        cerr << "Unknown exception caught" << endl;
    }
}

status Model::getStatus()
{
	if (knapsack.getStatus() == IloAlgorithm::Infeasible)
	{
		return INFEASIBLE;
	}
	else if(knapsack.getStatus() == IloAlgorithm::Optimal){
		return OPTIMALFOUND;
	}
	else
	{
		return SOLUTIONFOUND;
	}
}

#endif //MODEL_CPP_