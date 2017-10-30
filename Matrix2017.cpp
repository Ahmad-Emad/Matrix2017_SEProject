#include "Matrix2017.h"

int max(int a, int b)
{
	return (a > b ? a : b);
}

/*Matrix class implementation*/
Matrix::Matrix() //default constructor
{
	nRows = nColumns = 0;
	values = NULL;
}
Matrix::~Matrix() //default destructor
{
	reset();
}

Matrix::Matrix(int nRows, int nColumns, int initialization, double initializationValue)//initialize number of rows columns intitialization case 
{
	this->nRows = nRows;
	this->nColumns = nColumns;
	if ((nRows*nColumns) == 0)
	{
		values = NULL;
		return;
	}
	values = new double*[nRows];//create array with number of rows
	for (int iR = 0;iR<nRows;iR++)
	{
		values[iR] = new double[nColumns];// for each row create array with number of columns
		for (int iC = 0;iC<nColumns;iC++)
		{
			switch (initialization)//check initialization case: zeros , ones , eye , random or have initial value
					       //using enum  MI_ZEROS =0..MI_ONES=1..MI_EYE=2..MI_RAND=3..MI_VALUE=4
			{
			case MI_ZEROS:
				values[iR][iC] = 0;
				break;
			case MI_ONES:
				values[iR][iC] = 1;
				break;
			case MI_EYE:
				values[iR][iC] = (iR == iC) ? 1 : 0;//diagonal=1 , other elements = 0
				break;
			case MI_RAND: values[iR][iC] = (rand() % 1000000) / 1000000.0;
				break;
			case MI_VALUE:                               // use initializationValue
				values[iR][iC] = initializationValue;
				break;
			}
		}
	}
}

Matrix::Matrix(int nRows, int nColumns, double first, ...)// initialize number of rows columns and values using unlimited argument function
{
	this->nRows = nRows;
	this->nColumns = nColumns;
	if ((nRows*nColumns) == 0)
	{
		values = NULL;
		return;
	}
	values = new double*[nRows];
	va_list va;	//hold information about arguments 
	va_start(va, first);//initialize argument list
	for (int iR = 0;iR<nRows;iR++)
	{
		values[iR] = new double[nColumns];
		for (int iC = 0;iC<nColumns;iC++)
		{
			values[iR][iC] = (iC == 0 && iR == 0) ? first : va_arg(va, double);//check if first element (ir=ic=0)use first else use va_arg to get next argument 
		}
	}
	va_end(va);
}

Matrix::Matrix(Matrix& m)//initialize with matrix
{
	nRows = nColumns = 0;
	values = NULL;
	copy(m);
}

Matrix::Matrix(string s)
{
	nRows = nColumns = 0;
	values = NULL;
	copy(s);
}

Matrix::Matrix(double d)//initialize matrix 1x1 with one value
{
	nRows = nColumns = 0;
	values = NULL;
	copy(d);
}

void Matrix::copy(const Matrix& m)//copy matrix to new one
{
	reset();
	this->nRows = m.nRows;
	this->nColumns = m.nColumns;
	if ((nRows*nColumns) == 0)
	{
		values = NULL;
		return;
	}
	values = new double*[nRows];
	for (int iR = 0;iR<nRows;iR++)
	{
		values[iR] = new double[nColumns];
		for (int iC = 0;iC<nColumns;iC++)
		{
			values[iR][iC] = m.values[iR][iC];
		}
	}
}

void Matrix::copy(double d) // copy matrix with one value to new one
{
	reset();
	this->nRows = 1;
	this->nColumns = 1;
	values = new double*[1];
	values[0] = new double[1];
	values[0][0] = d;
}

