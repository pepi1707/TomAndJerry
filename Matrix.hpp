#include "Matrix.h"

template <class T>
Matrix<T>::Matrix(){

}

template <class T>
Matrix<T>::Matrix(const int& m, const int& n){
    matrix.assign(m, std::vector<T>(n));
}

template <class T>
Matrix<T>::Matrix(const int& m, const int& n, const T& val){
    matrix.assign(m, {});
    for(int i = 0; i < m; i ++){
        matrix[i].assign(n, val);
    }
}

template <class T>
std::vector<T>& Matrix<T>::operator[](const int& idx){
    assert(idx >= 0 && idx < matrix.size());
    return matrix[idx];
}

template <class T>
T& Matrix<T>::operator[](const pair<int, int>& idx){
    assert(idx.first >= 0 && idx.first < matrix.size() && idx.second >= 0 && idx.second <matrix[idx.first].size());
    return matrix[idx.first][idx.second];
}

template <class T>
void Matrix<T>::build(const int& m, const int& n){
    matrix.assign(m, std::vector<T>(n));
}

template <class T>
void Matrix<T>::build(const int& m, const int& n, const T& val){
    matrix.assign(m, {});
    for(int i = 0; i < m; i ++){
        matrix[i].assign(n, val);
    }
}