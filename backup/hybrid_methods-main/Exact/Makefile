####################################################
###################### MAKE ########################
####################################################

EXECUTAVEL = main
PATHEXEC = ./bin
PATHSRC= ./src
PATHTEMP = ./temp

# List all source files
SRCS := $(wildcard $(PATHSRC)/*.cpp)
# Generate object file paths from source files
OBJS := $(patsubst $(PATHSRC)/%.cpp,$(PATHTEMP)/%.o,$(SRCS))

all:
	mkdir -p $(PATHEXEC)
	mkdir -p $(PATHTEMP)
	make $(EXECUTAVEL)


# Compile each source file into its corresponding object file
$(PATHTEMP)/%.o: $(PATHSRC)/%.cpp
	$(CPP) $(CCFLAGS) -c $< -o $@

# Link object files to create the executable
$(EXECUTAVEL): $(OBJS)
	$(CPP) $(CCFLAGS) $(OBJS) $(CCLNFLAGS) -o $(PATHEXEC)/$(EXECUTAVEL)


####################################################
###################### CLEAN #######################
####################################################
clean:
	rm -rf $(PATHEXEC)
	rm -rf $(PATHTEMP)
	rm *.tmp

####################################################
####################### CPLEX ######################
####################################################
# g++ Model.cpp -o model_test
# -I/opt/ibm/ILOG/CPLEX_Studio2211/cplex/include
# -I/opt/ibm/ILOG/CPLEX_Studio2211/concert/include
# -L/opt/ibm/ILOG/CPLEX_Studio2211/cplex/lib/x86-64_linux/static_pic_linux/static_pic
# -L/opt/ibm/ILOG/CPLEX_Studio2211/concert/lib/x86-64_linux/static_pic
# -lilocplex -lconcert -lcplex -lm -lpthread

##### CPLEX CONFIGURATION's
# System architecture
SYSTEM = x86-64_linux
# Static library format for Cplex
LIBFORMAT = static_pic
# Cplex directory
CPLEXDIR = /opt/ibm/ILOG/CPLEX_Studio2211/cplex
# Concert directory
CONCERTDIR = /opt/ibm/ILOG/CPLEX_Studio2211/concert

##### CPLEX DIRECTIVE's
# Cplex static libraries directory
CPLEXLIBDIR = $(CPLEXDIR)/lib/$(SYSTEM)/$(LIBFORMAT)
# Concert static libraries directory
CONCERTLIBDIR = $(CONCERTDIR)/lib/$(SYSTEM)/$(LIBFORMAT)
# Cplex header's directory
CPLEXINCDIR = $(CPLEXDIR)/include
# Concert header's directory
CONCERTINCDIR = $(CONCERTDIR)/include

####################################################
##################### COMPILER #####################
####################################################

##### COMPILER CONFIGURATION's
# Compiler
CPP = g++
# Compilation parameters
CCOPT = -std=c++17 -O3 -g -fPIC -fexceptions -DIL_STD -fopenmp
## Include libraries identifiers
CCLNFLAGS = -L$(CPLEXLIBDIR) -lilocplex -lcplex  -L$(CONCERTLIBDIR) -lconcert -lm -pthread -ldl
# Header's include path
CCFLAGS = $(CCOPT) -I$(CPLEXINCDIR) -I$(CONCERTINCDIR)
