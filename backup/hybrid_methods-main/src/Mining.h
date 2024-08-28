//============================================================================
// Name        : Mining.h
// Author      : Marcos A. Guerine (marcosgribeiro@gmail.com)
// Modified	by : Pedro Henrique González Silva (pehgonzalez@gmail.com)
// Version     : 1.0
// Description : Data mining interface
//============================================================================

#ifndef _MINING_H_
#define _MINING_H_

#include <sstream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <set>
#include"ES.h"
#include"Solution.h"
#include"Pattern.h"

// Solution structure

class Mining{
	// Forward declaration
	private:	
		int nPatterns,					// number of patterns
			sizeES,						// current elite set size
			min_sup, 					// minimum support
			currentPattern, 			// current pattern
			numberMine,					// number of mining steps performed
			nMaxPatterns;
		
		Pattern** listOfPatterns;		// list of patterns that shall be mined

		ES& Elite;
		
		bool eschanged,					// true if ES has changed. false otherwise.
			 mined;						// true if the mining set has done
		
			
	public:
		Mining(ES& _Elite, int _suporte, int _nMaxPatterns);
		~Mining();
		
// Mining functions with file manipulations
		void mine();
		void map_file();
		void unmapall_file();
		void printPatterns(const char *filename);
		int getSizePatterns();

// To manipulate patterns
		Pattern** getlistOfPatterns();
};


//---------------------------------------------------------------------------
#endif
