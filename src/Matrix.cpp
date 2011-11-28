#ifndef MATRIX_CPP
#define MATRIX_CPP

#define DEFAULT_NUM_ROWS 3
#define DEFAULT_NUM_COLS 3

class Matrix {
public:
	int tid;
	static long int num_threads;
	int n;
	int m;
	int q;
	static double *a;
	static double *b;
	static double *c;
	static long int n_by_t;
	
	Matrix(int t, int m, int n, int q) : tid(t), n(n), m(m), q(q) {
	
	}
	
	static void setA(double* d) {
		a = d;
	}
	static void setB(double* d) {
		b = d;
	}
	static void setC(double* d) {
		c = d;
	}
};

double *Matrix::a;
double *Matrix::b;
double *Matrix::c;
long int Matrix::n_by_t;
long int Matrix::num_threads;

#endif

