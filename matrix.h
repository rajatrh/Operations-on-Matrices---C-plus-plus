#ifndef MATRIX_H
#define MATRIX_H

#include <vector>

/*
 * Versatile matrix implementation.
 * I am assuming that we are dealing with numerical data types alone.
 * 
 * I have considered std::vector, one of the many container classes from
 * Standard Template Library (STL).
 * 
 * Going by the assumption of considering only numerical values, I
 * could use std::valarray as well as it provides certain optimizations
 * for numerical values only.
 */

template <typename T>
class Matrix
{

/*
 * matrix - vector of vector
 * rows - number of rows
 * cols - number of cols
 */

private:
    std::vector<std::vector<T> > matrix;
    unsigned rows;
    unsigned cols;
    static int numThreads;
    // Variable to control logging
    static bool logging;
    Matrix<T> multiply(const Matrix<T> &m1, const Matrix<T> &m2);
    bool equals(const Matrix<T> &m1, const Matrix<T> &m2);

public:
    // Empty constructor
    Matrix() { }
    // Constructor overloading - filename
    Matrix(const std::string &route);
    // Constructor overloading - row, column and a value to be repeated
    Matrix(unsigned _r, unsigned _c, const T &_mat);
    // Constructor overloading - matrix
    Matrix(const Matrix<T> &_mat);
    // destructor
    virtual ~Matrix();
    // Assignment operators
    Matrix<T> &operator=(const Matrix<T> &_mat);
    // Multiplication operators with assignment 
    Matrix<T> &operator*=(const Matrix<T> &_mat);
    // Perform transpose of matrices
    Matrix<T> transpose() const;

    // Friend function to multiply  operators without assignment
    friend Matrix<T> operator*(const Matrix<T> &m1, const Matrix<T> &m2) {
        { Matrix<T> ref; return ref.multiply(m1,m2); }
    }
     
    //check if 2 matrices are equal
    friend bool operator==(const Matrix<T> &m1, const Matrix<T> &m2) {
        { Matrix<T> ref; return ref.equals(m1,m2); }
    }

    // () operator to access the value of matrix using indices
    T &operator()(const unsigned &row, const unsigned &col);
    // () operator to access the value of matrix using indices (return const)
    const T &operator()(const unsigned &row, const unsigned &col) const;

    // Row size of the matrix
    unsigned getRowSize() const;
    // Column size of the matrix
    unsigned getColSize() const;
    // Set number of threads
    static void setNumofThread(int _num);
    // Get number of threads
    static int getNumofThread();
    // Function to print the matrix
    void printMatrix();

    // Check if logger is enabled
    static bool isLoggerEnabled();
    // Enable logger
    static void enableLogger();

};

// Include implementation of matrix
#include "matrix.cpp"
#endif