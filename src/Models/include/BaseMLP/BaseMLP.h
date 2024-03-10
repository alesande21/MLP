//
// Created by Alesander Britni on 1/13/24.
//
#include "DataStruct/DataNetwork.h"

#ifndef MLP_BASEMLP_H
#define MLP_BASEMLP_H

namespace s21 {
class BaseMLP {
 public:
  virtual void Init(DataNetwork& data) = 0;
  virtual void PrintConf() = 0;
  virtual void SetInput(std::vector<double>& values) = 0;
  virtual int ForwardFeed() = 0;
  virtual void BackPropogation(double expect) = 0;
  virtual void WeightUpdate(double lr, double moment) = 0;
  virtual void SaveWeights(const std::string& file_path) = 0;
  virtual void LoadWeights(const std::string& file_path) = 0;
  virtual std::string GetClassNameMLP() = 0;
  virtual void PrintWeight() = 0;
  virtual void PrintLastNeurons() = 0;
  virtual bool EmptyLayers() = 0;
};
}  // namespace s21

#endif  // MLP_BASEMLP_H
