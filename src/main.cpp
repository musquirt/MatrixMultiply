#include <pthread.h>
#include <iostream>
#include <list>
#include <sys/time.h>
#include <unistd.h>
#include <cstdlib>
#include "Matrix.cpp"

using namespace std;

#define ROWS_1 10
#define COLS_1 10
#define ROWS_2 COLS_1
#define COLS_2 10
#define NUM_THREADS  8

void fillIdent(double *a, int n, double v=1);
void fillIncr(double *a, int n);
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
	
	// get command  line opts
	
	if (cols_a != rows_b) {
		cout << "Number columns in first must equal number rows in second!"
		     << endl;
		return -1;
	} else if (rows_a < n_threads) {
		n_threads = rows_a;
	}
	
	double *a = (double *)malloc(rows_a*cols_a * sizeof(double));
	double *b = (double *)malloc(rows_b*cols_b * sizeof(double));
	double *c = (double *)malloc(rows_a*cols_b * sizeof(double));
	
	fillIdent(a, rows_a*cols_a);
	fillIdent(b, rows_b*cols_b, 1);
	fillIdent(c, rows_a*cols_b, 0);
	
	pthread_t threads[n_threads];
	
	Matrix::setA(a);
	Matrix::setB(b);
	Matrix::setC(c);
	Matrix::n_by_t = n_threads * rows_a;
	
	list< Matrix> m;
	
	struct timeval start, end;
	gettimeofday(&start, NULL);
	for (int i=0; i<n_threads; i++) {
		Matrix p(i, n_threads, rows_a, cols_a, cols_b);
		m.push_back(p);
		pthread_create(&(threads[i]), NULL, multiply, (void*) &m.back());
	}
	
	for (int i=0; i<n_threads; i++) {
		pthread_join(threads[i], NULL);
	}
	gettimeofday(&end, NULL);
	
	interpretTime(start, end);
	
	#if 0
	printMatrix(c, rows_a, cols_b);
	#endif
	
	return 0;
}

void interpretTime(timeval start, timeval end) {
	long int useconds = end.tv_usec - start.tv_usec;
	long int seconds  = end.tv_sec - start.tv_sec;
	if (seconds == 0) cout << useconds << " us" << endl;
	else cout << useconds/1000 + seconds*1000 << " ms" << endl;
	return;
}

void printMatrix(double* c, int r_c, int c_c) {
	for (int i=0; i<r_c*c_c; i++) {
		if (i % (c_c) == 0 && i != 0) {
			cout << endl;
		}
		cout << c[i] << " ";
	}
	cout << endl;
	return;
}

void fillIdent(double *a, int n, double v) {
	for (int i=0; i<n; i++) {
		a[i] = v;
	}
	return;
}

void fillIncr(double *a, int n) {
	for (int i=0; i<n; i++) {
		a[i] = i;
	}
	return;
}

void fillRand(double *a, int n) {
    struct timeval r;
	srand(gettimeofday(&r,NULL));
	for (int i=0; i<n; i++) {
		a[i] = rand();
	}
	return;
}

void* multiply(void* arg) {
	Matrix m = *((Matrix*) arg);
	for (int i=m.tid*m.n; i<m.m*m.n; i+=Matrix::n_by_t) {
		for (int j=0; j<m.q; j++) {
			double sum = 0;
			for (int k=0; k<m.n; k++) {
				sum += Matrix::a[i+k]*Matrix::b[j+k*m.n];
			}
			Matrix::c[i+j] = sum;
		}
	} 
	
}

