// CMatrix.h
// MiniMatlab
// Ryan Madden, Northwestern University
// Copyright, 2014
//
// Interface of the CMatrix class

#ifndef CMATRIX_H
#define CMATRIX_H

class CMatrix
{
	int		m_nRow; // # of rows
	int		m_nCol; // # of columns
	double	*m_aData;

public:
	CMatrix(); 	// make a null matrix

	CMatrix(double d); // 1 by 1 matrix
	CMatrix(int nRow, int nCol); // nRow by nCol zero matrix

	CMatrix(const char *str); // matrix from string

	~CMatrix();

	// Is this matrix a null matrix?
	bool IsNull();

	void swap(CMatrix &m); // swap this and m

	void resize(int nRow, int nCol);

	// get
	int	getNRow(); // return # of rows
	int	getNCol(); // return # of columns

	// return the element at i-th row and j-th column
	double &element(int i, int j);

	 // output the matrix
    
    //returns the result of adding two matrices; result is a null matrix if input matrices are not the same size
    CMatrix operator+(CMatrix& mat);
    
    //returns the result of subtracting one matrix from another; result is a null matrix if input matrices are not the same size
    CMatrix operator-(CMatrix& mat);
    
    //returns the result of multiplying this matrix by another matrix; the result can be a null matrix if neither this nor the given matrix are 1x1 (unless doing bonus)
    CMatrix operator*(CMatrix& mat);
    
    //returns the result of dividing this matrix by a scalar (1x1 matrix); the result can be a null matrix if mat is not 1x1
    CMatrix operator/(CMatrix& mat);
    
    //returns true if the given CMatrix is the same size and contains all of the same values as this matrix (false otherwise)
    bool operator==(CMatrix& mat);
    
    //returns false if the given CMatrix is the same size and contains all of the same values as this matrix (true otherwise)
    bool operator!=(CMatrix& mat);
    

}; // class CMatrix

void printMatrix(CMatrix &m); 

#endif // CMATRIX_H