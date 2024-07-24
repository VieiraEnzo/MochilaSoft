//============================================================================
// Name        : Mining.cpp
// Author      : Marcos A. Guerine (marcosgribeiro@gmail.com)
// Modified	by : Pedro Henrique González Silva (pehgonzalez@gmail.com)
// Version     : 1.0
// Description : Data mining interface
//============================================================================


#include "Mining.h"

using namespace std;

Mining::Mining(ES& _Elite, int _suporte, int _nMaxPatterns):Elite(_Elite){
	currentPattern = nPatterns = 0;
	numberMine = 0;
	sizeES = Elite.getESsize();
	min_sup = _suporte;
	listOfPatterns = NULL;
	mined = eschanged = false;
	nMaxPatterns =  _nMaxPatterns;
	listOfPatterns = new Pattern*[nMaxPatterns];
	for(int i = 0; i < nMaxPatterns ;i++){
		listOfPatterns[i] = new Pattern();
	}
}

Mining::~Mining(){
	for(int i = 0; i < nPatterns; i++){
		listOfPatterns[i]->~Pattern();
	}
	delete[] listOfPatterns;
}

/* Usage:   fpmax_hnmp <seed> <id_tmp_file> <database> <db_size> <min_sup> <#ofpatterns> <output>
   Warning: fpmax_hnmp should be recompile for each new computer configuration  */

void Mining::mine(){

	// New patterns to be mined
	nPatterns = 0;
	ostringstream buffer;
	buffer.str("");

	/* Usage:   fpmax_hnmp <seed> <id_tmp_file> <database> <db_size> <min_sup> <#ofpatterns> <output>
	   Warning: fpmax_hnmp should be recompile for each new computer configuration  */

	buffer
			<< "miner/fpmax_hnmp "
			<< "1 "
			<< (rand() % 100)  /* Or use another randomizing library such as Mersenne Twister */
			<< " miner/db.txt "
			<<  sizeES
			<< " " << min_sup
			<< " " << nMaxPatterns
			<< " miner/patterns.txt" ;
	int s = system(buffer.str().c_str());
	if(s != 0){ cerr << "Mining.cpp ERROR: function system cannot be performed." << endl; exit(1);}
	buffer.str("");
	numberMine++;
	eschanged = false;
	mined = true;

}

// Map all elite solutions to FPmax* input file
void Mining::map_file(){

	ofstream input ("miner/db.txt",std::ofstream::out); 
	if(!input) { 
		cerr << "Mining.cpp ERROR: cannot open file db.txt" << endl; 
		return ; 
	}
	
	vector<Solution> Temp = Elite.getConjSol();
	for(size_t i=0; i<Temp.size(); ++i){
		vector<int> lista=Temp[i].getKS();
		vector<int>::iterator it;
		for(it = lista.begin(); it!=lista.end(); it++){
			input<< *it <<" ";
		}
		input<<endl;
	}

	input.close(); 
	return;
}

// Read FPmax* output file
void Mining::unmapall_file(){
	FILE *fp = fopen("miner/patterns.txt", "r");
	if(!fp) { cerr << "Mining.cpp ERROR: Could not open patterns.txt file." << endl;  exit(1);}
	
	int r, supp, tam;
	// Read size and support of first pattern
	r = fscanf(fp, "%d;%d;", &tam, &supp);
	Pattern *p = new Pattern();
	p->support = supp;
	p->size = tam;
	if(!p->support && !p->size){
		// there was no mined patterns 
		numberMine--;
		eschanged = true;
		mined = false;
		delete(p);
		return;  
		min_sup--; // Warning: re-mine decrementing minimum support or stop mining?
		mine();
	}
	
	while(r == 2){
		int elem;
		for(int i = 0; i < tam; i++){
			
			int l = fscanf(fp, "%d", &elem);
			if (l < 1){ cerr << "Mining.cpp ERROR: fscanf nothing read." << endl; exit(1);}
			p->elements.push_back(elem);
		}
		*listOfPatterns[nPatterns++] = *p;
		r = fscanf(fp, "%d;%d;", &tam, &supp);
		delete(p);
		p = new Pattern();
		p->support = supp;
		p->size = tam;
	}
	fclose(fp);
	delete(p);
}



void Mining::printPatterns(const char *filename){
	// method to print all patterns

	std::string instanceName = filename;
	std::string str2 = instanceName.substr (8);  
	std::string delimiter = ".txt";
	std::string token = str2.substr(0, str2.find(delimiter)); 
	
	//cout << "Writing patterns found into file... ";

	//cout << "Mined patterns. Quantity: " << nPatterns << endl ;
	for(int i=0; i< nPatterns ;i++){
		//cout << "Support: " << listOfPatterns[i]->support << " | ";
		ofstream mappedPatterns;
		string str= to_string(i);
		mappedPatterns.open ("analysis/allResults/"+token+"/"+token+"_pattern_"+str+".txt");
		for(unsigned int j=0; j< listOfPatterns[i]->elements.size(); j++){
			//cout << listOfPatterns[i]->elements[j] << " ";
			mappedPatterns<< listOfPatterns[i]->elements[j] <<" ";
		}
		mappedPatterns.close();
		//cout << endl;
	}
	//cout << endl << endl;
	//cout<<"Patterns written in file!"<<endl;

}

int Mining::getSizePatterns(){
	return nPatterns;
}
Pattern** Mining::getlistOfPatterns(){
	return listOfPatterns;
}

//---------------------------------------------------------------------------
