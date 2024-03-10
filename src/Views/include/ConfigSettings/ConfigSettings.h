#ifndef CONFIGSETTINGS_H
#define CONFIGSETTINGS_H

#include <QCoreApplication>
#include <QFileDialog>
#include <QLayoutItem>
#include <QOpenGLFunctions>
#include <QSpinBox>
#include <QThread>
#include <QVBoxLayout>
#include <QWidget>
#include <QtConcurrent/QtConcurrent>

#include "Controller.h"

namespace Ui {
class ConfigSettings;
}

namespace s21 {
class ConfigSettings : public QWidget {
  Q_OBJECT

 public:
  ConfigSettings() = delete;
  explicit ConfigSettings(Controller &controller, s21::DataNetwork &data_config,
                          QWidget *parent = nullptr);
  ConfigSettings(ConfigSettings &other) = delete;
  ConfigSettings(ConfigSettings &&other) = delete;
  ConfigSettings &operator=(const ConfigSettings other) = delete;
  ConfigSettings &operator=(ConfigSettings &&other) = delete;
  ~ConfigSettings();
  QString GetFilePath();
  int GetNumbGroups();
  void OnOffWeightsSettings(bool value);
  bool LoadFromSettings();
  bool TesingModeOn();
  void SetSettingsToModel();

  void OnOffAllSettings(bool value);

  void RunTesting();
  void SetIsEnabledReport();
  void StartTraining();

 private:
  Ui::ConfigSettings *ui_;
  DataNetwork &data_config_;
  Controller &controller_;
  int epoch_{};
  int hl_{};
  std::vector<QSpinBox *> spin_boxes_;

 private slots:
  void SetLearningRate();
  void SetMoment();
  void SetEpoch();
  void SetHiddenLayers();
  void SetLayers();
  void LoadFilepath();
  void LoadFilepathForTest();
  void SetFormulaLr();
  void SetReport();
  void SetMatrixRealisation();
  void SetGraphRealisation();
  void SetTypeFunction();

  void OnOffLayersSettings(bool value);
  void OnOffTeachingSettings(bool value);
  void OnOffTestingSettings(bool value);
  void OnOffTypeFunction(bool value);

  void TestingModeSwitcher();
  void CVModeSwitcher();
  void TrainingSwitcher();
  void TrainingTestingSwitcher();

  void SetExperiment();
};
}  // namespace s21

#endif  // CONFIGSETTINGS_H
