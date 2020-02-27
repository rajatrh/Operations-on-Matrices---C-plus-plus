#ifndef MATRIX_CPP
#define MATRIX_CPP
#include <iostream>
#include <fstream>
// Include the header file
#include "matrix.h"

// Constructor with row, column and a value  
template <typename T>
Matrix<T>::Matrix(unsigned _r, unsigned _c, const T &_mat)
{
    matrix.resize(_r);
    for (unsigned i = 0; i < matrix.size(); i++)
    {
        matrix[i].resize(_c, _mat);
    }
    rows = _r;
    cols = _c;
}

// Constructor with filename
template <typename T>
Matrix<T>::Matrix(const std::string &route)
{

    std::ifstream file(route);
    if (!file)
    {
        std::cerr << "Unable to open matrix file.\n";
        return;
    }
    unsigned _r = 0, _c = 0;
    file >> _r;
    file >> _c;

    matrix.resize(_r);
    for (unsigned i = 0; i < matrix.size(); i++)
    {
        matrix[i].resize(_c);
        for (unsigned j = 0; j < _c; j++)
        {
            file >> matrix[i][j];
        }
    }
    rows = _r;
    cols = _c;
}

// Copy constructor
template <typename T>
Matrix<T>::Matrix(const Matrix<T> &_mat)
{
    matrix = _mat.matrix;
    rows = _mat.getRowSize();
    cols = _mat.getColSize();
}

// Destructor Virtual
template <typename T>
Matrix<T>::~Matrix() {}

// Assignment Operator 
template <typename T>
Matrix<T> &Matrix<T>::operator=(const Matrix<T> &_mat)
{
    if (&_mat == this)
        return *this;

    // new rows and new columns
    unsigned nrows = _mat.getRowSize();
    unsigned ncols = _mat.getColSize();

    matrix.resize(nrows);
    for (unsigned i = 0; i < matrix.size(); i++)
    {
        matrix[i].resize(ncols);
    }

    for (unsigned i = 0; i < nrows; i++)
    {
        for (unsigned j = 0; j < ncols; j++)
        {
            matrix[i][j] = _mat(i, j);
        }
    }
    rows = nrows;
    cols = ncols;

    return *this;
}

// Multiplication operator only
template <typename T>
Matrix<T> Matrix<T>::operator*(const Matrix<T> &_mat)
{
    unsigned rows = _mat.getRowSize();
    unsigned cols = _mat.getColSize();
    Matrix result(rows, cols, 0.0);

    for (unsigned i = 0; i < rows; i++)
    {
        for (unsigned j = 0; j < cols; j++)
        {
            for (unsigned k = 0; k < rows; k++)
            {
                result(i, j) += this->matrix[i][k] * _mat(k, j);
            }
        }
    }

    return result;
}

// Multiplication operator and assignment operator
template <typename T>
Matrix<T> &Matrix<T>::operator*=(const Matrix<T> &_mat)
{
    Matrix result = (*this) * _mat;
    (*this) = result;
    return *this;
}

// Transpose of a matrix
template <typename T>
Matrix<T> Matrix<T>::transpose()
{
    Matrix result(rows, cols, 0.0);

    for (unsigned i = 0; i < rows; i++)
    {
        for (unsigned j = 0; j < cols; j++)
        {
            result(i, j) = this->matrix[j][i];
        }
    }

    return result;
}

// Operator () overloaded
template <typename T>
T &Matrix<T>::operator()(const unsigned &row, const unsigned &col)
{
    return this->matrix[row][col];
}

// Operator () overloaded (const)
template <typename T>
const T &Matrix<T>::operator()(const unsigned &row, const unsigned &col) const
{
    return this->matrix[row][col];
}

// Number of rows
template <typename T>
unsigned Matrix<T>::getRowSize() const
{
    return this->rows;
}

// Number of columns
template <typename T>
unsigned Matrix<T>::getColSize() const
{
    return this->cols;
}

// Print the matrix
template <typename T>
void Matrix<T>::printMatrix()
{
    for (int i = 0; i < this->rows; i++)
    {
        for (int j = 0; j < this->cols; j++)
        {
            std::cout << this->matrix[i][j] << ", ";
        }
        std::cout << std::endl;
    }
}

#endif
