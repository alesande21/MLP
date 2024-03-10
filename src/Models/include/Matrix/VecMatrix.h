//
// Created by Alesander Britni on 12/9/23.
//

#ifndef MLP_VECMATRIX_H
#define MLP_VECMATRIX_H

#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <random>
#include <stdexcept>
#include <vector>

#include "Random/Random.h"

namespace s21 {
class Matrix {
 public:
  using matrix_type = s21::Matrix;
  using vector_type = std::vector<double>;

  Matrix() = default;
  Matrix(size_t rows, size_t cols);
  Matrix(const Matrix& other) = default;
  Matrix(Matrix&& other) = default;
  ~Matrix() = default;
  Matrix& operator=(const Matrix& other) = default;
  Matrix& operator=(Matrix&& other) = default;
  std::vector<double>& operator[](size_t row) { return matrix_[row]; }
  const std::vector<double>& operator[](size_t row) const {
    return matrix_[row];
  }
  //        double &operator[](size_t col) { return [col]; }
  //        double &operator[](std::vector<double> a, size_t col) { return
  //        a[col];}
  inline size_t Rows() const { return rows_; }
  inline size_t Cols() const { return cols_; }
  void Resize(size_t rows, size_t cols);
  static void Multi(const matrix_type& matrix1, const vector_type& neuron,
                    size_t n, vector_type& c);
  static void Multi_T(const matrix_type& matrix1, const vector_type& neuron,
                      size_t n, vector_type& c);
  static void SumVector(vector_type& a, const vector_type& b, int n);
  static void SetRandom(matrix_type& vec);
  void SetRandom();
  static double GetRandom();
  void Print();

 private:
  size_t rows_{}, cols_{};
  std::vector<std::vector<double>> matrix_;
};

}  // namespace s21

#endif  // MLP_VECMATRIX_H
