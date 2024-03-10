//
// Created by Alesander Britni on 1/25/24.
//

#include "GraphMLP/Layer.h"

namespace s21 {

void Layer::SetSizeNeurons(int row, int col) {
  if (row < 1 || col < 1) return;
  neurons_.resize(row);
  size_ = row;
  for (int i = 0; i < row; ++i) {
    neurons_[i].ResizeWeight(col);
    neurons_[i].ResizeWeightError(col);
    neurons_[i].SetRandom(i);
    neurons_[i].SetRandomBios(col);
  }
}

void Layer::RecalculateNeurons(std::vector<Neuron> &prev_layer,
                               ActivateFunction &act_func) {
  for (int i = 0; i < (int)neurons_.size(); ++i) {
    neurons_[i].MultiplyByWeight(prev_layer);
    neurons_[i].SumWithBios();
    neurons_[i].ActivateFunc(act_func);
  }
}

int Layer::SearchMaxIndex() {
  double max = neurons_[0].GetValue();
  int prediction = 0;
  double tmp{};
  for (int i = 0; i < (int)neurons_.size(); ++i) {
    tmp = neurons_[i].GetValue();
    if (tmp > max) {
      prediction = i;
      max = tmp;
    }
  }
  return prediction;
}

void Layer::SetInput(std::vector<double> &values) {
  if (neurons_.size() != values.size()) {
    throw std::invalid_argument("neurons_.size() != values.size()");
  }
  for (int i = 0; i < (int)values.size(); ++i) {
    neurons_[i].GetValue() = values[i];
  }
}

void Layer::BPForLastLayer(double expect) {
  std::vector<double> correct(neurons_.size());
  if ((int)expect >= (int)correct.size()) {
    throw std::invalid_argument("Correct is empty");
  }
  correct[(int)expect] = 1;
  for (int i = 0; i < (int)neurons_.size(); ++i) {
    neurons_[i].CalculateError(correct[i]);
  }
}

void Layer::BPForOtherLayers(std::vector<Neuron> &next_layer) {
  for (int i = 0; i < (int)neurons_.size(); ++i) {
    double tmp{};
    for (int j = 0; j < (int)next_layer.size(); ++j) {
      tmp += next_layer[j].GetValueWeight(i) * next_layer[j].GetError();
    }
    neurons_[i].GetError() =
        tmp * neurons_[i].GetValue() * (1 - neurons_[i].GetValue());
  }
}

void Layer::WeightUpdate(double lr, double moment,
                         std::vector<Neuron> &prev_layer) {
  for (int i = 0; i < (int)neurons_.size(); ++i) {
    neurons_[i].UpdateWeight(lr, moment, prev_layer[i].GetValue(), prev_layer);
  }
}

void Layer::BiosUpdate(double lr) {
  for (int i = 0; i < (int)neurons_.size(); ++i) {
    neurons_[i].BiosUpdate(lr);
  }
}

void Layer::SaveWeight(std::ofstream &out) {
  for (int i = 0; i < (int)neurons_.size(); ++i) {
    neurons_[i].SaveWeight(out);
    out << std::endl;
  }
}

void Layer::LoadWeight(std::ifstream &file) {
  for (int i = 0; i < (int)neurons_.size();) {
    std::string line = "";
    if (!std::getline(file, line)) {
      std::cerr << "Веса загрузились не полностью! " << std::endl;
      return;
    }
    if (!line.empty()) {
      neurons_[i].LoadWeight(line);
      ++i;
    }
  }
}

void Layer::PrintLastNeurons() {
  std::cout << "Neurons: ";
  for (int i = 0; i < (int)neurons_.size(); ++i) {
    std::cout << neurons_[i].GetValue() << " ";
  }
  std::cout << std::endl;
}

void Layer::Print(int l) {
  std::cout << "Layer: " << l << std::endl;
  for (int i = 0; i < (int)neurons_.size(); ++i) {
    neurons_[i].Print();
    std::cout << std::endl;
  }
#if 1
  std::cout << "Neurons: ";
  for (int i = 0; i < (int)neurons_.size(); ++i) {
    std::cout << neurons_[i].GetValue() << " ";
  }
  std::cout << std::endl;

  std::cout << "Bios: ";
  for (int i = 0; i < (int)neurons_.size(); ++i) {
    std::cout << neurons_[i].GetBios() << " ";
  }
  std::cout << std::endl;
#endif
}

int Layer::Size() { return neurons_.size(); }

std::vector<Neuron> &Layer::GetNeurons() { return neurons_; }

}  // namespace s21
