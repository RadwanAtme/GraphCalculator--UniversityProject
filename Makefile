CC = /usr/local/bin/g++
OBJS_PROJECT = Main.o Graph.o Edge.o Vertex.o
OBJS = Graph.o Edge.o Vertex.o
EXEC_PROJECT = gcalc
ZIP_TEXT_FILES = design.pdf test_in.txt test_out.txt Makefile graph.i
ZIP_CPP_FILES = Main.cpp Graph.cpp Edge.cpp Vertex.cpp
ZIP_HEADER_FILES = MainFunctions.h Graph.h Edge.h Vertex.h
DEBUG_FLAG = -g
COMP_FLAG = -std=c++11 -Wall -Werror -pedantic-errors


$(EXEC_PROJECT) : $(OBJS_PROJECT)
	$(CC) $(DEBUG_FLAG) $(OBJS_PROJECT) -o $@ 
Main.o   : Main.cpp MainFunctions.h Graph.h Edge.h Vertex.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.cpp 
Graph.o  : Graph.cpp Graph.h Edge.h Vertex.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.cpp
Edge.o   : Edge.cpp Edge.h Vertex.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.cpp 
Vertex.o :Vertex.cpp Vertex.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.cpp

tar :
	zip gcalc.zip $(ZIP_TEXT_FILES) $(ZIP_CPP_FILES) $(ZIP_HEADER_FILES)

clean : 
	rm -f $(OBJS_PROJECT) $(EXEC_PROJECT)

libgraph.a: $(OBJS)
	ar -rs $@ $^
	