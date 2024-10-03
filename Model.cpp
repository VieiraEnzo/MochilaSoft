#ifndef MODEL_CPP_
#define MODEL_CPP_

#include "Model.h"

// cmd: g++ Model.cpp -o model_test -I/opt/ibm/ILOG/CPLEX_Studio2211/cplex/include -I/opt/ibm/ILOG/CPLEX_Studio2211/concert/include -L/opt/ibm/ILOG/CPLEX_Studio2211/cplex/lib/x86-64_linux/static_pic_linux/static_pic

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


void Model::setMIPstart(vector<int> &solution, IloCplex &cplex, IloBoolVarArray &x, ProblemInstance* _p) {  	
    int solvalx[_p->num_items]; 

    vector<int> bin_solution = convertToBinaryVector(solution, _p); 

    // Solution mip_solution(_p); 

    // cout << "mip_start solution: " << endl;
    // for(const auto i: solution){
    //     cout << solution[i] << " "; 
    //     mip_solution.add_item(i);
    // }
    // cout << endl; 

    // cout << "mip_start solution cost: " << mip_solution.getCost() << endl; 

    for (int i=0;i<_p->num_items;++i){ 		 		
        solvalx[i] = bin_solution[i]; 	
    }  

    IloNumVarArray startVar(env); 	
    IloNumArray startVal(env);  	
    for (int i=0;i<_p->num_items;++i) 	{ 		
        startVar.add(x[i]); 		
        startVal.add(solvalx[i]); 	
    } 
    cplex.addMIPStart(startVar, startVal);
    startVal.end();
    startVar.end();
}

void Model::addObjective(IloEnv &env, IloModel &model, IloBoolVarArray &x, IloBoolVarArray &v, ProblemInstance* _p){
    IloExpr summation(env);
    for(int i=0;i<_p->num_items;++i){
        summation += _p->profits[i]*x[i];
    }

    for(int k=0;k<_p->num_forfeits_pairs;++k){
        summation -= _p->forfeits_costs[k]*v[k];
    }

    model.add(IloMaximize(env,summation));
}


