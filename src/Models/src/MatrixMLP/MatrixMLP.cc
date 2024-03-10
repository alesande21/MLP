//
// Created by Alesander Britni on 11/20/23.
//

#include "MatrixMLP/MatrixMLP.h"

namespace s21 {

void MatrixMLP::Init(DataNetwork& data) {
  act_func_.Set();
  srand(time(NULL));
  layers_ = data.layers;
  size_.resize(layers_);
  for (size_t i = 0; i < (size_t)layers_; ++i) {
    size_[i] = data.size[i];
  }
  weight_.resize(layers_ - 1);
  weight_err_.resize(layers_ - 1);
  bios_.resize(layers_ - 1);

  for (size_t i = 0; i < (size_t)layers_ - 1; ++i) {
    weight_[i].Resize(size_[i + 1], size_[i]);
    weight_err_[i].Resize(size_[i + 1], size_[i]);
    bios_[i].resize(size_[i + 1]);
    weight_[i].SetRandom();
    for (size_t j = 0; j < (size_t)size_[i + 1]; ++j) {
#if 0
                          bios_[i][j] = GetRandom();
             bios_[i][j] = ((rand() % 50)) * 0.06 / (size_[i] + 15);
#endif

      bios_[i][j] = Random::GetRandomForBios(size_[i]);
    }
#if 0
         weight_[i].Print();
         weight_err_[i].Print();
#endif
  }
  neurons_val_.resize(layers_);
  neurons_err_.resize(layers_);
  bios_err_.resize(layers_);
  for (size_t i = 0; i < (size_t)layers_; ++i) {
    neurons_val_[i].resize(size_[i]);
    neurons_err_[i].resize(size_[i]);
    bios_err_[i].resize(size_[i]);
  }
  neurons_bios_val_.resize(layers_ - 1);
  for (size_t i = 0; i < (size_t)layers_ - 1; ++i) {
    neurons_bios_val_[i] = 1;
  }

#if 0
    PrintWeight();
#endif
}

void MatrixMLP::SetInput(std::vector<double>& values) {
  if (size_.empty()) {
    Logging::LogError(
        "Проблема при загрузки даннных: нет загруженных слоев(size_ пустой)!",
        0, 0, 1);
    return;
  }
  if (values.empty()) {
    Logging::LogError(
        "Проблема при загрузки даннных: нет данных для загрузки(values "
        "пустой)!",
        0, 0, 0);
    return;
  }

  if (neurons_val_.empty()) {
    Logging::LogError(
        "Проблема при загрузки даннных: не сформирована структура нейронов "
        "(neurons_val_ пустой)!",
        0, 0, 0);
    return;
  }

  for (int i = 0; i < size_[0]; ++i) {
    neurons_val_[0][i] = values[i];
  }
}

void MatrixMLP::SetRandom(std::vector<std::vector<double>>& vec) {
  int i = 0;
  for (auto& row : vec) {
    for (auto& col : row) {
#if 0
             col = GetRandom();
#endif

      col = ((rand() % 100)) * 0.03 / (i + 35.);
    }
    ++i;
  }
}

void MatrixMLP::PrintConf() {
  QString size = "";
  for (size_t i = 0; i < (size_t)layers_; ++i) {
    size += " " + QString::number(size_[i]);
  }
  Logging::Log("### Загрузка слоёв... Перцептрон содержит " +
                   QString::number(layers_) + " слоёв: " + size,
               0, 06, 0);
}

int MatrixMLP::ForwardFeed() {
  for (size_t k = 1; k < (size_t)layers_; ++k) {
    s21::Matrix::Multi(weight_[k - 1], neurons_val_[k - 1], size_[k - 1],
                       neurons_val_[k]);

    s21::Matrix::SumVector(neurons_val_[k], bios_[k - 1], size_[k]);

    act_func_.Use(neurons_val_[k], size_[k]);
  }

  int pred = SearchMaxIndex(neurons_val_[layers_ - 1]);
  return pred;
}

int MatrixMLP::SearchMaxIndex(std::vector<double>& value) {
  double max = value[0];
  int prediction = 0;
  double tmp{};
  for (size_t j = 1; j < (size_t)size_[layers_ - 1]; ++j) {
    tmp = value[j];
    if (tmp > max) {
      prediction = j;
      max = tmp;
    }
  }
  return prediction;
}

void MatrixMLP::Multi(const std::vector<std::vector<double>>& matrix1,
                      const std::vector<double>& neuron, int n,
                      std::vector<double>& c) {
  if (matrix1[0].size() != (size_t)n)
    throw std::runtime_error("Error Multi \n");
  //#pragma omp parallel for
  for (int i = 0; i < (int)matrix1.size(); ++i) {
    double tmp{};
    for (int j = 0; j < (int)matrix1[i].size(); ++j) {
      tmp += matrix1[i][j] * neuron[j];
    }
    c[i] = tmp;
  }
}

void MatrixMLP::Multi_T(const std::vector<std::vector<double>>& matrix1,
                        const std::vector<double>& neuron, int n,
                        std::vector<double>& c) {
  if (matrix1.size() != (size_t)n) throw std::runtime_error("Error Multi \n");
  //#pragma omp parallel for
  for (size_t i = 0; i < matrix1[i].size(); ++i) {
    double tmp{};
    for (size_t j = 0; j < matrix1.size(); ++j) {
      tmp = tmp + matrix1[j][i] * neuron[j];
    }
    c[i] = tmp;
  }
}

void MatrixMLP::SumVector(std::vector<double>& a, const std::vector<double>& b,
                          int n) {
  for (int i = 0; i < n; ++i) {
    a[i] += b[i];
  }
}

void MatrixMLP::PrintValues(int layers_) {
  for (size_t j = 0; j < (size_t)size_[layers_]; ++j) {
    std::cout << j << " " << neurons_val_[layers_][j] << std::endl;
  }
}

void MatrixMLP::BackPropogation(double expect) {
  std::vector<double> correct(size_[layers_ - 1]);
  if ((int)expect >= (int)correct.size()) {
    throw std::invalid_argument("Correct is empty");
  }
  correct[(int)expect] = 1;
  //#pragma omp parallel for
  for (int i = 0; i < size_[layers_ - 1]; ++i) {
    neurons_err_[layers_ - 1][i] = (correct[i] - neurons_val_[layers_ - 1][i]) *
                                   neurons_val_[layers_ - 1][i] *
                                   (1 - neurons_val_[layers_ - 1][i]);
  }

#if 0  // bias update
        for (int i = 0; i < size_[layers_ - 1]; ++i) {
            bios_err_[layers_ - 1][i] = 2 * (neurons_val_[layers_ - 1][i] - correct[i]) *  neurons_val_[layers_ - 1][i] * (1 -  neurons_val_[layers_ - 1][i]);
        }

        for (int k = layers_ - 2; k >= 0; --k) {
            for (int i = 0; i < size_[k]; ++i) {
                double tmp{};
                for (int j = 0; j < size_[k + 1]; ++j) {
                    tmp += weight_[k][j][i] * neurons_err_[k + 1][j];
                }
                neurons_err_[k][i] = tmp * neurons_val_[k][i] * (1 -  neurons_val_[k][i]);
            }
        }
#endif
  for (int k = layers_ - 2; k >= 0; --k) {
    if (k > layers_) {
      throw std::invalid_argument("Invalid parametr layer");
    }
    for (int i = 0; i < size_[k]; ++i) {
      double tmp{};
      for (int j = 0; j < size_[k + 1]; ++j) {
        tmp += weight_[k][j][i] * neurons_err_[k + 1][j];
      }
      neurons_err_[k][i] = tmp * neurons_val_[k][i] * (1 - neurons_val_[k][i]);
    }
  }
}

void MatrixMLP::WeightUpdate(double lr, double moment) {
  for (int k = layers_ - 2; k >= 0; --k) {
    for (int i = 0; i < size_[k + 1]; ++i) {
      double tmp{};
      for (int j = 0; j < size_[k]; ++j) {
        weight_err_[k][i][j] *= moment;
        tmp = neurons_val_[k][j] * neurons_err_[k + 1][i] * lr +
              weight_err_[k][i][j];
        weight_[k][i][j] += tmp;
        weight_err_[k][i][j] = tmp;
      }
    }
  }

  for (int i = 0; i < layers_ - 1; ++i) {
    for (int k = 0; k < size_[i + 1]; ++k) {
      bios_[i][k] += neurons_err_[i + 1][k] * lr;
    }
  }
}

void MatrixMLP::SaveWeights(const std::string& file_path) {
  if (weight_.empty()) return;
  std::ofstream out;
  out.open(file_path);
  if (!out.is_open()) {
#if 1
    Logging::LogError(
        "Ошибка открытия файла: " + QString::fromStdString(file_path), 0, 0, 1);
#endif
    out.close();
    return;
  }

  for (int i = 0; i < (int)size_.size(); ++i) {
    out << size_[i] << " ";
  }
  out << std::endl;

  for (int l = 0; l < (int)size_.size() - 1; ++l) {
    for (int i = 0; i < size_[l + 1]; ++i) {
      for (int j = 0; j < size_[l]; ++j) {
        double val = weight_[l][i][j];
        out << val << " ";
      }
      out << std::endl;
    }
    out << std::endl;
  }
}

void MatrixMLP::LoadWeights(const std::string& file_path) {
  std::ifstream file(file_path);
  if (!file.is_open()) {
#if 1
    Logging::LogError(
        "Ошибка открытия файла: " + QString::fromStdString(file_path), 0, 0, 1);
    return;
#endif
  }
  // ClearData();
  std::string line;
  std::getline(file, line);
  if (!ReadSizeLayers(line)) return;

  PrintConf();
  double val{};
  for (int k = 0; k < (int)weight_.size(); ++k) {
    for (int i = 0; i < (int)weight_[k].Rows();) {
      if (!std::getline(file, line)) {
        std::cerr << "Веса загрузились не полностью! " << std::endl;
        break;
      }
      if (line.empty()) continue;
      std::istringstream iss(line);
      int j = 0;
      while (iss >> val && j < (int)weight_[k].Cols()) {
        weight_[k][i][j] = val;
        ++j;
      }
      ++i;
    }
  }
}

void MatrixMLP::ReadLine(const std::string& line, size_t l) {
  std::istringstream iss(line);
  std::vector<int> vec;
  double val{};
  for (int i = 0; i < (int)weight_[l].Rows(); ++i) {
    for (int j = 0; j < (int)weight_[l].Cols(); ++j) {
      if (!(iss >> val)) {
        return;
      }
    }

    vec.push_back(val);
  }
}

bool MatrixMLP::ReadSizeLayers(const std::string& line) {
  std::istringstream iss(line);
  //     iss.ignore(std::numeric_limits<std::streamsize>::max(), ' ');
  size_t size{};
  DataNetwork data;
  while (iss >> size) {
    std::cout << size << std::endl;
    data.size.push_back((int)size);
  }
  data.layers = (int)data.size.size();
  Init(data);
  return true;
}

std::string MatrixMLP::GetClassNameMLP() { return " матричный "; }

void MatrixMLP::SetSizeLayers([[maybe_unused]] DataNetwork& data) {}

void MatrixMLP::ClearLayers() {}

void MatrixMLP::PrintWeight() {
  std::cout << "Print weight ver matrix: " << std::endl;
  std::cout << "Layer: 0" << std::endl;
  std::cout << "Neuron: ";
  for (int i = 0; i < size_[0]; ++i) {
    std::cout << neurons_val_[0][i] << " ";
  }
  std::cout << std::endl;
  std::cout << std::endl;
  for (int l = 0; l < (int)size_.size() - 1; ++l) {
    //         std::cout << "l: " << weight_[l].size() << " ";
    std::cout << "Layer: " << l + 1 << std::endl;
    for (int i = 0; i < (int)weight_[l].Rows(); ++i) {
      //            std::cout << weight_[l][i].size() << " ";
      for (int j = 0; j < (int)weight_[l].Cols(); ++j) {
        std::cout << weight_[l][i][j] << " ";
      }
      std::cout << std::endl;
    }
    std::cout << "Neurons: " << size_[l + 1] << " :  ";
    for (int n = 0; n < size_[l + 1]; ++n) {
      std::cout << neurons_val_[l + 1][n] << " ";
    }
    std::cout << std::endl;
    std::cout << "Bios: " << size_[l + 1] << " :  ";
    for (int n = 0; n < size_[l + 1]; ++n) {
      std::cout << bios_[l][n] << " ";
    }
    std::cout << std::endl;
    std::cout << std::endl;
  }
  std::cout << std::endl;
  std::cout << std::endl;
}

void MatrixMLP::PrintLastNeurons() {
  std::cout << "PrintLastNeurons: " << std::endl;
  std::cout << "    Neurons: ";
  for (int n = 0; n < size_[layers_ - 1]; ++n) {
    std::cout << neurons_val_[layers_ - 1][n] << " ";
  }
  std::cout << std::endl;
}

bool MatrixMLP::EmptyLayers() { return size_.empty(); }

}  // namespace s21
