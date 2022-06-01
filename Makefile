all:
	g++ -o kmer -I./inc/ ./src/hash.cpp ./src/bloom.cpp -fopenmp -lpthread 
