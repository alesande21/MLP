//
// Created by Alesander Britni on 11/20/23.
//

#ifndef MLP_CONTROLLER_H
#define MLP_CONTROLLER_H

#include <QTextEdit>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <thread>
#include <vector>

#include "MatrixMLP/MatrixMLP.h"
#include "Parser/ParseFile.h"
#include "Perceptron/Perceptron.h"
#include "qcustomplot.h"

namespace s21 {
class Controller {
 public:
  Controller() = delete;
  Controller(Perceptron &splicer);
  Controller(Controller &other) = delete;
  Controller(Controller &&other) = delete;
  Controller &operator=(const Controller other) = delete;
  Controller &operator=(Controller &&other) = delete;

  void SetTypePerceptron(int num);
  void StartTraining(int numb_group, double testing_rate,
                     const std::string &filepath_train,
                     const std::string &filepath_test);
  void StartTesting(const std::string &filepath, double testing_rate, int type,
                    int numb_exp, bool is_enabled_exp);
  void LetterRecognition(QPixmap &scr);
  void SaveWeight(const std::string &filepath);
  void LoadWeight(const std::string &filepath);
  void LoadLayers(DataNetwork &data_config);
  void LoadAndConvertBmp(const QString &filepath);
  void AbortTraining();
  void SetEpoch(int value);
  void SetLearningRate(double lr);
  void SetMoment(double moment);
  void SetIsEnabledCalcLR(bool is_enabled);
  void SetIsEnabledReport(bool is_enabled);
  void SetIsEnabledTesting(bool is_enabled);
  void SetEnabledCV(bool is_enabled);
  void SetGraph(Graph *graph);
  bool CheckEmptyLayers();

 private:
  Perceptron &perceptron_;
};
}  // namespace s21

#endif  // MLP_CONTROLLER_H
