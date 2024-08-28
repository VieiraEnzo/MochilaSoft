####################################################
##################### COMPILER #####################
####################################################

##### COMPILER CONFIGURATION's
# Compiler
CPP = g++
# Compilation parameters
# CCOPT = -std=c++11 -Ofast -g -fPIC -fexceptions -DIL_STD -fopenmp
CCOPT = -Ofast

## Include libraries identifiers
# CCLNFLAGS = -L$(LEMONLIBDIR) -lemon -L$(CPLEXLIBDIR) -lilocplex -lcplex  -L$(CONCERTLIBDIR) -lconcert -lm -pthread -ldl
# Header's include path
# CCFLAGS = $(CCOPT) -I$(CPLEXINCDIR) -I$(CONCERTINCDIR) -I$(LEMONINCDIR)
CCFLAGS = $(CCOPT)



####################################################
###################### MAKE ########################
####################################################

EXECUTAVEL = hello
PATHEXEC = ./bin
PATHSRC= ./src
PATHTEMP = ./temp

all:
	mkdir -p $(PATHEXEC)
	mkdir -p $(PATHTEMP)
	make $(EXECUTAVEL)

#Juntando todos os objetos e gerando o programa
$(EXECUTAVEL): $(PATHTEMP)/main.o $(PATHTEMP)/function1.o $(PATHTEMP)/function2.o
	$(CPP) $(CCFLAGS) $(PATHTEMP)/main.o $(PATHTEMP)/function1.o $(PATHTEMP)/function2.o -o $(PATHEXEC)/$(EXECUTAVEL)

$(PATHTEMP)/main.o: $(PATHSRC)/main.cpp
	$(CPP) $(CCFLAGS) -c $(PATHSRC)/main.cpp -o $(PATHTEMP)/main.o

$(PATHTEMP)/function1.o: $(PATHSRC)/function1.cpp
	$(CPP) $(CCFLAGS) -c $(PATHSRC)/function1.cpp -o $(PATHTEMP)/function1.o

$(PATHTEMP)/function2.o: $(PATHSRC)/function2.cpp
	$(CPP) $(CCFLAGS) -c $(PATHSRC)/function2.cpp -o $(PATHTEMP)/function2.o

####################################################
###################### CLEAN #######################
####################################################
clean:
	rm -rf $(PATHEXEC)
	rm -rf $(PATHTEMP)
	rm *.tmp


# all: hello

# hello: main.o function1.o function2.o
# 	g++ main.o function1.o function2.o -o hello

# main.o: main.cpp
# 	g++ -c main.cpp

# function1.o: function1.cpp
# 	g++ -c function1.cpp

# function2.o: function2.cpp
# 	g++ -c function2.cpp

# clean:
# 	rm -rf *o hello