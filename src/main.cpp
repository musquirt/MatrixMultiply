#include <pthread.h>
#include <iostream>
#include <list>
#include <sys/time.h>
#include <unistd.h>
#include <cstdlib>
#include "Matrix.cpp"

using namespace std;

#define ROWS_1 1000
#define COLS_1 1000
#define ROWS_2 COLS_1
#define COLS_2 1000
#define NUM_THREADS  1000

void ident(double *a, int n, double v);
void incVals(double *a, int n);
void* multiply(void* arg);

int main(int argc, char** argv) {
	double *a = (double *)malloc(ROWS_1*COLS_1 * sizeof(double));
	double *b = (double *)malloc(ROWS_2*COLS_2 * sizeof(double));
	double *c = (double *)malloc(ROWS_1*COLS_2 * sizeof(double));
	
	incVals(a, ROWS_1*COLS_1);
	ident(b, ROWS_2*COLS_2, 1);
	ident(c, ROWS_1*COLS_2, 0);
	
	pthread_t threads[NUM_THREADS];
	
	Matrix::setA(a);
	Matrix::setB(b);
	Matrix::setC(c);
	
	list< Matrix> m;
	
	struct timeval start, end;
	int long mtime, seconds, useconds;
	gettimeofday(&start, NULL);
	for (int i=0; i<NUM_THREADS; i++) {
		Matrix p(i, NUM_THREADS, ROWS_1, COLS_1, COLS_2);
		m.push_back(p);
		pthread_create(&(threads[i]), NULL, multiply, (void*) &m.back());
	}
	
	for (int i=0; i<NUM_THREADS; i++) {
		pthread_join(threads[i], NULL);
	}
	gettimeofday(&end, NULL);
	
	useconds = end.tv_usec - start.tv_usec;
	seconds  = end.tv_sec - start.tv_sec;
	cout << useconds + seconds*1000000 << " us" << endl;
	cout << useconds/1000 + seconds*1000 << " ms" << endl;
	
	#if 0
	for (int i=0; i<ROWS_1*COLS_2; i++) {
		if (i % (COLS_2) == 0 && i != 0) {
			cout << endl;
		}
		cout << c[i] << " ";
	}
	cout << endl;
	#endif
	
	return 0;
}

void ident(double *a, int n, double v) {
	for (int i=0; i<n; i++) {
		a[i] = v;
	}
	return;
}

void incVals(double *a, int n) {
	for (int i=0; i<n; i++) {
		a[i] = i;
	}
	return;
}

void *multiply(void* arg) {
	Matrix m = *((Matrix*) arg);
	
	for (int i=m.tid*m.n; i<m.m*m.n; i+=m.num_threads*m.n) {
		for (int j=0; j<m.q; j++) {
			double sum = 0;
			for (int k=0; k<m.n; k++) {
				sum += Matrix::a[i+k]*Matrix::b[j+k*m.n];
			}
			Matrix::c[i+j] = sum;
		}
	} 
	
}