void Matrix::copy(string s)
{
	reset();
	char* buffer = new char[s.length() + 1];
    //strcpy_s(buffer, s.length() + 1, s.c_str());
	strncpy(buffer, s.c_str(), s.length() + 1);
	//char* lineContext;
	const char* lineSeparators = ";\r\n";
	//char* line = strtok_s(buffer, lineSeparators, &lineContext);
   // char* line = strtok_s(buffer, lineSeparators, &lineContext);
     char* line = strtok(buffer, lineSeparators);
     char* Remainlines = strtok(NULL, "");
	while (line)
	{
		Matrix row;
       // char* context;
        const char* separators = " []";
		//char* token = strtok_s(line, separators, &context);
		char* token = strtok(line, separators);

		while (token)
		{
		    const double token_value=atof(token);
			Matrix item;
            item = (const double)token_value;
			row.addColumn(item);
			token = strtok(NULL, separators);

		}
		if (row.nColumns>0 && (row.nColumns == nColumns || nRows == 0))
			addRow(row);

        line = strtok(Remainlines, lineSeparators);
        Remainlines = strtok(NULL, "");
			//addRow(row);line = strtok_s(NULL, lineSeparators, &lineContext);
	}
	delete[] buffer;
}

void Matrix::reset() // delete the matrix
{
	if (values) 
	{
		for (int i = 0;i<nRows;i++)
			delete[] values[i];
		delete[] values;
	}
	nRows = nColumns = 0;
	values = NULL;
}

string Matrix::getString() // write data of the matrix in special format
{
	string s="";
	for (int iR = 0; iR<nRows; iR++)
	{
		for (int iC = 0; iC<nColumns; iC++)
		{
			//cout << values[iR][iC] << " ";

			char buffer[50]=""; 			     //initialize buffer with size =50
			snprintf(buffer, 50, "%g\t", values[iR][iC]);// write the data in the buffer with the format
			s += buffer;				     //add data in the buffer to s
		}
		//cout << endl;
		s += "\n";
	}
	return s;
}

Matrix Matrix::operator=(const Matrix& m)
{
	copy(m);
	return *this;
}

Matrix Matrix::operator=(const double d) { copy(d);return *this; }
Matrix Matrix::operator=(string s) { copy(s);return *this; }

void Matrix::add(const Matrix& m) // add two matrices
{
	if (nRows != m.nRows || nColumns != m.nColumns) // 2 matrices should bs same dimensions
		throw("Invalid matrix dimension");
	for (int iR = 0; iR < nRows; iR++)
	{
		for (int iC = 0;iC < nColumns;iC++)
			values[iR][iC] += m.values[iR][iC]; // add values of 2 matrices
	}
}
void Matrix::operator+=(Matrix& m) { add(m); } //add two matrices 
void Matrix::operator+=(double d) { add(Matrix(nRows, nColumns, MI_VALUE, d)); }
Matrix Matrix::operator+(Matrix& m) { Matrix r = *this;r += m;return r; } // add two matrices to new one .. C=A+B
Matrix Matrix::operator+(double d) { Matrix r = *this;r += d;return r; }

void Matrix::sub(const Matrix& m)
{
	if (nRows != m.nRows || nColumns != m.nColumns)
		throw("Invalid matrix dimension");
	for (int iR = 0;iR < nRows;iR++)
	{
		for (int iC = 0;iC < nColumns;iC++)
			values[iR][iC] -= m.values[iR][iC];
	}
}

void Matrix::operator-=(Matrix& m) { sub(m); }
void Matrix::operator-=(double d) { sub(Matrix(nRows, nColumns, MI_VALUE, d)); }
Matrix Matrix::operator-(Matrix& m) { Matrix r = *this;r -= m;return r; }
Matrix Matrix::operator-(double d) { Matrix r = *this;r -= d;return r; }

void Matrix::mul(Matrix& m)
{
	if (nColumns != m.nRows) //that's how matrices are multiplied
		throw("Invalid matrix dimension for multiplication");

	Matrix r(nRows, m.nColumns); //the dim of the product matrix

	for (int iR = 0; iR<r.nRows; iR++)
	{
		for (int iC = 0; iC < r.nColumns; iC++)
		{
			r.values[iR][iC] = 0; //initializing this particular element of the matrix with zero

			for (int k = 0; k < m.nColumns; k++)
				r.values[iR][iC] += values[iR][k] * m.values[k][iC];
		}
	}
	copy(r);
}


