all:
	@g++ -O2 -Wall matrixproduct.cpp -o matrixproduct -lpapi
	@javac MatrixProduct.java

clean:
	@rm item*.txt

item1: all
	@touch item1-cpp.txt item1-java.txt
	@./matrixproduct < in1.txt >> item1-cpp.txt
	@java MatrixProduct < in1.txt >> item1-java.txt

item2: all
	@touch item2-cpp.txt item2-java.txt item2-cpp-only.txt
	@./matrixproduct < in2.txt >> item2-cpp.txt
	@java MatrixProduct < in2.txt >> item2-java.txt
	@.matrixproduct < in2-cpp-only.txt >> item2-cpp-only.txt

item3: all
	@touch item3-cpp.txt
	@./matrixproduct < in3.txt >> item3-cpp.txt
