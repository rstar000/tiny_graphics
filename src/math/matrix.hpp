/*
 * =====================================================================================
 *
 *       Filename:  matrix.hpp
 *
 *    Description: Matrix data structure
 *
 *        Version:  1.0
 *        Created:  19.09.2018 21:59:06
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Fedor Novikov (rstar000)
 *   Organization
 *
 * =====================================================================================
 */
#ifndef MATRIX_HPP
#define MATRIX_HPP
#include "vector.hpp"

namespace tmath
{
// n rows * m cols
template <int N, int M>
class Matrix
{
    std::array<Vector<M>, N> rows;
public:
    Matrix() { }
    Matrix(const Matrix &other) : rows(other.rows) { }
    Matrix(const Matrix &&other) : rows(std::move(other.rows)) { }
    Matrix(const std::array<Vector<M>, N> &rows) : rows(rows) { }

    Vector<M>& row(unsigned i) { return rows[i]; }
    Vector<M> row(unsigned i) const { return rows[i]; }
    Vector<M>& operator[] (unsigned i) { return rows[i]; }
    Vector<M> operator[]  (unsigned i) const { return rows[i]; }
    float& operator() (unsigned i, unsigned j) { return rows[i][j]; }
    float operator() (unsigned i, unsigned j) const { return rows[i][j]; }


    Matrix& operator=(Matrix& other) {
        rows = other.rows;
    }

    Matrix& operator=(Matrix&& other) {
        rows = std::move(other.rows);
    }

    Vector<N> column(unsigned j) const {
        Vector<N> res;
        for(int i = 0; i < N; i++) {
            res[i] = rows[i][j];
        }
        return res;
    }
};

// Matrix operations
template<int N, int M>
inline std::ostream& operator<< (std::ostream &os, const Matrix<N,M> &matr)
{
    os << "Matrix " << N << 'x' << M << "\n";
    for(int i = 0; i < N; i++) os << matr[i] << std::endl;
    return os;
}

template<int N, int M>
inline Vector<N> operator* (const Matrix<N, M> &m, const Vector<M> &v)
{
    Vector<N> result;
    for(unsigned i = 0; i < N; i++) {
        result[i] = dot(v, m[i]);
    }
    return result;
}

template<int N, int M, int K>
inline Matrix<N, K> operator* (const Matrix<N, M> &left, const Matrix<M, K> &right)
{
    Matrix<N, K> result;
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < K; j++) {
            result(i,j) = dot(left.row(i), right.column(j));
        }
    }
    return result;
}

template<int N, int M>
inline Matrix<M, N> transpose(const Matrix<N, M> &m)
{
    Matrix<M, N> result;
    for(int i = 0; i < M; i++) {
        for(int j = 0; j < N; j++) {
            result(i,j) = m(j,i);
        }
    }
    return result;
}

using Mat4 = Matrix<4,4>;
using Mat3 = Matrix<3,3>;
}


#endif