void Matrix::operator%=(const Matrix& m)
	{

	}



void Matrix::operator*=(Matrix& m) { mul(m); }
void Matrix::operator*=(double d)
{
	for (int iR = 0;iR<nRows;iR++)
		for (int iC = 0;iC<nColumns;iC++)
			values[iR][iC] *= d;
}
Matrix Matrix::operator*(Matrix& m) { Matrix r = *this;	r *= m;	return r; }
Matrix Matrix::operator*(double d) { Matrix r = *this;	r *= d;	return r; }


Matrix Matrix::operator/(Matrix& m) //C = A/B where C, A and B are all matrices
{
	Matrix r = *this;
	Matrix ret;
	ret = r.div(m);
    return ret;
}

Matrix Matrix::operator/(double d) //C = A/B where C, A are matrices and B is a double
{
	Matrix r = *this;
	for (int iR = 0; iR<r.nRows; iR++)
		for (int iC = 0; iC<r.nColumns; iC++)
			r.values[iR][iC] /= d;
	return r;
}

void Matrix::operator/=(Matrix& m) // Divides by m and stores the result in the calling function
{
	*this = div(m);
}

void Matrix::operator/=(double d) // Divides by d (element wise) and stores the result in the calling function
{
	for (int iR = 0; iR<nRows; iR++)
		for (int iC = 0; iC<nColumns; iC++)
			values[iR][iC] /= d;
}

Matrix Matrix::div(Matrix& m)//div C = A/B = A * B.getInverse();
{
	Matrix r = *this;
	if (nColumns != m.nRows)
		throw("First matrix must have the same number of columns as the rows in the second matrix.");
	Matrix t ;
	t= m.getInverse(); // get the inverse of B
	r *= t;
	return r;
}

Matrix Matrix::operator++() { add(Matrix(nRows, nColumns, MI_VALUE, 1.0));return *this; }

Matrix Matrix::operator++(int) { Matrix C = *this;add(Matrix(nRows, nColumns, MI_VALUE, 1.0));return C; }
Matrix Matrix::operator--() { add(Matrix(nRows, nColumns, MI_VALUE, -1.0));return *this; }

//add (-1) to each element of the matrix, then return the matrix.
Matrix Matrix::operator--(int) //int is not used.
{
	Matrix r = *this;
	add(Matrix(nRows, nColumns, MI_VALUE, -1.0));
	return r; 
}

//return the same matrix multiplied by a negative sign for each element.
Matrix Matrix::operator-()
{
	for (int iR = 0;iR < nRows;iR++)
	{
		for (int iC = 0;iC < nColumns;iC++)
			values[iR][iC] = -values[iR][iC];
	}
	return *this;
}

//return the same matrix.
Matrix Matrix::operator+() { return *this; }

//copy a submatrix (m) into a matrix, r & c are row & columns where we want to copy.
void Matrix::setSubMatrix(int r, int c, Matrix& m)
{
	if ((r + m.nRows)>nRows || (c + m.nColumns)>nColumns)
		throw("Invalid matrix dimension");
	for (int iR = 0;iR<m.nRows;iR++)
		for (int iC = 0;iC<m.nColumns;iC++)
			values[r + iR][c + iC] = m.values[iR][iC];
}

//extract a submatrix from matrix, r & c are row & column where we want to extract. nRows & nColumns are the rows & columns of the submatrix.
Matrix Matrix::getSubMatrix(int r, int c, int nRows, int nColumns)
{
	if ((r + nRows)>nRows || (c + nColumns)>nColumns)
		throw("Invalid matrix dimension");
	Matrix m(nRows, nColumns);
	for (int iR = 0;iR<m.nRows;iR++)
		for (int iC = 0;iC<m.nColumns;iC++)
			m.values[iR][iC] = values[r + iR][c + iC];
	return m;
}

