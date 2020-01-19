#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <cassert>

template<class T>
class Matrix{

    std::vector<std::vector<T>> matrix;

public:
    Matrix();
    
    Matrix(const int& m, const int& n);

    Matrix(const int& m, const int& n, const T& val);

    std::vector<T>& operator[](const int& idx);

    T& operator[](const pair<int, int>& idx);

    void build(const int& m, const int& n);

    void build(const int& m, const int& n, const T& val);


};

#include "Matrix.hpp"


#endif