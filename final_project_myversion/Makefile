CXX = g++
CFLAGS  = -O3 -std=c++11
DIR = src/

all: myPolygon

myPolygon: $(DIR)main.cpp Node.o Reclist.o Polygon.o Structure.o
	@$(CXX)  $(CFLAGS) $(DIR)main.cpp Node.o Reclist.o Polygon.o Structure.o -o myPolygon
	@echo "compile all successfully"
	@echo "To execute, type:"
	@echo "./myPolygon input_file output_file"

Node.o: $(DIR)Node.cpp
	@$(CXX)  $(CFLAGS) -c $(DIR)Node.cpp
	@echo "compile Node.o successfully"

Reclist.o: $(DIR)Reclist.cpp
	@$(CXX)  $(CFLAGS) -c $(DIR)Reclist.cpp
	@echo "compile Reclist.o successfully"

Polygon.o: $(DIR)Polygon.cpp
	@$(CXX)  $(CFLAGS) -c $(DIR)Polygon.cpp
	@echo "compile Polygon.o successfully"

Structure.o: $(DIR)Structure.cpp
	@$(CXX)  $(CFLAGS) -c $(DIR)Structure.cpp
	@echo "compile Structure.o successfully"    

clean:
	rm  *.o myPolygon
