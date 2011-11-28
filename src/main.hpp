#ifndef MAIN_HPP
#define MAIN_HPP

#include <pthread.h>
#include <iostream>
#include "Matrix.cpp"

#define ROWS_1 11
#define COLS_1 5
#define ROWS_2 COLS_1
#define COLS_2 6
#define NUM_THREADS  8

using namespace std;

bool getCommandLineOptions(int argc, char** argv, int &rows_a, int &rows_b, int &cols_a, int &cols_b, int &matrixType, int &num, int &num_threads, bool &verbose) {
	for (int i=1; i<argc; i++) {
		string arg = argv[i];
		if (arg == "-a") {
			if (i+1 < argc) {
				i++;
				rows_a = atoi(argv[i]);
				if (i+1 < argc && argv[i+1][0] != '-') {
					i++;
					cols_a = atoi(argv[i]);
				} else {
					cols_a = rows_a;
				}
			} else {
				cout << "Expected integer(s) number after -a" << endl;
				return false;
			}
		} else if (arg == "-b") {
			if (i+1 < argc) {
				i++;
				rows_b = atoi(argv[i]);
				if (i+1 < argc && argv[i+1][0] != '-') {
					i++;
					cols_b = atoi(argv[i]);
				} else {
					cols_b = rows_b;
				}
			} else {
				cout << "Expected integer(s) number after -b" << endl;
				return false;
			}
		} else if (arg == "-rand") {
			matrixType = 1;
		} else if (arg == "-ident") {
			matrixType = 0;
			if ((i+1) < argc) {
				i++;
				num = atoi(argv[i]);
			} else {
				cout << "Expected integer number after -ident" << endl;
				return false;
			}
		} else if (arg == "-incr") {
			matrixType = 2;
			if ((i+1) < argc) {
				i++;
				num = atoi(argv[i]);
			} else {
				cout << "Expected integer number after -incr" << endl;
				return false;
			}
		} else if (arg == "-v") { // verbose mode
			verbose = true;
		} else if (arg == "-h" || arg == "-help") {
			cout << "Multi-Threaded Matrix Multiply" << endl;
			cout << "Options:" << endl;
			cout << "-a rows_a [cols_a]: Define the dimensions of the first matrix to be multplied. If only one dimension is given, the matrix is assumed square." << endl;
			cout << "-b rows_b [cols_b]: Define the dimensions of the second matrix to be multplied. If only one dimension is given, the matrix is assumed square." << endl;
			cout << "-t num_threads: Define the number of threads to utilize. If this number is greater than the number of rows in matrix A, the number of threads will drop to that number of rows." << endl;
			cout << "-rand: Entries in the matrices will be random floating point numbers." << endl;
			cout << "-ident num: Entries in the matrices will all be num." << endl;
			cout << "-incr start: Entries in the matrices will be increasing starting at start." << endl;
			cout << "-h|-help: display this message" << endl;
			return false;
		} else if (arg == "-t") {
			if (i+1<argc) {
				i++;
				num_threads = atoi(argv[i]);
			} else {
				cout << "Expected integer number after -t" << endl;
			}
		} else {
			cout << "Usage: " << argv[0] << " [-a rows_a [cols_a]] [-b rows_b [cols_b]] [-t num_threads] [-rand|-ident num|-incr start] [-v] [-h|-help]" << endl;
			return false;
		}
	}
	
	return true;
}

void* multiply(void* arg) {
	Matrix m = *((Matrix*) arg);
	for (int i=m.tid*m.n; i<m.m*m.n; i+=Matrix::n_by_t) {
		for (int j=0; j<m.q; j++) {
			double sum = 0;
			for (int k=0; k<m.n; k++) {
				sum += Matrix::a[i+k]*Matrix::b[j+k*m.q];
			}
			Matrix::c[i*m.q/m.n+j] = sum;
		}
	} 
}

void interpretTime(timeval start, timeval end) {
	long int useconds = end.tv_usec - start.tv_usec;
	long int seconds  = end.tv_sec - start.tv_sec;
	cout << useconds + seconds*1000000 << " us" << endl;
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

void fillIncr(double *a, int n, int startVal) {
	for (int i=startVal; i<n; i++) {
		a[i] = i;
	}
	return;
}

void fillRand(double *a, int n) {
	for (int i=0; i<n; i++) {
		a[i] = rand() % 100;
	}
	return;
}

#endif

