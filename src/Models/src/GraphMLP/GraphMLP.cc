//
// Created by Alesander Britni on 12/19/23.
//

#include "GraphMLP/GraphMLP.h"

namespace s21 {

void GraphMLP::Init(DataNetwork &data) {
  act_func_.Set();
  srand(time(NULL));
  numb_layers_ = data.layers;
  size_.resize(numb_layers_);

  for (size_t i = 0; i < (size_t)numb_layers_; ++i) {
    size_[i] = data.size[i];
  }
  layers_.resize(numb_layers_);
  layers_[0].SetSizeNeurons(size_[0], 1);
  for (int i = 1; i < numb_layers_; ++i) {
    layers_[i].SetSizeNeurons(size_[i], size_[i - 1]);
  }

#if 0
        std::cout << "Why don't you print?" << std::endl;
        for (int i = 1; i < numb_layers_; ++i) {
            layers_[i].Print(i);
            std::cout << std::endl;
            std::cout << std::endl;
        }
#endif
}

void GraphMLP::SetInput(std::vector<double> &values) {
  layers_[0].SetInput(values);
}

int GraphMLP::ForwardFeed() {
  for (int i = 1; i < numb_layers_; ++i) {
    layers_[i].RecalculateNeurons(layers_[i - 1].GetNeurons(), act_func_);
  }
  int pred = layers_[numb_layers_ - 1].SearchMaxIndex();
  return pred;
}

void GraphMLP::BackPropogation(double expect) {
  layers_[numb_layers_ - 1].BPForLastLayer(expect);
  for (int i = numb_layers_ - 2; i > 0; --i) {
    layers_[i].BPForOtherLayers(layers_[i + 1].GetNeurons());
  }
}

void GraphMLP::WeightUpdate(double lr, double moment) {
  for (int i = numb_layers_ - 1; i > 0; --i) {
    layers_[i].WeightUpdate(lr, moment, layers_[i - 1].GetNeurons());
  }
  for (int i = 1; i < numb_layers_; ++i) {
    layers_[i].BiosUpdate(lr);
  }
}

void GraphMLP::PrintConf() {
  QString size = "";
  for (size_t i = 0; i < (size_t)numb_layers_; ++i) {
    size += " " + QString::number(size_[i]);
  }
  Logging::Log("### Загрузка слоёв... Перцептрон содержит " +
                   QString::number(numb_layers_) + " слоёв: " + size,
               0, 06, 0);
}

void GraphMLP::SaveWeights(const std::string &file_path) {
  if (layers_.empty()) return;
  std::ofstream out;
  out.open(file_path);
  if (!out.is_open()) {
#if 1
    Logging::LogError(
        "Ошибка открытия файла: " + QString::fromStdString(file_path), 0, 0, 1);
    return;
#endif
    out.close();
    return;
  }

  for (int i = 0; i < (int)size_.size(); ++i) {
    out << size_[i] << " ";
  }
  out << std::endl;

  for (int i = 1; i < numb_layers_; ++i) {
    layers_[i].SaveWeight(out);
    out << std::endl;
  }
}

void GraphMLP::LoadWeights(const std::string &file_path) {
  std::ifstream file(file_path);
  if (!file.is_open()) {
#if 1
    Logging::LogError(
        "Ошибка открытия файла: " + QString::fromStdString(file_path), 0, 0, 1);
    return;
#endif
  }
  std::string line;
  std::getline(file, line);
  if (!ReadSizeLayers(line)) return;
#if 1
  PrintConf();
#endif
  for (int i = 1; i < numb_layers_; ++i) {
    layers_[i].LoadWeight(file);
  }
}

std::string GraphMLP::GetClassNameMLP() { return " графовый "; }

bool GraphMLP::ReadSizeLayers(const std::string &line) {
  std::istringstream iss(line);
  size_t size{};
  DataNetwork data;
  while (iss >> size) {
    data.size.push_back((int)size);
  }
  data.layers = (int)data.size.size();
  Init(data);
  return true;
}

void GraphMLP::PrintWeight() {
  std::cout << "Print Weight: " << std::endl;
  for (int i = 0; i < numb_layers_; ++i) {
    layers_[i].Print(i);
    std::cout << std::endl;
  }
}

void GraphMLP::PrintLastNeurons() {
  std::cout << "PrintLastNeurons: " << std::endl;
  layers_[numb_layers_ - 1].PrintLastNeurons();
}

bool GraphMLP::EmptyLayers() { return size_.empty(); }

}  // namespace s21
