#ifndef MATRIX_CPP
#define MATRIX_CPP
#include <iostream>
#include <fstream>
#include <thread>
using namespace std;

// Include the header file
#include "matrix.h"

template <typename T>
int Matrix<T>::numThreads = 0;
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


template <class T>
void matrixParallel(const Matrix<T>* a, const Matrix<T>* b, int numThreads, int currentThread, Matrix<T>* c) {

    for (int i = currentThread; i < a->getRowSize(); i+=numThreads) {
        for (int j = 0; j < b->getRowSize(); j++) {
            T result = 0;
            for (int k = 0; k < a->getColSize(); k++) {
                result += ((*a)(i,k)*(*b)(j,k));
            }
            (*c)(i,j) = result;
        }
    }

}

template <typename T>
Matrix<T> Matrix<T>::multiply(const Matrix<T> &m1, const Matrix<T> &m2) {    
    // Parallel Method
    Matrix<T> m2t = m2.transpose();
    Matrix<T> multiply(m1.getRowSize(), m2.getColSize(), 0.0);

    int numCPU = std::thread::hardware_concurrency();
    std::cout << numCPU;
    std::cout << endl;
    std::thread* threads = new std::thread[numCPU];

    const Matrix<T>* m1Pointer = &m1;
    const Matrix<T>* m2tPointer = &m2t;
    
    Matrix<T>* multiplyPointer = &multiply;

    for (int i = 0; i < numCPU; i++) {
        threads[i] = thread(matrixParallel<T>, m1Pointer, m2tPointer, numCPU, i, multiplyPointer);
    }

    for (int i = 0; i < numCPU; i++) {
        threads[i].join();
    }

    delete[] threads;

    return multiply;
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
Matrix<T> Matrix<T>::transpose() const
{
    int rows = this->getRowSize();
    int cols = this->getColSize();
    Matrix result(cols, rows, 0.0);

    for (unsigned i = 0; i < cols; i++)
    {
        for (unsigned j = 0; j < rows; j++)
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

// Set number of threads for execution
template <typename T>
void Matrix<T>::setNumofThread(int _num)
{
    Matrix<T>::numThreads = _num;
}

// Get number of threads for execution
template <typename T>
int Matrix<T>::getNumofThread()
{
    return Matrix<T>::numThreads;
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
    std::cout << std::endl;
}

void checkDimen() {
     // if (m1.get () != m2.getRows()) {
    //     cout << "Error: Cannot Multiply Matrices of Dimensions ";
    //     cout << "(" << m1.getRows() << "x" << m1.getCols() << ")*";
    //     cout << "(" << m2.getRows() << "x" << m2.getCols() << ")" << endl;
    //     cout << "        (Must be in the form (MxN)*(NxP)" << endl;
    //     return Matrix<type>();
    // }
}

#endif
