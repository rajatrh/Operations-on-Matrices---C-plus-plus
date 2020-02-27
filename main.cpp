#include "matrix.h"

int main(int argc, char **argv)
{
    // Way to enable logger
    // Matrix<double>::enableLogger();

    // Way to set the number of threads for execution
    Matrix<double>::setNumofThread(3);
    
    // Way to initialise the matrix
    std::cout << "Matrix 1:" << std::endl;
    Matrix<double> mat1("matrix1.txt");
    mat1.printMatrix();

    std::cout << "Matrix 2:" << std::endl;
    Matrix<double> mat2("matrix2.txt");
    mat2.printMatrix();

    // Precomputed result for the multiplication of the above 2 matrices
    std::cout << "Precomputed Product Matrix:" << std::endl;
    Matrix<double> precomputedResult("result.txt");
    precomputedResult.printMatrix();

    // Multiplication  (That internally uses transpose)

    std::cout << "Resultant Matrix:" << std::endl;
    Matrix<double> mat3 = mat1 * mat2;
    mat3.printMatrix();

    /*
    * Not entirely aware of the test suite in C++
    * Hence decided to perform test by comparing computed and precomputed matrices 
    */
    bool res = (mat3 == precomputedResult);

    if (res == 1) {
        cout << "Precomputed result matches the computed matrix.";
    } else {
        cout << "Something's fishy!";
    }
    return 0;
}