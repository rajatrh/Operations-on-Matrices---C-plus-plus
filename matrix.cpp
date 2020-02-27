#ifndef MATRIX_CPP
#define MATRIX_CPP
#include <iostream>
#include <fstream>
#include <thread>
#include <typeinfo>
using namespace std;

// Include the header file
#include "matrix.h"

/*
 * A bare minimum logger
 * input : string pointer (to the log)
 * usage : Logger<T>("Test")
 */
template <typename T>
inline void Logger(const std::string &log) {
    if (Matrix<T>::isLoggerEnabled()) {
        std::cout << log << std::endl;   
    }
}

/*
 * Number of threads explicitly to be used
 * default : 1
 */
template <typename T>
int Matrix<T>::numThreads = std::thread::hardware_concurrency();;

/*
 * Logger is controlled using the boolean logging
 * default : false
 */
template <typename T>
bool Matrix<T>::logging = false;

/*
 * Constructor with row columns and initial value
 * inputs : _r -> number of rows
 *          _c -> number of columns
 *          _mat -> Value of the type T
 */
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

/*
 * Constructor with row columns and initial value
 * input : route -> file name
 *          
 * Could alternatively send File Pointer as well (not implemented)
 */
template <typename T>
Matrix<T>::Matrix(const std::string &route)
{
    std::ifstream file(route);
    if (!file)
    {
        std::cerr << "Error: Unable to open matrix file.\n";
        return;
    }
    int _r = 0, _c = 0;
    file >> _r;
    file >> _c;

    if (_r < 0 || _c < 0) {
        cout << "Error: Invalid dimensions for a matrix. Please verify the input.";
        return;
    }

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

/*
 * Copy constructor
 */
template <typename T>
Matrix<T>::Matrix(const Matrix<T> &_mat)
{
    matrix = _mat.matrix;
    rows = _mat.getRowSize();
    cols = _mat.getColSize();
}

/*
 * Virtual destructor
 */
template <typename T>
Matrix<T>::~Matrix() {}

/*
 * Assignment operator
 * inputs : _mat -> Matrix to be assigned to a variable
 * 
 */
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

/*
 * Inplace matric multiplication
 * inputs : _res -> Address to resultant matrix
 *          _mat1 -> 1st matrix
 *          _mat2 -> 2nd matrix
 *          currentThread -> current thread number
 *          numThreads -> Number of threads allocated
 */
template <class T>
void parallelMultiplication(const Matrix<T>* _mat1, const Matrix<T>* _mat2, int numThreads, int currentThread, Matrix<T>* _res) {

    for (int i = currentThread; i < _mat1->getRowSize(); i+=numThreads) {
        for (int j = 0; j < _mat2->getRowSize(); j++) {
            T result = 0;
            for (int k = 0; k < _mat1->getColSize(); k++) {
                result += ((*_mat1)(i,k)*(*_mat2)(j,k));
            }
            (*_res)(i,j) = result;
        }
    }
}

/*
 * Compare matrices for equality
 * inputs : _mat -> 1st matrix
 *          _mat2 -> 2nd matrix
 */
template <typename T>
bool Matrix<T>::equals(const Matrix<T> &_mat1, const Matrix<T> &_mat2) {
    if (_mat1.getColSize() != _mat2.getColSize() || _mat1.getRowSize() != _mat2.getRowSize()) {
        return false;
    }
    int rows = _mat1.getRowSize();
    int cols = _mat1.getColSize();

    for (unsigned i = 0; i < rows; i++)
    {
        for (unsigned j = 0; j < cols; j++)
        {
            if (_mat1(i,j) != _mat2(i,j)) {
                return false;
            }
        }
    }

    return true;
}

/*
 * Set up the matrix instatiate number of threads
 * inputs : _res -> Address to resultant matrix
 *          _mat -> 1st matrix
 *          _mat2 -> 2nd matrix
 */
template <typename T>
Matrix<T> Matrix<T>::multiply(const Matrix<T> &_mat1, const Matrix<T> &_mat2) {
    const char* type = typeid(T).name();
    if (*type != 'i' && *type != 'f' && *type != 'd') {
        cout << "Error: Multiplication operation on invalid type of data.";
    }
    if (_mat1.getColSize() != _mat2.getRowSize()) {
        cout << "Error: Cannot Multiply Matrices of Dimensions ";
        cout << "(" << _mat1.getRowSize() << "x" << _mat1.getColSize() << ")*";
        cout << "(" << _mat2.getRowSize() << "x" << _mat2.getColSize() << ")" << endl;
        cout << "        (Must be in the form (MxN)*(NxP)" << endl;
        return Matrix<T>(0,0,0);
    }
    // Parallel Method
    Matrix<T> mat2t = _mat2.transpose();
    Matrix<T> product(_mat1.getRowSize(), _mat2.getColSize(), 0.0);

    int maxNumCPU = std::thread::hardware_concurrency();
    int numCPU = Matrix<T>::getNumofThread();
    if (numCPU > maxNumCPU) {
        cout << "Error: Number of threads cannot exceed " <<maxNumCPU <<".";
        return Matrix<T>(0,0,0);
    } else if (numCPU <= 0) {
        cout << "Error: Invalid number of threads.";
        return Matrix<T>(0,0,0);
    }
    
    Logger<T>("Number of threads : " + std::to_string(numCPU));
    std::thread* threads = new std::thread[numCPU];

    const Matrix<T>* mat1Pointer = &_mat1;
    const Matrix<T>* ma2tPointer = &mat2t;
    
    Matrix<T>* productPointer = &product;

    for (int i = 0; i < numCPU; i++) {
        Logger<T>("Starting thread : " + std::to_string(i));
        threads[i] = thread(parallelMultiplication<T>, mat1Pointer, ma2tPointer, numCPU, i, productPointer);
    }

    for (int i = 0; i < numCPU; i++) {
        Logger<T>("Joining thread : " + std::to_string(i));
        threads[i].join();
    }

    delete[] threads;

    return product;
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

// Operator () overloaded and used to get elements at indices i,j
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

// Get number of rows
template <typename T>
unsigned Matrix<T>::getRowSize() const
{
    return this->rows;
}

// Get number of columns
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

// Get status of Logger
template <typename T>
bool Matrix<T>::isLoggerEnabled()
{
    return Matrix<T>::logging;
}

// Set status of Logger
template <typename T>
void Matrix<T>::enableLogger()
{
    Matrix<T>::logging = true;
}

// Print the matrix
// Can write an additional functionality to write/stream resutls to a file/memory
template <typename T>
void Matrix<T>::printMatrix()
{
    std::cout << "===============================" << std::endl;
    for (int i = 0; i < this->rows; i++)
    {
        for (int j = 0; j < this->cols; j++)
        {
            std::cout << this->matrix[i][j] << ", ";
        }
        std::cout << std::endl;
    }
    std::cout << "===============================" << std::endl;
}
#endif