/*void Matrix::setMatrix()
{

}*/

//add column to matrix (m).
void Matrix::addColumn(Matrix& m)
{
	Matrix n(max(nRows, m.nRows), nColumns + m.nColumns);
	n.setSubMatrix(0, 0, *this);
	n.setSubMatrix(0, nColumns, m);
	*this = n;
}

//add row to matrix (m).
void Matrix::addRow(Matrix& m)
{
	Matrix n(nRows + m.nRows, max(nColumns, m.nColumns));
	n.setSubMatrix(0, 0, *this);
	n.setSubMatrix(nRows, 0, m);
	*this = n;
}

//return cofactor matrix, r & c are element's row & column which we want to get its cofactor.
Matrix Matrix::getCofactor(int r, int c)
{
	//valid only for (2*2) matrices.
	if (nRows <= 1 && nColumns <= 1)
		throw("Invalid matrix dimension");
	Matrix m(nRows - 1, nColumns - 1);
	for (int iR = 0;iR<m.nRows;iR++)
		for (int iC = 0;iC<m.nColumns;iC++)
		{
			int sR = (iR<r) ? iR : iR + 1;
			int sC = (iC<c) ? iC : iC + 1;
			m.values[iR][iC] = values[sR][sC];
		}
	return m;
}

//return the determinant of the matrix.
double Matrix::getDeterminant()
{
	//valid only when rows=columns.
	if (nRows != nColumns)
		throw("Invalid matrix dimension");
	if (nRows == 1 && nColumns == 1)return values[0][0];
	double value = 0, m = 1;
	for (int iR = 0;iR<nRows;iR++)
	{
		value += m * values[0][iR] * getCofactor(0, iR).getDeterminant();
		m *= -1;
	}
	return value;
}

istream& operator >> (istream &is, Matrix& m) //inputs the matrix through "cin>>" example: Matrix myMatrix; cin>>myMatrix;
{
	string s;
	getline(is, s, ']'); //] is the delimiter at which the getline knows this is the end of the string
	s += "]"; //because it wasn't saved in the actual string and the constructor uses it
	m = Matrix( s); //uses the constructor which takes an input string
	return is;
}
ostream& operator << (ostream &os, Matrix& m) //prints out the matrix elements using "cout<<"
{
	os << m.getString();
	return os;
}

Matrix Matrix::getInverse()//inverse=(1/determinant)*transpose of cofactor matrix
{
	if (nRows != nColumns)//inverse can only be done on square matrices
		throw("Invalid Matrix Dimension");
	Matrix n(nRows, nColumns);// copy matrix
	for (int iR = 0;iR<n.nRows;iR++)
		for (int iC = 0;iC<n.nColumns;iC++)
		{
			n.values[iR][iC] = values[iR][iC];
		}
	double det_value = n.getDeterminant();//determinant value of the matrix

	Matrix m(nRows, nColumns);//cofactor matrix
	int sign = 1;

	for (int iR = 0;iR<m.nRows;iR++)
		for (int iC = 0;iC<m.nColumns;iC++)
		{
			m.values[iR][iC] = sign*n.getCofactor(iR, iC).getDeterminant();//getting detreminant values of cofactor matrix
			sign *= -1;//following sign rule in matrices
		}

	m.getTranspose();//transpose of cofactor matrix
	m *= (1 / det_value);
	return m;
}


Matrix Matrix::getTranspose() {
	Matrix x(nColumns, nRows);
	for (int ir = 0; ir<x.nRows;ir++) {
		for (int ic = 0; ic<x.nColumns;ic++) {
			x.values[ir][ic] = values[ic][ir];
		}
	}
	return x;
}



//void mul_dot_astrisk(Matrix& m)
//{
//	if (nRows != m.nRows || nColumns != m.nColumns)
//		throw("Invalid matrix dimension");
//
//}
