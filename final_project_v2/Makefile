CXX = g++
CFLAGS  = -O3 -std=c++11
DIR = src/

all: myPolygon

myPolygon: $(DIR)main.cpp
	@$(CXX)  $(CFLAGS) $(DIR)main.cpp -o myPolygon
	@echo "compile successfully"
	@echo "To execute, type:"
	@echo "./myPolygon input_file output_file"

clean:
	rm  *.o myPolygon
