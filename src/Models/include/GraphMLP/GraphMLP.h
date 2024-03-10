//
// Created by Alesander Britni on 12/19/23.
//

#ifndef MLP_GRAPHMLP_H
#define MLP_GRAPHMLP_H

#include <QString>
#include <ctime>
#include <fstream>
#include <sstream>

#include "ActivateFunction/ActivateFunction.h"
#include "BaseMLP/BaseMLP.h"
#include "DataStruct/DataNetwork.h"
#include "Layer.h"
#include "Logging/Logging.h"
#include "Matrix/VecMatrix.h"

namespace s21 {
class GraphMLP : public BaseMLP {
 public:
  using matrix_type = s21::Matrix::matrix_type;
  using vector_type = s21::Matrix::vector_type;
  void Init(DataNetwork &data) override;
  int ForwardFeed() override;
  void SetInput(std::vector<double> &values) override;
  void BackPropogation(double expect) override;
  void WeightUpdate(double lr, double moment) override;
  void PrintConf() override;
  void PrintWeight() override;
  void SaveWeights(const std::string &file_path) override;
  void LoadWeights(const std::string &file_path) override;
  std::string GetClassNameMLP() override;
  void PrintLastNeurons() override;
  bool EmptyLayers() override;
  virtual ~GraphMLP(){};

 private:
  std::vector<matrix_type> weight_;
  std::vector<matrix_type> weight_err_;
  std::vector<int> size_;
  ActivateFunction act_func_;
  int numb_layers_{};
  std::vector<Layer> layers_;

  bool ReadSizeLayers(const std::string &line);
};

}  // namespace s21

#endif  // MLP_GRAPHMLP_H
