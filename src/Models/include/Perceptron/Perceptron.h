//
// Created by Slava on 15.12.2023.
//

#ifndef MLP_PERCEPTRON_H
#define MLP_PERCEPTRON_H

#include <QDebug>
#include <QImage>
#include <QImageReader>
#include <QMetaObject>
#include <QObject>
#include <QTextEdit>
#include <chrono>  // Для использования std::chrono_literals
#include <mutex>
#include <thread>  // Для использования std::this_thread::sleep_for

#include "Clocks/Clocks.h"
#include "Graph/Graph.h"
#include "Logging/Logging.h"
#include "Parser/ParseFile.h"
#include "PerceptronSwitcher/PerceptronSwitcher.h"
//#include "qcustomplot.h"

namespace s21 {

class Perceptron : public QObject {
  Q_OBJECT
 public:
  Perceptron();
  ~Perceptron();

  void SwitchPerceptron(int num);
  void LoadLayers(DataNetwork &data_config);
  void SetEpoch(int value);
  //        void StartTraining(const std::string &filepath);
  void StartTraining(int numb_group, double testing_rate,
                     const std::string &filepath_train,
                     const std::string &filepath_test);
  void StartTesting(const std::string &filepath, double testing_rate, int type,
                    int numb_exp, bool is_enabled_exp);
  void ConvertToImage(QPixmap &scr);
  void AbortTraining();
  //        void RunEdu(QTextEdit &logging, const std::string &filepath);
  void SaveWeight(const std::string &filepath);
  void LoadWeight(const std::string &filepath);
  void LoadAndConvertBmp(const QString &filepath);
  void CrossValidationSwitcher(bool switcher);
  void SetLearningRate(double lr);
  void SetMoment(double moment);
  void SetIsEnableCalcLR(bool is_enable);
  void ActivateTesting(bool is_enabled);
  void SetGraph(Graph *graph);
  void ActivateReport(bool is_enabled);
  bool EmptyLayers();

 signals:
  void SendDataToGraph(QVector<double> x, QVector<double> y);
  void SendSettingsToGraph(int x, int y);
  void SendStatToGraph(double av_accuracy, double precision, double recall,
                       double f_measure, double time_spent);

 private:
  bool abort_training_ = false;
  bool cross_valid_ = false;
  bool is_enabled_clr_ = false;
  bool is_enabled_report_ = false;
  bool is_enabled_test_ = false;
  double lr_{};
  double momentum_{};
  int epoch_{};
  QVector<double> x_, y_;
  Graph *graph_;
  ParseFile parser_;
  BaseMLP *perceptron_;
  PerceptronSwitcher switcher_;
  Logging log_;
  //        QMutex mutex_;

  double GetLearningRate(int epoch);
  void RunTesting(int c_index, int e_index, ParseFile::data_type &data,
                  double testing_rate, int epoch, bool print_log);
  void ReplotGraph(double error, int epoch);
  void SetSettingsToGraph(int epoch);
  double Recall(s21::Matrix::matrix_type &conf_matrix,
                Matrix::matrix_type &stat);
  double AverageAccuracy(Matrix::matrix_type &conf_matrix,
                         Matrix::matrix_type &stat);
  double Precision(s21::Matrix::matrix_type &conf_matrix,
                   Matrix::matrix_type &stat);
  double FMeasure(s21::Matrix::matrix_type &conf_matrix,
                  Matrix::matrix_type &stat);
  void CalcMeasures(double &tp, double &tn, double &fp, double &fn,
                    s21::Matrix::matrix_type &conf_matrix);
  void LetterRecognition(std::vector<double> &input);
  void ConvertToVector(QImage &img);
  void PreExperiment(ParseFile::data_type &data, double testing_rate, int type,
                     int numb_exp);
  void StartExperiment(ParseFile::data_type &data, double testing_rate,
                       int numb_exp);
  void CalcTestStat(Matrix::matrix_type &conf_matrix,
                    Matrix::matrix_type &stat);
  void PrintTestLog(Matrix::matrix_type &stat, long long timer_rdtsc,
                    double timer_std);
  void PrintStatForAllClasses(Matrix::matrix_type &stat);
  void PrintStatByClass(int i, Matrix::matrix_type &stat);
  bool ParcingDataset(const std::string &filepath_train,
                      ParseFile::data_type &data,
                      const std::string &filepath_test,
                      ParseFile::data_type &data_test);
};

}  // namespace s21

#endif  // MLP_PERCEPTRON_H
