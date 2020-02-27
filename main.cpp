#include "matrix.h"

int main(int argc, char **argv)
{
    // Matrix<double> mat1(10, 10, 1.0);
    // Matrix<double> mat2(10, 10, 3.0);

    Matrix<double> mat1("matrix1.txt");

    mat1.printMatrix();

    Matrix<double> mat2("matrix2.txt");

    mat2.printMatrix();

    Matrix<double> mat3 = mat1 * mat2;
    mat3.printMatrix();
    return 0;
}