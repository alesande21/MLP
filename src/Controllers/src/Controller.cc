//
// Created by Alesander Britni on 11/20/23.
//

#include "Controller.h"

namespace s21 {

Controller::Controller(Perceptron &perceptron) : perceptron_(perceptron){};

void Controller::SetTypePerceptron(int num) {
  perceptron_.SwitchPerceptron(num);
}

void Controller::LoadLayers(DataNetwork &data_config) {
  perceptron_.LoadLayers(data_config);
}

void Controller::LoadAndConvertBmp(const QString &filepath) {
  perceptron_.LoadAndConvertBmp(filepath);
}

void Controller::StartTraining(int numb_group, double testing_rate,
                               const std::string &filepath_train,
                               const std::string &filepath_test) {
  perceptron_.StartTraining(numb_group, testing_rate, filepath_train,
                            filepath_test);
}

void Controller::StartTesting(const std::string &filepath, double testing_rate,
                              int type, int numb_exp, bool is_enabled_exp) {
  perceptron_.StartTesting(filepath, testing_rate, type, numb_exp,
                           is_enabled_exp);
}

void Controller::LetterRecognition(QPixmap &scr) {
  perceptron_.ConvertToImage(scr);
}

void Controller::SaveWeight(const std::string &filepath) {
  perceptron_.SaveWeight(filepath);
}

void Controller::LoadWeight(const std::string &filepath) {
  perceptron_.LoadWeight(filepath);
}

void Controller::SetEpoch(int value) { perceptron_.SetEpoch(value); }

void Controller::AbortTraining() { perceptron_.AbortTraining(); }

void Controller::SetLearningRate(double lr) { perceptron_.SetLearningRate(lr); }

void Controller::SetMoment(double moment) { perceptron_.SetMoment(moment); }

void Controller::SetEnabledCV(bool is_enabled) {
  perceptron_.CrossValidationSwitcher(is_enabled);
}

void Controller::SetIsEnabledCalcLR(bool is_enabled) {
  perceptron_.SetIsEnableCalcLR(is_enabled);
}

void Controller::SetIsEnabledReport(bool is_enabled) {
  perceptron_.ActivateReport(is_enabled);
}

void Controller::SetIsEnabledTesting(bool is_enabled) {
  perceptron_.ActivateTesting(is_enabled);
}

void Controller::SetGraph(Graph *graph) { perceptron_.SetGraph(graph); }

bool Controller::CheckEmptyLayers() { return perceptron_.EmptyLayers(); }

}  // namespace s21
