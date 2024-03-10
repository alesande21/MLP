#include "ConfigSettings/ConfigSettings.h"

#include "ui_configsettings.h"

namespace s21 {

ConfigSettings::ConfigSettings(Controller &controller,
                               s21::DataNetwork &data_config, QWidget *parent)
    : QWidget(parent),
      ui_(new Ui::ConfigSettings),
      data_config_(data_config),
      controller_(controller) {
  ui_->setupUi(this);
  setWindowTitle("Configuration options");
  SetHiddenLayers();
  SetEpoch();
  OnOffWeightsSettings(false);
  OnOffTestingSettings(false);
  OnOffTypeFunction(false);
#if WIN32
  ui_->to_filepath->addItem(
      "C:/Users/Slava/Desktop/CurrentProject/MLP/datasets/emnist-letters/"
      "emnist-letters-train.csv");
  ui_->to_filepath_for_test->addItem(
      "C:/Users/Slava/Desktop/CurrentProject/MLP/datasets/emnist-letters/"
      "emnist-letters-test.csv");
#else
  ui_->to_filepath->addItem(
      "/opt/goinfre/alesande/emnist-letters/emnist-letters-train.csv");
  ui_->to_filepath_for_test->addItem(
      "/opt/goinfre/alesande/emnist-letters/emnist-letters-test.csv");
#endif
  connect(ui_->epoch, QOverload<int>::of(&QSpinBox::valueChanged), this,
          &ConfigSettings::SetEpoch);
  connect(ui_->hidden_layers, QOverload<int>::of(&QSpinBox::valueChanged), this,
          &ConfigSettings::SetHiddenLayers);
  connect(ui_->learning_rate,
          QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          &ConfigSettings::SetLearningRate);
  connect(ui_->moment, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
          this, &ConfigSettings::SetMoment);

  connect(ui_->load_filepath, QOverload<bool>::of(&QPushButton::clicked), this,
          &ConfigSettings::LoadFilepath);

  connect(ui_->load_filepath_for_test,
          QOverload<bool>::of(&QPushButton::clicked), this,
          &ConfigSettings::LoadFilepathForTest);

  connect(ui_->use_lr_formula, QOverload<bool>::of(&QCheckBox::clicked), this,
          &ConfigSettings::SetFormulaLr);

  connect(ui_->make_report, QOverload<bool>::of(&QCheckBox::clicked), this,
          &ConfigSettings::SetReport);

  connect(ui_->matrix, QOverload<bool>::of(&QRadioButton::clicked), this,
          &ConfigSettings::SetMatrixRealisation);

  connect(ui_->graph, QOverload<bool>::of(&QRadioButton::clicked), this,
          &ConfigSettings::SetGraphRealisation);

  connect(ui_->layers_from_settings, &QRadioButton::clicked, this,
          [this]() { OnOffLayersSettings(true); });

  connect(ui_->layers_from_file, &QRadioButton::clicked, this,
          [this]() { OnOffLayersSettings(false); });

  connect(ui_->type_function,
          QOverload<int>::of(&QComboBox::currentIndexChanged), this,
          &ConfigSettings::SetTypeFunction);

  connect(ui_->testing_mode, QOverload<bool>::of(&QRadioButton::toggled), this,
          &ConfigSettings::TestingModeSwitcher);

  connect(ui_->cv_mode, QOverload<bool>::of(&QRadioButton::toggled), this,
          &ConfigSettings::CVModeSwitcher);

  connect(ui_->training_mode, QOverload<bool>::of(&QRadioButton::toggled), this,
          &ConfigSettings::TrainingSwitcher);

  connect(ui_->training_plus_testing,
          QOverload<bool>::of(&QRadioButton::toggled), this,
          &ConfigSettings::TrainingTestingSwitcher);

  connect(ui_->make_exp, QOverload<bool>::of(&QCheckBox::clicked), this,
          &ConfigSettings::SetExperiment);

#if 0
  connect(ui_->to_filepath, QOverload<int>::of(&QComboBox::activated),
          this, &ConfigSettings::LoadFilepath);
  connect(ui_->gen_lab, QOverload<bool>::of(&QPushButton::clicked), this,
          &ConfigSettings::);
  connect(ui_->create_path, QOverload<bool>::of(&QPushButton::clicked), this,
          &MazeSettings::);
#endif
}

ConfigSettings::~ConfigSettings() { delete ui_; }

void ConfigSettings::SetEpoch() { controller_.SetEpoch(ui_->epoch->value()); }

void ConfigSettings::SetHiddenLayers() {
  hl_ = ui_->hidden_layers->value();
  ui_->horizontalLayout->setSpacing(10);
  QLayoutItem *child;
  while ((child = ui_->horizontalLayout->takeAt(0)) != nullptr) {
    delete child->widget();
    delete child;
  }
  spin_boxes_.clear();

  for (int i = 0; i < hl_; ++i) {
    QSpinBox *spinBox = new QSpinBox(this);
    spinBox->setObjectName(QString("spinBox_%1").arg(i + 1));
    spinBox->setMinimum(1);
    spinBox->setMaximum(10000);
    spinBox->setValue(100);
    spinBox->setAlignment(Qt::AlignCenter);
    spinBox->setFixedWidth(71);
    spinBox->setFixedHeight(21);
    connect(spinBox, QOverload<int>::of(&QSpinBox::valueChanged), this,
            &ConfigSettings::SetLayers);
    spin_boxes_.push_back(spinBox);
    ui_->horizontalLayout->addWidget(spinBox);
  }
  SetLayers();
}

void ConfigSettings::SetLayers() {
  data_config_.size.clear();
  data_config_.size.resize(spin_boxes_.size() + 2);
  data_config_.size[0] = 784;
  for (int i = 0; i < (int)spin_boxes_.size(); ++i) {
    int value = spin_boxes_[i]->value();
    data_config_.size[i + 1] = value;
  }
  data_config_.layers = spin_boxes_.size() + 2;
  data_config_.size[data_config_.layers - 1] = 26;
}

void ConfigSettings::SetLearningRate() {
  data_config_.learning_rate = ui_->learning_rate->value();
}

void ConfigSettings::SetMoment() {
  data_config_.moment = ui_->moment->value();

  qDebug() << ui_->to_filepath->currentText();

  QString cur = ui_->to_filepath->currentText();
  qDebug() << cur;
}

void ConfigSettings::LoadFilepath() {
  QString file =
      QFileDialog::getOpenFileName(this, "Upload file ", "", "CSV (*.csv)");
  if (!file.isEmpty()) {
    ui_->to_filepath->addItem(file);
  }
}

void ConfigSettings::LoadFilepathForTest() {
  QString file =
      QFileDialog::getOpenFileName(this, "Upload file ", "", "CSV (*.csv)");
  if (!file.isEmpty()) {
    ui_->to_filepath_for_test->addItem(file);
  }
}

QString ConfigSettings::GetFilePath() {
  return ui_->to_filepath->currentText();
}

int ConfigSettings::GetNumbGroups() { return ui_->num_group->value(); }

void ConfigSettings::SetFormulaLr() {
  if (ui_->use_lr_formula->checkState() == false)
    ui_->learning_rate->setEnabled(true);
  else
    ui_->learning_rate->setDisabled(true);
}

void ConfigSettings::SetReport() { ui_->learning_rate->setEnabled(true); }

void ConfigSettings::OnOffWeightsSettings(bool value) {
  ui_->layers_from_file->setEnabled(value);
}

void ConfigSettings::SetMatrixRealisation() {
  controller_.SetTypePerceptron(0);
  OnOffWeightsSettings(!controller_.CheckEmptyLayers());
  ui_->layers_from_settings->setChecked(!ui_->layers_from_file->isChecked() ||
                                        controller_.CheckEmptyLayers());
}

void ConfigSettings::SetGraphRealisation() {
  controller_.SetTypePerceptron(1);
  OnOffWeightsSettings(!controller_.CheckEmptyLayers());
  ui_->layers_from_settings->setChecked(!ui_->layers_from_file->isChecked() ||
                                        controller_.CheckEmptyLayers());
}

void ConfigSettings::SetTypeFunction() { ui_->type_function->setEnabled(true); }

bool ConfigSettings::LoadFromSettings() {
  return ui_->layers_from_settings->isChecked();
}

void ConfigSettings::OnOffLayersSettings(bool value) {
  ui_->hidden_layers->setEnabled(value);
  for (int i = 0; i < (int)spin_boxes_.size(); ++i) {
    spin_boxes_[i]->setEnabled(value);
    ;
  }
}

void ConfigSettings::SetSettingsToModel() {
  controller_.SetIsEnabledTesting(ui_->training_plus_testing->isChecked());
  controller_.SetLearningRate(ui_->learning_rate->value());
  controller_.SetIsEnabledCalcLR(ui_->use_lr_formula->isChecked());
  controller_.SetIsEnabledReport(ui_->make_report->isChecked());
  controller_.SetMoment(ui_->moment->value());
  controller_.SetEnabledCV(ui_->cv_mode->isChecked());
}

void ConfigSettings::SetIsEnabledReport() {
  controller_.SetIsEnabledReport(ui_->make_report->isChecked());
}

void ConfigSettings::StartTraining() {
  int num_group = ui_->num_group->value();
  double testing_rate = ui_->testing_rate->value();
  std::string to_training_filepath_ =
      ui_->to_filepath->currentText().toStdString();
  std::string to_testing_filepath =
      ui_->to_filepath_for_test->currentText().toStdString();
  controller_.StartTraining(num_group, testing_rate, to_training_filepath_,
                            to_testing_filepath);
  OnOffWeightsSettings(!controller_.CheckEmptyLayers());
}

void ConfigSettings::RunTesting() {
  double testing_rate = ui_->testing_rate->value();
  std::string to_testing_filepath =
      ui_->to_filepath_for_test->currentText().toStdString();
  int type = ui_->graph->isChecked();
  int numb_exp = ui_->numb_exp->value();
  bool is_enabled_exp = ui_->make_exp->isChecked();
  controller_.StartTesting(to_testing_filepath, testing_rate, type, numb_exp,
                           is_enabled_exp);
}

void ConfigSettings::TestingModeSwitcher() {
  OnOffTeachingSettings(!ui_->testing_mode->isChecked());
}

void ConfigSettings::CVModeSwitcher() {
  ui_->epoch->setEnabled(!ui_->cv_mode->isChecked());
  ui_->make_exp->setEnabled(!ui_->cv_mode->isChecked());
  ui_->numb_exp->setEnabled(!ui_->cv_mode->isChecked());
}

void ConfigSettings::TrainingSwitcher() {
  OnOffTestingSettings(!ui_->training_mode->isChecked());
}

void ConfigSettings::TrainingTestingSwitcher() {
  ui_->num_group->setEnabled(!ui_->training_plus_testing->isChecked());
  ui_->testing_rate->setDisabled(!ui_->training_plus_testing->isChecked());
}

void ConfigSettings::SetExperiment() {
  ui_->make_report->setEnabled(!ui_->make_exp->isChecked());
  ui_->make_report->setChecked(false);
  ui_->numb_exp->setEnabled(ui_->make_exp->isChecked());
}

void ConfigSettings::OnOffTeachingSettings(bool value) {
  ui_->testing_rate->setDisabled(value);
  ui_->learning_rate->setEnabled(value);
  ui_->epoch->setEnabled(value);
  ui_->moment->setEnabled(value);
  ui_->hidden_layers->setEnabled(value);
  ui_->num_group->setEnabled(value);
  ui_->use_lr_formula->setEnabled(value);
  ui_->layers_from_settings->setEnabled(value);
  ui_->layers_from_file->setEnabled(value);
  OnOffLayersSettings(value);
}

void ConfigSettings::OnOffTestingSettings(bool value) {
  ui_->make_report->setEnabled(value);
  ui_->num_group->setEnabled(value);
  ui_->testing_rate->setEnabled(value);
  ui_->make_exp->setEnabled(value);
  ui_->numb_exp->setEnabled(value);
  SetExperiment();
}

void ConfigSettings::OnOffTypeFunction(bool value) {
  ui_->type_function->setEnabled(value);
}

void ConfigSettings::OnOffAllSettings(bool value) {
  ui_->gridWidget->setEnabled(value);
}

bool ConfigSettings::TesingModeOn() {
  return ui_->testing_mode->isChecked() != false;
}

}  // namespace s21
