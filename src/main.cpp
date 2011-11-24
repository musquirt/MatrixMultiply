#include <pthread.h>
#include <iostream>
#include <list>
#include <sys/time.h>
#include <unistd.h>
#include <cstdlib>
#include "Matrix.cpp"
#include <sstream>
#include <cmath>

using namespace std;

#define ROWS_1 3
#define COLS_1 4
#define ROWS_2 COLS_1
#define COLS_2 3
#define NUM_THREADS  4

void fillIdent(double *a, int n, double v=1);
void fillIncr(double *a, int n, int startVal=0);
void fillRand(double *a, int n);
void* multiply(void* arg);
void printMatrix(double* , int , int );
void interpretTime(timeval , timeval );
bool getCommandLineOptions(int argc, char** argv, int &rows_a, int &rows_b, int &cols_a, int &cols_b, int &matrixType, int &num, int &num_threads) {
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
		} else if (arg == "-h" || arg == "-help") {
			
			return false;
		} else if (arg == "-t") {
			if (i+1<argc) {
				i++;
				num_threads = atoi(argv[i]);
			} else {
				cout << "Expected integer number after -t" << endl;
			}
		} else {
			cout << "Usage: " << argv[0] << " [-a rows_a [cols_a]] [-b rows_b [cols_b]] [-t num_threads] [-rand|-ident num|-incr start]" << endl;
			return false;
		}
	}
	
	return true;
}

int main(int argc, char** argv) {
	int rows_a = ROWS_1;
	int rows_b = ROWS_2;
	int cols_a = COLS_1;
	int cols_b = COLS_2;
	int n_threads = NUM_THREADS;
	int matrixType = -1;
	int num = 0;
	
	// get command  line opts
	if (!getCommandLineOptions(argc, argv, rows_a, rows_b, cols_a, cols_b, matrixType, num, n_threads)) {
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
	
	//interpretTime(start, end);
	
	#if 1
	//printMatrix(a, rows_a, cols_a);
	//cout << "multiplied by" << endl;
	//printMatrix(b, rows_b, cols_b);
	//cout << "equals" << endl;
	printMatrix(c, rows_a, cols_b);
	#endif
	
	return 0;
}

void* multiply(void* arg) {
	Matrix m = *((Matrix*) arg);
	for (int i=m.tid*m.n; i<m.m*m.n; i+=Matrix::n_by_t) {
		for (int j=0; j<m.q; j++) {
			double sum = 0;
			for (int k=0; k<m.n; k++) {
				sum += Matrix::a[i+k]*Matrix::b[j+k*m.q];
			}
			stringstream out;
			out << i/Matrix::num_threads*m.q + j << " " << m.m*m.q << endl;
			cout << out.str();
			Matrix::c[i/Matrix::num_threads*m.q + j] = sum;
		}
	} 
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

void fillIncr(double *a, int n, int startVal) {
	for (int i=startVal; i<n; i++) {
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

