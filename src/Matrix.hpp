

#define DEFAULT_NUM_ROWS 3
#define DEFAULT_NUM_COLS 3

class Matrix {
private:
	double *rows;
	double *cols;
public:	
	virtual ~Matrix();
	Matrix();
	Matrix(int num_rows, int num_cols);
	double multiply();
};

