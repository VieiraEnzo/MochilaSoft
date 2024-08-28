flags = -Wall -O3

all:: fim_all fim_closed fim_maximal fpmax_hnmp

fpmax_hnmp: fim_maximal fpmax_hnmp.o 
	g++ $(flags) fpmax_hnmp.o -o fpmax_hnmp

fpmax_hnmp.o: fpmax_hnmp.cpp
	g++  $(flags) -c fpmax_hnmp.cpp

fim_all: fpgrowth.o fpdata.o fp_tree.o fpbuffer.o fpsout.o fp_node.o
	g++ $(flags) fpgrowth.o fpdata.o fp_tree.o fpbuffer.o fpsout.o fp_node.o -o fim_all

fim_closed: fpclosed.o fpdata.o fp_tree.o fpbuffer.o fpsout.o fp_node.o
	g++ $(flags) fpclosed.o fpdata.o fp_tree.o fpbuffer.o fpsout.o fp_node.o -o fim_closed

fim_maximal: fpmax.o fpdata.o fp_tree.o fpbuffer.o fpsout.o fp_node.o
	g++ $(flags) fpmax.o fpdata.o fp_tree.o fpbuffer.o fpsout.o fp_node.o -o fim_maximal

fpgrowth.o: fpmax.cpp fpdata.h fp_tree.h fpcommon.h fpsout.h
	g++ $(flags) -DFI -c fpmax.cpp -o fpgrowth.o

fpclosed.o: fpmax.cpp fpdata.h fp_tree.h fpcommon.h fpsout.h
	g++ $(flags) -DCFI -c fpmax.cpp -o fpclosed.o

fpmax.o: fpmax.cpp fpdata.h fp_tree.h fpcommon.h fpsout.h
	g++ $(flags) -DMFI -c fpmax.cpp

fpdata.o: fpdata.cpp fpdata.h
	g++ $(flags) -c fpdata.cpp

fp_tree.o: fp_tree.cpp fpdata.h fp_tree.h fpcommon.h fpsout.h
	g++ $(flags) -c fp_tree.cpp

fpbuffer.o: fpbuffer.cpp fpdata.h fpbuffer.h
	g++ $(flags) -c fpbuffer.cpp

fpsout.o: fpsout.cpp fpsout.h fpcommon.h
	g++ $(flags) -c fpsout.cpp

fp_node.o: fp_node.cpp fp_node.h fpsout.h fpcommon.h
	g++ $(flags) -c fp_node.cpp

clean:
	rm fim_all fim_closed fim_maximal fpmax_hnmp *.o
