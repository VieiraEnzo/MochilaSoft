SOURCES = main.cpp Solution.cpp ProblemInstance.cpp Constructive.cpp ConstructiveCG.cpp LocalSearch.cpp ILS.cpp ES.cpp Mining.cpp Model.cpp Pattern.cpp
CXX = g++
CXXFLAGS = -O3 -std=c++17 -Wall -g -fPIC -fexceptions -DIL_STD -fopenmp
INCLUDES = -I/opt/ibm/ILOG/CPLEX_Studio2211/cplex/include -I/opt/ibm/ILOG/CPLEX_Studio2211/concert/include
LDFLAGS = -L/opt/ibm/ILOG/CPLEX_Studio2211/cplex/lib/x86-64_linux/static_pic -lilocplex -lcplex -L/opt/ibm/ILOG/CPLEX_Studio2211/concert/lib/x86-64_linux/static_pic -lconcert -lm -pthread -ldl

all:
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SOURCES) $(LDFLAGS) -o a.out
