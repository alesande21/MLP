//
// Created by Alesander Britni on 12/9/23.
//

#include "Matrix/VecMatrix.h"

namespace s21 {

Matrix::Matrix(size_t rows, size_t cols) : rows_(rows), cols_(cols) {
  matrix_.resize(rows_, std::vector<double>(cols_));
}

void Matrix::Resize(size_t rows, size_t cols) {
  rows_ = rows;
  cols_ = cols;
  matrix_.resize(rows_, std::vector<double>(cols_));
}

void Matrix::Multi(const matrix_type& matrix1, const vector_type& neuron,
                   size_t n, vector_type& c) {
  if (matrix1.cols_ != n) throw std::runtime_error("Error Multi \n");
  //#pragma omp parallel for
  //#pragma omp unroll
  for (size_t i = 0; i < matrix1.rows_; ++i) {
    double tmp{};
    for (size_t j = 0; j < matrix1.cols_; ++j) {
      tmp += matrix1[i][j] * neuron[j];
    }
    c[i] = tmp;
  }
}

void Matrix::Multi_T(const matrix_type& matrix1, const vector_type& neuron,
                     size_t n, vector_type& c) {
  if (matrix1.rows_ != n) throw std::runtime_error("Error Multi \n");
  //#pragma omp parallel for
  for (size_t i = 0; i < matrix1.cols_; ++i) {
    double tmp{};
    for (size_t j = 0; j < matrix1.rows_; ++j) {
      tmp = tmp + matrix1[j][i] * neuron[j];
    }
    c[i] = tmp;
  }
}

void Matrix::SumVector(vector_type& a, const vector_type& b, int n) {
  for (int i = 0; i < n; ++i) {
    a[i] += b[i];
  }
}

double Matrix::GetRandom() {
  std::random_device r;
  std::default_random_engine e1(r());
  std::uniform_real_distribution<double> uniform_dist(-0.1, 0.1);
  return uniform_dist(e1);
}

void Matrix::SetRandom(matrix_type& matrix) {
  //#pragma omp parallel for schedule(static)
  for (size_t i = 0; i < matrix.rows_; ++i) {
    for (size_t j = 0; j < matrix.cols_; ++j) {
#if 0
                matrix.matrix_[i][j] = 0.1;
#endif

#if 0
                                matrix[i][j] = GetRandom();
                matrix.matrix_[i][j] = ((rand() % 100)) * 0.03 / (i + 35.);
#endif
      matrix.matrix_[i][j] = Random::GetRandomForWeight(i);
    }
  }
}

void Matrix::SetRandom() {
  for (size_t i = 0; i < rows_; ++i) {
    for (size_t j = 0; j < cols_; ++j) {
#if 0
                matrix_[i][j] = 0.1;
#endif

#if 0
                matrix[i][j] = GetRandom();
                matrix_[i][j] = ((rand() % 100)) * 0.03 / (i + 35.);
#endif
      matrix_[i][j] = Random::GetRandomForWeight(i);
    }
  }
}

void Matrix::Print() {
  std::cout << "Rows: " << rows_ << " - " << matrix_.size()
            << " Cols: " << cols_ << " - " << matrix_[0].size() << std::endl;
}

}  // namespace s21
