all:
	@g++ -O2 -Wall matrixproduct.cpp -o matrixproduct -lpapi

run: all
	@./matrixproduct < in.txt