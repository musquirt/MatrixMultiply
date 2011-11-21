#include "Matrix.hpp"

/* Default constructor uses DEFAULT_NUM for rows, cols */
Matrix::Matrix() {
	rows = new double(DEFAULT_NUM_ROWS);
	cols = new double(DEFAULT_NUM_COLS);
}

Matrix::Matrix(int num_rows, int num_cols) {
	rows = new double(num_rows);
	cols = new double(num_cols);
}

Matrix::~Matrix() {
	delete rows;
	delete cols;
}

double Matrix::multiply() {
	return 0.0;
}

