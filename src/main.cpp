#include <pthread.h>
#include <iostream>
#include <list>
#include <sys/time.h>
#include <unistd.h>
#include <cstdlib>
#include "Matrix.cpp"
#include "main.hpp"

using namespace std;



void fillIdent(double *a, int n, double v=1);
void fillIncr(double *a, int n, int startVal=0);
void fillRand(double *a, int n);
void* multiply(void* arg);
void printMatrix(double* , int , int );
void interpretTime(timeval , timeval );

int main(int argc, char** argv) {
	int rows_a = ROWS_1;
	int rows_b = ROWS_2;
	int cols_a = COLS_1;
	int cols_b = COLS_2;
	int n_threads = NUM_THREADS;
	int matrixType = -1;
	int num = 0;
	bool verbose = false;
	
	// get command  line opts
	if (!getCommandLineOptions(argc, argv, rows_a, rows_b, cols_a, cols_b, matrixType, num, n_threads, verbose)) {
		return -1;
	}
	
	if (cols_a != rows_b) {
		cout << "Number columns in first must equal number rows in second!"
		     << endl;
		return -1;
	} else if (n_threads > rows_a) {
		n_threads = rows_a;
	}
	
	double *a = (double *)malloc(rows_a*cols_a * sizeof(double));
	double *b = (double *)malloc(rows_b*cols_b * sizeof(double));
	double *c = (double *)malloc(rows_a*cols_b * sizeof(double));
	
	switch(matrixType) {
		case 0:
			fillIdent(a, rows_a*cols_a, num);
			fillIdent(b, rows_b*cols_b, num);
			break;
		case 1:	
			struct timeval r;
			srand(gettimeofday(&r,NULL));
			fillRand(a, rows_a*cols_a);
			fillRand(b, rows_b*cols_b);
			break;
		case 2:
			fillIncr(a, rows_a*cols_a, num);
			fillIncr(b, rows_b*cols_b, num);
			break;
		default:
			fillIdent(a, rows_a*cols_a, 1);
			fillIdent(b, rows_b*cols_b, 1);
			break;
	}
	
	pthread_t threads[n_threads];
	
	Matrix::setA(a);
	Matrix::setB(b);
	Matrix::setC(c);
	Matrix::n_by_t = n_threads * cols_a;
	Matrix::num_threads = n_threads;
	
	list< Matrix> m;
	
	struct timeval start, end;
	gettimeofday(&start, NULL);
	for (int i=0; i<n_threads; i++) {
		Matrix p(i, rows_a, cols_a, cols_b);
		m.push_back(p);
		pthread_create(&(threads[i]), NULL, multiply, (void*) &m.back());
	}
	
	for (int i=0; i<n_threads; i++) {
		pthread_join(threads[i], NULL);
	}
	gettimeofday(&end, NULL);
	
	if (verbose) {
		printMatrix(a, rows_a, cols_a);
		cout << "multiplied by" << endl;
		printMatrix(b, rows_b, cols_b);
		cout << "equals" << endl;
		printMatrix(c, rows_a, cols_b);
	}
	
	interpretTime(start, end);
	
	free(a);
	free(b);
	free(c);
	
	return 0;
}

