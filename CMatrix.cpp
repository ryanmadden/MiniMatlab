// CMatrix.cpp
// MiniMatlab
// Ryan Madden, Northwestern University
// Copyright, 2014
//
// This file contains the CMatrix class functions


#include <iostream>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include "CMatrix.h"

// for string input, we get something from previous projects
enum MATRIX_INPUT {NUM, SEP, END, ERR};

MATRIX_INPUT MatrixPiece(const char *buffer, int &st, char *piece)
{
	while (buffer[st] == ' ')
		st++;

	if (((buffer[st] <= '9') && (buffer[st] >= '0')) || (buffer[st] == '.'))
	{
		int ed = st;
		while (((buffer[ed] <= '9') && (buffer[ed] >= '0')) || (buffer[ed] == '.'))
		{
			piece[ed-st] = buffer[ed];
			ed++;
		}
		piece[ed-st] = 0;
		st = ed;
		return NUM;
	}
	else if (buffer[st] == ']')
	{
		++st;
		return END;
	}
	else if (buffer[st] == ';')
	{
		++st;
		return SEP;
	}
	else
		return ERR;
}

bool GetRowCol(const char *str, int &nRow, int &nCol)
{
	nRow = 0;
	nCol = 0;

	// should start with a '['
	if (str[0] != '[')
		return false;

	char piece[100];
	int col = 0, st = 1;
	for (;;)
	switch (MatrixPiece(str, st, piece))
	{
		case SEP: // new row
			if ((nRow != 0) && (col != nCol))
				return false;
			nCol = col;
			++nRow;
			col = 0;
			break;
		case NUM: // new number
			++col;
			break;
		case END:
			if ((nRow != 0) && (col != nCol))
				return false;
			nCol = col;
			++nRow;
			return true;
		case ERR:
			return false;
		default:
			return false;
	}
	return false;
}

void FillArray(const char *str, double *array, int size)
{
	// call GetRowCol first, this one should not fail

	char piece[100];
	int i = 0, st = 1;
	for (; i < size;)
		switch (MatrixPiece(str, st, piece))
	{
		case SEP:
			break;
		case NUM:
			array[i] = atof(piece);
			++i;
			break;
		case END:
			return;
		case ERR:
			assert(false);
		default:
			assert(false);
	}
}

CMatrix::CMatrix()
	: m_nRow(0), m_nCol(0), m_aData(0)
{
}

CMatrix::CMatrix(double d)
	: m_nRow(1), m_nCol(1), m_aData(new double[1])
{
	element(0, 0) = d;
}

CMatrix::CMatrix(int nRow, int nCol)
	: m_nRow(nRow), m_nCol(nCol), m_aData(new double[nRow*nCol])
{
	for (int i = 0; i < m_nRow; ++i)
		for (int j = 0; j < m_nCol; ++j)
			element(i, j) = 0;
}

CMatrix::CMatrix(const char *str)
	: m_nRow(0), m_nCol(0), m_aData(0)
{
	int nRow, nCol;
	if (!GetRowCol(str, nRow, nCol))
		return;

	m_nRow = nRow;
	m_nCol = nCol;
	m_aData = new double[m_nRow*m_nCol];

	FillArray(str, m_aData, m_nRow*m_nCol);
	// FillArray should not return false since we checked that with GetRowCol
}

CMatrix::~CMatrix()
{
	//delete m_aData;
}

void CMatrix::swap(CMatrix &m)
{
	int tmp_i;
	double *tmp_p;

	// swap all the member variables

	tmp_i = m_nCol; m_nCol = m.m_nCol; m.m_nCol = tmp_i;

	tmp_i = m_nRow; m_nRow = m.m_nRow; m.m_nRow = tmp_i;

	tmp_p = m_aData; m_aData = m.m_aData; m.m_aData = tmp_p;
}

bool CMatrix::IsNull()
{
	return m_aData == 0;
}

void CMatrix::resize(int nRow, int nCol)
{
	// 1 Create a new matrix m.
	// 2 Assign values to m.
	// 3 Make this matrix m by swapping.

	CMatrix m(nRow, nCol);

	for (int i = 0; (i < m_nRow) && (i < m.m_nRow); ++i)
		for (int j = 0; (j < m_nCol) && (j < m.m_nCol); ++j)
			m.element(i, j)  = element(i, j);

	// We don't need assignment here because we don't need m any more!
	swap(m);

	// m's destructor will be called here.
	// The old m_aData will be deleted then.
}

int CMatrix::getNRow()
{
	return m_nRow;
}

int CMatrix::getNCol()
{
	return m_nCol;
}

double &CMatrix::element(int i, int j)
{
	assert((i >= 0) && (i < m_nRow));
	assert((j >= 0) && (j < m_nCol));

	return m_aData[i*m_nCol+j];
}