void Model::addConstraintTotalCapacity(IloEnv &env,IloModel &model,IloBoolVarArray &x, ProblemInstance* _p){
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

void Model::addConstraintLinearization(IloEnv &env,IloModel &model, IloBoolVarArray &x, IloBoolVarArray &v, ProblemInstance* _p){
    int idx_pair = 0;
    for (const auto& pair : _p->forfeits_pairs) {
        model.add(IloRange(env, -IloInfinity, x[pair.first] + x[pair.second] - v[idx_pair], 1, "linearizationConstraint")); 
        idx_pair = idx_pair + 1;   
    }
    
}

void Model::addConstraintofChoosePattern(IloEnv &env, IloModel &model, IloBoolVarArray &z, int num_patterns){
    IloExpr summation(env);
    for(int p= 0; p<num_patterns;++p){
        summation += z[p];       
    }
    IloRange choosepattern(env, 1, summation, IloInfinity);
    stringstream name;
    name << "ChoosePattern: ";
    choosepattern.setName(name.str().c_str());
    model.add(choosepattern); 
} 


void Model::addConstraintofFixPattern(IloEnv &env, IloModel &model, IloBoolVarArray &x, IloBoolVarArray &z, const std::vector<std::vector<int>>& pattern_matrix, int num_patterns, ProblemInstance* _p){
    for(int p= 0; p<num_patterns;++p){
        for (int i=0; i < _p->num_items; i++){
            model.add(IloRange(env, 0, x[i] - pattern_matrix[i][p]*z[p], IloInfinity, "fixPatternsConstraint"));   
        }
    }
    
    // IloExpr summation(env);
    // for(int p= 0; p<num_patterns;++p){
    //     for (int i=0; i < _p->num_items; i++){
    //         summation += x[i] - pattern_matrix[i][p]*z[p];       
    //     }
    // }
    
    // IloRange fixpattern(env, 0, summation, IloInfinity);
    // stringstream name;
    // name << "Fix Pattern: ";
    // fixpattern.setName(name.str().c_str());
    // model.add(fixpattern); 
}

void Model::addConstraintLocalBranching(IloEnv &env,IloModel &model,IloBoolVarArray &x, std::vector<int> &solution, ProblemInstance* _p){
    double delta = 0.5 * _p->num_items; 
    IloExpr summation(env);

    vector<int> bin_solution = convertToBinaryVector(solution, _p); 

    for(int i=0;i<_p->num_items;++i){
        if (bin_solution[i] == 1){
            summation += (1 - x[i]);
        }else{
            summation += x[i];
        }
    }
    
    //LB FAB
    summation -= delta; 
    IloRange constr_localbranch(env, -IloInfinity, summation, 0);
    stringstream lbfab;
    lbfab << "constr_localbranching";
    constr_localbranch.setName(lbfab.str().c_str());
    model.add(constr_localbranch);    
}


void Model::addConstraintImprSol(IloEnv &env, IloModel &model, IloBoolVarArray &x, IloBoolVarArray &v, ProblemInstance* _p, int best_value){
    IloExpr summation(env);
    for(int i=0;i<_p->num_items;++i){
        summation += _p->profits[i]*x[i];
    }

    for(int k=0;k<_p->num_forfeits_pairs;++k){
        summation -= _p->forfeits_costs[k]*v[k];
    }
    IloRange checkimpr(env, best_value, summation, IloInfinity);
    stringstream name;
    name << "checkimpr: ";
    checkimpr.setName(name.str().c_str());
    model.add(checkimpr); 
}



std::pair<Solution, int> Model::Build_Model_with_Patterns(ProblemInstance* _p, int num_patterns, const std::vector<std::vector<int>>& pattern_matrix, vector<int> best_solution, int best_value){
    env = IloEnv();
    Solution kp_solution(_p); 

    try{
    
        model = IloModel(env);
        
        // Declaring Variables
        IloBoolVarArray x(env, _p->num_items); 
        
        for (int i = 0; i < p->num_items ; ++i){
            stringstream varx;
            varx << "x" << i;
            x[i].setName(varx.str().c_str());
            model.add(x[i]);
        }

        IloBoolVarArray v(env, _p->num_forfeits_pairs); 
        // v = IloNumVarArray(env, _p->num_forfeits_pairs, 0.0, 1.0, IloNumVar::Float);

        for (int k = 0; k < p->num_forfeits_pairs ; ++k){
            stringstream varv;
            varv << "v" << k;
            v[k].setName(varv.str().c_str());
            model.add(v[k]);
        }

        IloBoolVarArray z(env, num_patterns); 
        
        for (int p = 0; p < num_patterns; ++p){
            stringstream varz;
            varz << "z" << p;
            z[p].setName(varz.str().c_str());
            model.add(z[p]);
        }

        //Add Constraints    
        addConstraintTotalCapacity(env, model, x, _p); 
        addConstraintLinearization(env, model, x, v, _p); 
        addConstraintofChoosePattern(env, model, z, num_patterns); 
        addConstraintofFixPattern(env, model, x, z, pattern_matrix, num_patterns, _p); 
        addConstraintImprSol(env, model, x, v, _p, best_value); 
        addObjective(env,model,x,v,_p);

    	ofstream outfile;
	    // cout << "---------------------filename:" << _p->filename << endl; 

        string log_filename = "./output_cplex/results/relatorio_"+_p->i_name+"_patterns.txt"; 
        outfile.open(log_filename, std::ios_base::app);
        

        knapsack = IloCplex(model);
        int timelimit = 2;  
        knapsack.setParam(IloCplex::Param::TimeLimit,timelimit);
        knapsack.setOut(outfile);
        setMIPstart(best_solution, knapsack, x, _p); 
        // knapsack.exportModel("KPFModel_patterns.lp");
        // cplex.extract(model); 


        // double objValue = knapsack.getObjValue();
        // cout << "Objective Value: " << objValue << endl; 

        // cout << "Number of mined patterns: " << num_patterns << endl; 
        // std::cout << "Values of z:" << std::endl;
        // int combined_patterns=0;
        // for (int p = 0; p < num_patterns; ++p) {
        //     std::cout << "z[" << p << "]: " << knapsack.getValue(z[p]) << std::endl;
        //    if(knapsack.getValue(z[p]) == 1){
        //        combined_patterns+=1;
        //     }
        // }
        // cout << "Number of combined patterns: " << combined_patterns << endl; 
        // cout << "----------\n";

        // cout << "get values: \n";
        // for(int i = 0; i < _p->num_items; i++){
        //     cout << knapsack.getValue(x[i]) << " "; 
        // }
        // cout << endl; 

        if(knapsack.solve()){
            double solveTime = knapsack.getTime();
            double objValue = knapsack.getObjValue();

            if(getStatus()==0){
                outfile<<"Optimum Found"<<endl;
                outfile<<"Value: "<<knapsack.getObjValue()<<endl;
                outfile<<"Solution: "; 
                for(int i = 0; i < _p->num_items; i++){
                    if(knapsack.getValue(x[i]) == 1){
                        outfile<<i<<" ";
                        kp_solution.add_item(i); 
                    }
                }
                outfile << endl; 
                outfile<<"GAP: "<<knapsack.getMIPRelativeGap()<<endl;
                outfile << "Solve Time: " << solveTime << " seconds\n" << endl;

            }else if(getStatus() == 1){
                outfile<<"Solution Found"<<endl;
                outfile<<"Value: "<<knapsack.getObjValue()<<endl;
                outfile<<"Solution: "; 
                for(int i = 0; i < _p->num_items; i++){
                    if(knapsack.getValue(x[i]) == 1){
                        outfile<<i<<" ";
                    }
                }
                outfile << endl; 
                outfile<<"GAP: "<<knapsack.getMIPRelativeGap()<<endl;
                outfile << "Solve Time: " << solveTime << " seconds\n" << endl;
            }

        }else{
            // Handle the infeasibility or other solution status
            outfile << "Solution not found. Status: " << knapsack.getStatus() << endl;

            IloAlgorithm::Status status = knapsack.getStatus();
            if (status == IloAlgorithm::Infeasible) {
                outfile << "The model is infeasible.\n" << endl;
            } else if (status == IloAlgorithm::Unbounded) {
                outfile << "The model is unbounded.\n" << endl;
            } else {
                outfile << "The model could not be solved for another reason.\n" << endl;
            }
            return make_pair(kp_solution, 0);
        }
        
        outfile << "\n\n ---------- Execution Model with Patterns for " + _p->filename + " ---------- \n\n" << endl; 
        outfile.close(); 
    }
    catch (IloException& e) {
       cerr << "Error: " << e.getMessage() << endl;
    }
    catch (...) {
        cerr << "Unknown exception caught" << endl;
    }
    
    // env.end();
	return make_pair(kp_solution, knapsack.getObjValue());
}

std::pair<Solution, double> Model::Build_Model_with_LB(ProblemInstance* _p, vector<int> best_solution){
    env = IloEnv();
    Solution kp_solution(_p); 

    try{
    
        model = IloModel(env);
        
        // Declaring Variables
        IloBoolVarArray x(env, _p->num_items);         
        // x = IloNumVarArray(env, _p->num_items, 0, 1, IloNumVar::Bool); 
        
        for (int i = 0; i < p->num_items ; ++i){
            stringstream varx;
            varx << "x" << i;
            x[i].setName(varx.str().c_str());
            model.add(x[i]);
        }


        // v = IloNumVarArray(env, _p->num_forfeits_pairs, 0.0, 1.0, IloNumVar::Float);
        IloBoolVarArray v(env, _p->num_forfeits_pairs);         


        for (int k = 0; k < p->num_forfeits_pairs ; ++k){
            stringstream varv;
            varv << "v" << k;
            v[k].setName(varv.str().c_str());
            model.add(v[k]);
        }


        //Add Constraints    
        addConstraintTotalCapacity(env, model, x, _p); 
        addConstraintLinearization(env, model, x, v, _p); 
        addConstraintLocalBranching(env,model, x, best_solution, _p);
        addObjective(env,model,x,v,_p);
        

    	ofstream outfile;
	    // cout << "---------------------instance_name:" << _p->i_name << endl; 
        string log_filename = "./output_cplex/local_branching/relatorio_"+_p->i_name+"_.txt"; 
        outfile.open(log_filename, std::ios_base::app);
        

        knapsack = IloCplex(model);
        int timelimit = 180;
        knapsack.setParam(IloCplex::Param::TimeLimit,timelimit);

        // Set the memory emphasis parameter to true to reduce memory usage
        knapsack.setParam(IloCplex::Param::Emphasis::Memory, IloTrue);
        knapsack.setOut(outfile);
        setMIPstart(best_solution, knapsack, x, _p); 

        // knapsack.exportModel("KPFModel.lp");
        // cplex.extract(model); 
		time(&start);
        knapsack.solve();
		time(&end);
        double total_time = double(end - start); 

        // cout << "number of patterns: " << num_patterns << endl; 
        // std::cout << "Values of z:" << std::endl;
        // for (int p = 0; p < num_patterns; ++p) {
        //     std::cout << "z[" << p << "]: " << knapsack.getValue(z[p]) << std::endl;
        // }

        if(getStatus()==0){
		    outfile<<"Optimum Found"<<endl;
            outfile<<"Value: "<<knapsack.getObjValue()<<endl;
            outfile<<"Exact Solution: "; 
            for(int i = 0; i < _p->num_items; i++){
                if(knapsack.getValue(x[i]) == 1){
                    outfile<<i<<" ";
                    kp_solution.add_item(i); 
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
                if(knapsack.getValue(x[i]) == 1){
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
    
	return make_pair(kp_solution, knapsack.getObjValue());

    //cout << " Max =" << cplex.getObjValue() << endl ; 
}


std::pair<Solution, int> Model::Build_Model(ProblemInstance* _p, vector<int> best_solution){
    env = IloEnv();
    Solution kp_solution(_p); 

    try{
    
        model = IloModel(env);
        
        // Declaring Variables
        IloBoolVarArray x(env, _p->num_items); 

        // x = IloNumVarArray(env, _p->num_items, 0, 1, IloNumVar::Bool); 
        
        for (int i = 0; i < p->num_items ; ++i){
            stringstream varx;
            varx << "x_" << i;
            x[i].setName(varx.str().c_str());
            model.add(x[i]);
        }


        // v = IloNumVarArray(env, _p->num_forfeits_pairs, 0.0, 1.0, IloNumVar::Float);
        IloBoolVarArray v(env, _p->num_forfeits_pairs); 


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
	    // cout << "---------------------instance_name:" << _p->i_name << endl; 
        string log_filename = "./output_cplex/results/relatorio_"+_p->i_name+"_model.txt"; 
        outfile.open(log_filename, std::ios_base::app);
        

        knapsack = IloCplex(model);
        knapsack.setParam(IloCplex::Param::TimeLimit,2);
        knapsack.setOut(outfile);
        setMIPstart(best_solution, knapsack, x, _p); 
        // knapsack.exportModel("KPFModel.lp");
       
        if(knapsack.solve()){
            double solveTime = knapsack.getTime();
            double objValue = knapsack.getObjValue();

            if(getStatus()==0){
                outfile<<"Optimum Found"<<endl;
                outfile<<"Value: "<<knapsack.getObjValue()<<endl;
                outfile<<"Solution: "; 
                for(int i = 0; i < _p->num_items; i++){
                    if(knapsack.getValue(x[i]) == 1){
                        outfile<<i<<" ";
                        kp_solution.add_item(i); 
                    }
                }
                outfile << endl; 
                outfile<<"GAP: "<<knapsack.getMIPRelativeGap()<<endl;
                outfile << "Solve Time: " << solveTime << " seconds\n" << endl;

            }else if(getStatus() == 1){
                outfile<<"Solution Found"<<endl;
                outfile<<"Value: "<<knapsack.getObjValue()<<endl;
                outfile<<"Solution: "; 
                for(int i = 0; i < _p->num_items; i++){
                    if(knapsack.getValue(x[i]) == 1){
                        outfile<<i<<" ";
                    }
                }
                outfile << endl; 
                outfile<<"GAP: "<<knapsack.getMIPRelativeGap()<<endl;
                outfile << "Solve Time: " << solveTime << " seconds\n" << endl;
            }

        }else{
            // Handle the infeasibility or other solution status
            outfile << "Solution not found. Status: " << knapsack.getStatus() << endl;

            IloAlgorithm::Status status = knapsack.getStatus();
            if (status == IloAlgorithm::Infeasible) {
                outfile << "The model is infeasible.\n" << endl;
            } else if (status == IloAlgorithm::Unbounded) {
                outfile << "The model is unbounded.\n" << endl;
            } else {
                outfile << "The model could not be solved for another reason.\n" << endl;
            }
            return make_pair(kp_solution, 0);
        }
        
        outfile << "\n\n ---------- Execution Original Model for " + _p->filename + " ---------- \n\n" << endl; 
        outfile << "---------------------------------------------------------------\n\n"; 
        outfile.close(); 
    }
    catch (IloException& e) {
       cerr << "Error: " << e.getMessage() << endl;
    }
    catch (...) {
        cerr << "Unknown exception caught" << endl;
    }
    
	return make_pair(kp_solution, knapsack.getObjValue());
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