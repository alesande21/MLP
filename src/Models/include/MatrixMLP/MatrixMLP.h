//
// Created by Alesander Britni on 11/20/23.
//

#ifndef MLP_NETWORK_H
#define MLP_NETWORK_H

#include <ctime>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

#include "ActivateFunction/ActivateFunction.h"
#include "BaseMLP/BaseMLP.h"
#include "DataStruct/DataNetwork.h"
#include "Logging/Logging.h"
#include "Matrix/VecMatrix.h"

namespace s21 {
class MatrixMLP : public BaseMLP {
  using matrix_type = s21::Matrix::matrix_type;
  using vector_type = s21::Matrix::vector_type;

 public:
  MatrixMLP() = default;
  void Init(DataNetwork& data) override;
  void PrintConf() override;
  void SetInput(std::vector<double>& values) override;
  int ForwardFeed() override;
  void BackPropogation(double expect) override;
  void WeightUpdate(double lr, double moment) override;
  void SaveWeights(const std::string& file_path) override;
  void LoadWeights(const std::string& file_path) override;
  std::string GetClassNameMLP() override;
  void PrintWeight() override;
  void PrintLastNeurons() override;
  bool EmptyLayers() override;
  virtual ~MatrixMLP(){};

 private:
  int layers_;
  std::vector<int> size_;
  ActivateFunction act_func_;
  std::vector<matrix_type> weight_;
  std::vector<matrix_type> weight_err_;
  std::vector<vector_type> bios_;
  std::vector<vector_type> neurons_val_;
  std::vector<vector_type> neurons_err_;
  std::vector<vector_type> bios_err_;
  std::vector<double> neurons_bios_val_;

  void ClearLayers();
  bool ReadSizeLayers(const std::string& line);
  void ReadLine(const std::string& line, size_t layer);
  void SetSizeLayers(DataNetwork& data);
  void SetRandom(std::vector<std::vector<double>>& vec);

  int SearchMaxIndex(std::vector<double>& value);
  void PrintValues(int L);
  void Multi(const std::vector<std::vector<double>>& matrix1,
             const std::vector<double>& neuron, int n, std::vector<double>& c);
  void Multi_T(const std::vector<std::vector<double>>& matrix1,
               const std::vector<double>& neuron, int n,
               std::vector<double>& c);
  void SumVector(std::vector<double>& a, const std::vector<double>& b, int n);
};

}  // namespace s21

#endif  // MLP_NETWORK_H