//returns the result of adding two matrices; result is a null matrix if input matrices are not the same size
CMatrix CMatrix::operator+(CMatrix& mat)
{
    int row = mat.getNRow();
    int col = mat.getNCol();
    CMatrix temp = *new CMatrix(row, col);
    if (!(row == this->getNRow() && col == this->getNCol()))
    {
        temp = *new CMatrix();
        return temp;
    }
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            temp.element(i, j) = this->element(i, j) + mat.element(i, j);
        }
    }
    return temp;
}

//returns the result of subtracting one matrix from another; result is a null matrix if input matrices are not the same size
CMatrix CMatrix::operator-(CMatrix& mat)
{
    int row = mat.getNRow();
    int col = mat.getNCol();
    CMatrix temp = *new CMatrix(row, col);
    if (!(row == this->getNRow() && col == this->getNCol()))
    {
        temp = *new CMatrix();
        return temp;
    }
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            temp.element(i, j) = this->element(i, j) - mat.element(i, j);
        }
    }
    return temp;
}

//returns the result of multiplying this matrix by another matrix; if the matrices are the appropriate size, performs matrix multiplication, otherwise
CMatrix CMatrix::operator*(CMatrix& mat)
{
    double scalar;
    int row;
    int col;
    CMatrix temp;
    if (mat.getNCol() == 1 && mat.getNRow() == 1)
    {
        scalar = mat.element(0, 0);
        row = this->getNRow();
        col = this->getNCol();
        temp = *new CMatrix(row, col);
        for (int i = 0; i < row; i ++)
        {
            for (int j = 0; j < col; j++)
            {
                temp.element(i, j) = this->element(i, j) * scalar;
            }
        }
        return temp;
    }
    else if (this->getNCol() == 1 && this->getNRow() == 1)
    {
        scalar = this->element(0, 0);
        row = mat.getNRow();
        col = mat.getNCol();
        temp = *new CMatrix(row, col);
        for (int i = 0; i < row; i ++)
        {
            for (int j = 0; j < col; j++)
            {
                temp.element(i, j) = this->element(i, j) * scalar;
            }
        }
        return temp;
    }
    else if ((this->getNRow() == mat.getNCol()) && (this->getNCol() == mat.getNRow()))
    {
        row = this->getNRow();
        col = mat.getNCol();
        temp = *new CMatrix(row, col);
        int temp_sum = 0;
        for (int i = 0; i < row; i ++)
        {
            for (int j = 0; j < col; j++)
            {
                for (int k = 0; k < this->getNCol(); k++)
                {
                    temp_sum += (this->element(i, k) * mat.element(k, j));
                }
                temp.element(i, j) = temp_sum;
                temp_sum = 0;
            }
        }
        return temp;
    }
    else
    {
        temp = *new CMatrix(); //null matrix
        return temp;
    }
}

//returns the result of dividing this matrix by a scalar (1x1 matrix); the result can be a null matrix if mat is not 1x1
CMatrix CMatrix::operator/(CMatrix& mat)
{
    CMatrix temp;
    double divisor;
    if (!(mat.getNCol() == 1 && mat.getNRow() == 1))
    {
        temp = *new CMatrix(); //Create null matrix
        return temp;
    }
    else
    {
        int row = this->getNRow();
        int col = this->getNCol();
        temp = *new CMatrix(row, col);
        divisor = mat.element(0, 0);
        for (int i = 0; i < row; i ++)
        {
            for (int j = 0; j < col; j++)
            {
                temp.element(i, j) = this->element(i, j) / divisor;
            }
        }
        return temp;
    }
}

//returns true if the given CMatrix is the same size and contains all of the same values as this matrix (false otherwise)
bool CMatrix::operator==(CMatrix& mat)
{
    if (mat.getNRow() != this->getNRow())
    {
        return false;
    }
    if (mat.getNCol() != this->getNCol())
    {
        return false;
    }
    for (int i = 0; i < mat.getNRow(); i ++)
    {
        for (int j = 0; j < mat.getNCol(); j++)
        {
            if (mat.element(i, j) != this->element(i, j))
            {
                return false;
            }
        }
    }
    return true;
}

//returns false if the given CMatrix is the same size and contains all of the same values as this matrix (true otherwise)
bool CMatrix::operator!=(CMatrix& mat)
{
    if (*this == mat)
    {
        return false;
    }
    else
    {
        return true;
    }
}

void printMatrix(CMatrix &m)
{
	if (m.IsNull())
		std::cout << "\tnull matrix" << std::endl;
	else
	{
		// row by row
		for (int i = 0; i < m.getNRow(); ++i)
		{
			std::cout << "\t\t";
			for (int j = 0; j < m.getNCol(); ++j)
				std::cout << m.element(i, j) << "\t";
			std::cout << std::endl;
		}
	}
	std::cout << std::endl;
}
