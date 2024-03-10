//
// Created by Alesander Britni on 12/15/23.
//

#include "Perceptron/Perceptron.h"

#include <mutex>

namespace s21 {

Perceptron::Perceptron() { perceptron_ = switcher_.Switcher(0); }

Perceptron::~Perceptron() {}

void Perceptron::SwitchPerceptron(int num) {
  perceptron_ = switcher_.Switcher(num);
  //        Logging::Log("Активирован" +
  //        QString::fromStdString(perceptron_->GetClassName()) + "перцептрон.",
  //        0, 2,1);
}

void Perceptron::StartTesting(const std::string &filepath, double testing_rate,
                              int type, int numb_exp, bool is_enabled_exp) {
  if (EmptyLayers()) {
    Logging::LogError(
        "Перцептрон " + QString::fromStdString(perceptron_->GetClassNameMLP()) +
            " пустой! Тестирование невозможно",
        0, 0, 1);
    return;
  }

  ParseFile::data_type data;
  data = parser_.Parser(filepath);

  try {
    data = parser_.Parser(filepath);
  } catch (...) {
    Logging::LogError(
        "Необработанное исключение при считывании файла. Процесс тестирования "
        "прерван!",
        0, 6, 1);
    return;
  }

  if (!is_enabled_exp)
    RunTesting(0, data.size(), data, testing_rate, epoch_, true);
  if (is_enabled_exp) PreExperiment(data, testing_rate, type, numb_exp);
}

void Perceptron::StartTraining(int numb_group, double testing_rate,
                               const std::string &filepath_train,
                               const std::string &filepath_test) {
#if 0
        QString a("Training begin... ");
        logging.append("Training begin... " +  (QString::number(120 / 60.)));
        return;
#endif

  abort_training_ = false;
  s21::ParseFile::data_type data;
  s21::ParseFile::data_type data_test;
  double ra = 0.0, maxra = 0.0;
  int right{}, predict{};
  int epoch = 0;

  std::chrono::duration<double> time;
  auto timer_std_start = Clocks::StdTimer();
  auto timer_rdtsc_start = Clocks::Rdtsc();

  try {
    if (!ParcingDataset(filepath_train, data, filepath_test, data_test)) return;
  } catch (...) {
    Logging::LogError(
        "Необработанное исключение при считывании файла. Процесс обучения "
        "прерван!",
        0, 6, 1);
    return;
  }

  auto timer_rdtsc_end = Clocks::Rdtsc();
  auto timer_std_end = Clocks::StdTimer();

  if (!cross_valid_) numb_group = 1;
  int group_size = data.size() / numb_group;
  std::vector<int> groups(numb_group);
  int index{};
  if (data.empty()) groups.clear();
  for (int i = 0; i < (int)groups.size(); ++i) {
    index = i * group_size;
    groups[i] = index;
  }

  int examples = -1;
  if (cross_valid_) epoch_ = numb_group;
  if (is_enabled_report_) SetSettingsToGraph(epoch_);
  time = timer_std_end - timer_std_start;

  Logging::Log("Время парсинга базы данных: Clocks: " +
                   QString::number(timer_rdtsc_end - timer_rdtsc_start) +
                   " Секунды: " + QString::number(time.count() / 60., 'f', 2),
               0, 16, 0);
  Logging::Log("Обучение начинается... ну всё, я спать!", 0, 4, 1);
  while (ra / examples * 100 < 100) {
    examples = 0;
    ra = 0;
    auto epoch_std_start = Clocks::StdTimer();
    auto epoch_rdtsc_start = Clocks::Rdtsc();
    int current_group = 0;
    for (int i = 0; i < (int)groups.size(); ++i) {
      if (i == current_group && cross_valid_) continue;
      int c_index = groups[i];
      int e_index = i == numb_group - 1 ? data.size() : groups[i + 1];
      for ([[maybe_unused]] int j = 0; c_index < e_index; ++j, ++c_index) {
        if (abort_training_) {
          Logging::LogError("Обучение прервано!", 0, 4, 1);
          return;
        }

        perceptron_->SetInput(data[c_index].pixels);
        right = data[c_index].answer;
        predict = perceptron_->ForwardFeed();
        if (predict != right - 1) {
          perceptron_->BackPropogation(right - 1);
          perceptron_->WeightUpdate(GetLearningRate(epoch), momentum_);
        } else {
          perceptron_->BackPropogation(right - 1);
          perceptron_->WeightUpdate(GetLearningRate(epoch), momentum_);
          ra++;
        }
        ++examples;
      }
    }
    timer_std_end = Clocks::StdTimer();
    timer_rdtsc_end = Clocks::Rdtsc();

    time = timer_std_end - epoch_std_start;
    if (ra > maxra) maxra = ra;

    Logging::Log("Эпоха: " + QString::number(epoch) + "  прав-ые ответы(%): " +
                     QString::number(ra / examples * 100, 'f', 2) +
                     "  лучший рез-т(%): " +
                     QString::number(maxra / examples * 100, 'f', 2) +
                     "  кол-во данных: " + QString::number(examples),
                 0, 6, 1);
    Logging::Log(
        "Clocks: " + QString::number(timer_rdtsc_end - epoch_rdtsc_start) +
            "  Секунды: " + QString::number(time.count(), 'f', 2),
        0, 72, 0);

    //                    printf("Clocks: %f Seconds %f\n", (double)stop -
    //                    start, (double)(stop - start)/(double)3000000000);
    qDebug() << is_enabled_test_ << cross_valid_;
    if (is_enabled_test_)
      RunTesting(0, data_test.size(), data_test, testing_rate, epoch, true);
    if (cross_valid_)
      RunTesting(groups[current_group],
                 (numb_group - 1) == current_group ? data.size()
                                                   : groups[current_group + 1],
                 data, 1.0, epoch, true);

    epoch++;
    if (epoch == epoch_ || abort_training_) break;
  }
  timer_std_end = Clocks::StdTimer();
  timer_rdtsc_end = Clocks::Rdtsc();
  time = timer_std_end - timer_std_start;

  Logging::Log("Время обучения. Clocks: " +
                   QString::number(timer_rdtsc_end - timer_rdtsc_start) +
                   " Секунды: " + QString::number(time.count() / 60., 'f', 2),
               0, 4, 1);
}

bool Perceptron::ParcingDataset(const std::string &filepath_train,
                                ParseFile::data_type &data,
                                const std::string &filepath_test,
                                ParseFile::data_type &data_test) {
  Logging::Log("### Загрузка базы данных для обучения... ", 0, 6, 0);

  data = parser_.Parser(filepath_train);
  if (data.empty()) {
    Logging::LogError(
        "Базы данных для обучения не загружены! Обучение прерывается!", 0, 6,
        0);
    return false;
  }

  if (is_enabled_test_) {
    Logging::Log("### Загрузка базы данных для тестирования... ", 0, 6, 0);
    data_test = parser_.Parser(filepath_test);
  }
  if (data_test.empty() && is_enabled_test_) {
    Logging::LogError(
        "Базы данных для тестирования не загружены! Обучение прерывается!", 0,
        6, 0);
    return false;
  }

  return true;
}

void Perceptron::RunTesting(int c_index, int e_index,
                            ParseFile::data_type &data, double testing_rate,
                            int epoch, bool print_log) {
  if (data.empty()) {
    Logging::LogError("Тестовая база пуста!", 0, 0, 1);
    return;
  }

  [[maybe_unused]] double tp{}, tn{}, fp{}, fn{};
  double ra = 0.0;
  int right{}, predict{};

  ra = 0;
  s21::Matrix::matrix_type conf_matrix(26, 26);
  if (print_log) Logging::Log("Тестирование нейросети начинается... ", 0, 4, 0);
  auto timer_std_start = Clocks::StdTimer();

  [[maybe_unused]] auto timer_rdtsc_start = Clocks::Rdtsc();
  for (; c_index < e_index * testing_rate; ++c_index) {
    perceptron_->SetInput(data[c_index].pixels);
    right = data[c_index].answer;
    predict = perceptron_->ForwardFeed();
    if (right - 1 >= 26) {
      Logging::Log("Правильный ответ " + QString::number(right, 'f', 2) +
                       " не входит в диапазон правильных классов!",
                   1, 4, 0);
    } else if (right - 1 == predict) {
      ra++;
      conf_matrix[right - 1][predict] += 1;
    } else {
      conf_matrix[right - 1][predict] += 1;
    }
  }

  s21::Matrix::matrix_type stat(27, 4);

  auto timer_std_end = Clocks::StdTimer();
  [[maybe_unused]] auto timer_rdtsc_end = Clocks::Rdtsc();
  auto time = timer_std_end - timer_std_start;
  qDebug() << time.count();
  Logging::Log("Количество правильных ответов: " +
                   QString::number(ra / e_index * 100, 'f', 2) + " процентов",
               1, 2, 0);
  if (print_log) CalcTestStat(conf_matrix, stat);
  if (print_log)
    PrintTestLog(stat, timer_rdtsc_end - timer_rdtsc_start, time.count());
  //                    if (epoch == epoch_) PrintStatForAllClasses(stat);

  Logging::Log(
      "Время тестирования: " + QString::number(time.count() / 60.0, 'f', 2), 1,
      0, 0);
  if (is_enabled_report_) ReplotGraph(100 - (ra / e_index * 100), epoch);
  if (is_enabled_report_)
    emit SendStatToGraph(AverageAccuracy(conf_matrix, stat),
                         Precision(conf_matrix, stat),
                         Recall(conf_matrix, stat), FMeasure(conf_matrix, stat),
                         (time.count() / 60.));
}

void Perceptron::CalcTestStat(Matrix::matrix_type &conf_matrix,
                              Matrix::matrix_type &stat) {
  AverageAccuracy(conf_matrix, stat);
  Precision(conf_matrix, stat);
  Recall(conf_matrix, stat);
  FMeasure(conf_matrix, stat);
}

void Perceptron::PrintTestLog(Matrix::matrix_type &stat, long long timer_rdtsc,
                              double timer_std) {
  PrintStatByClass(26, stat);

  Logging::Log("Clocks: " + QString::number(timer_rdtsc) +
                   "  Секунды: " + QString::number(timer_std, 'f', 2),
               0, 72, 0);
}

void Perceptron::PrintStatForAllClasses(
    [[maybe_unused]] Matrix::matrix_type &stat) {
#if 0
        for (int i = 0; i < 26; ++i) {
            Logging::Log("Средняя точность(average accuracy): " + QString::number(6,'f',2),1,0,0);
            Logging::Log("Прецизионность (precision): " + QString::number(5,'f',2),1,0,0);
             Logging::Log("Средняя точность(average accuracy): " + QString::number(stat[i][0],'f',2),1,0,0);
             Logging::Log("Прецизионность (precision): " + QString::number(stat[i][1],'f',2),1,0,0);
             Logging::Log("Полнота(recall): " + QString::number(stat[i][2],'f',2),1,0,0);
             Logging::Log("f-мера(f-measure): " + QString::number(stat[i][3],'f',2),1,0,0);

        }
#endif
}

void Perceptron::PrintStatByClass([[maybe_unused]] int i,
                                  Matrix::matrix_type &stat) {
  Logging::Log("Средняя точность(average accuracy): " +
                   QString::number(stat[i][0], 'f', 2),
               1, 0, 0);
  Logging::Log(
      "Прецизионность (precision): " + QString::number(stat[i][1], 'f', 2), 1,
      0, 0);
  Logging::Log("Полнота(recall): " + QString::number(stat[i][2], 'f', 2), 1, 0,
               0);
  Logging::Log("f-мера(f-measure): " + QString::number(stat[i][3], 'f', 2), 1,
               0, 0);
}

void Perceptron::PreExperiment(ParseFile::data_type &data, double testing_rate,
                               int type, int numb_exp) {
  Logging::Log("Начало эксперимента...", 0, 2, 1);
  StartExperiment(data, testing_rate, numb_exp);
  SwitchPerceptron(!type);
  if (EmptyLayers()) {
    Logging::LogError(
        "Перцептрон " + QString::fromStdString(perceptron_->GetClassNameMLP()) +
            " пустой! Эксперимент невозможен!",
        0, 0, 1);
    return;
  }
  StartExperiment(data, testing_rate, numb_exp);
}

void Perceptron::StartExperiment(ParseFile::data_type &data,
                                 double testing_rate, int numb_exp) {
  Logging::Log(
      "Аквтивирован " + QString::fromStdString(perceptron_->GetClassNameMLP()) +
          "перцептрон. Количество экспериментов: " + QString::number(numb_exp),
      0, 2, 1);
  auto timer_std_start = Clocks::StdTimer();
  auto timer_rdtsc_start = Clocks::Rdtsc();
  for (int i = 0; i < numb_exp; ++i) {
    RunTesting(0, data.size(), data, testing_rate, 0, false);
  }
  auto timer_std_end = Clocks::StdTimer();
  auto timer_rdtsc_end = Clocks::Rdtsc();
  auto time = timer_std_end - timer_std_start;
  Logging::Log("Время тестирования. Clocks: " +
                   QString::number(timer_rdtsc_end - timer_rdtsc_start) +
                   " Секунды: " + QString::number(time.count() / 60., 'f', 2),
               0, 4, 0);
}

void Perceptron::ReplotGraph(double error, int epoch) {
  x_.push_back(epoch);
  y_.push_back(error);
  emit SendDataToGraph(x_, y_);
}

void Perceptron::SetLearningRate(double lr) { lr_ = lr; }

void Perceptron::SetMoment(double momentum) { momentum_ = momentum; }

void Perceptron::SetIsEnableCalcLR(bool is_enable) {
  is_enabled_clr_ = is_enable;
}

void Perceptron::ActivateReport(bool is_enabled) {
  is_enabled_report_ = is_enabled;
}

bool Perceptron::EmptyLayers() { return perceptron_->EmptyLayers(); }

void Perceptron::ActivateTesting(bool is_enabled) {
  is_enabled_test_ = is_enabled;
}

double Perceptron::GetLearningRate(int epoch) {
  if (is_enabled_clr_) return 0.15 * exp(-epoch / (double)epoch_);
  return lr_;
}

void Perceptron::LoadAndConvertBmp(const QString &filepath) {
  QImageReader reader(filepath);
  QImage img = reader.read();

  if (img.isNull()) {
    qDebug() << "Ошибка чтения изображения:" << reader.errorString();
    Logging::LogError("Ошибка чтения изображения!", 0, 0, 1);
    return;
  }

  ConvertToVector(img);
}

void Perceptron::ConvertToImage(QPixmap &scr) {
  QImage img;
  img = scr.toImage();
  ConvertToVector(img);
}

void Perceptron::ConvertToVector(QImage &img) {
  img = img.scaled(28, 28);
  std::vector<double> input(784);
  int w = img.size().width();
  int h = img.size().height();

  for (int i = 0; i < w; i++) {
    for (int j = 0; j < h; j++) {
      if (!(j < 2 && i < 17)) input[i * h + j] = img.pixelColor(i, j).green();
    }
  }

  LetterRecognition(input);
}

void Perceptron::LetterRecognition(std::vector<double> &input) {
  if (input.empty()) {
    Logging::LogError("Нет данных для распознавания буквы!", 0, 0, 1);
    return;
  }
  int predict{};
  perceptron_->SetInput(input);
  predict = perceptron_->ForwardFeed();
  Logging::Log("Ответ нейросети: " + QString((char)(predict + 65)), 0, 2, 0);
}

void Perceptron::SaveWeight(const std::string &filepath) {
  Logging::Log("Сохранение весов... Перцептрон: " +
                   QString::fromStdString(perceptron_->GetClassNameMLP()),
               0, 2, 1);
  perceptron_->SaveWeights(filepath);
}

void Perceptron::LoadWeight(const std::string &filepath) {
  Logging::Log("Загрузка весов в" +
                   QString::fromStdString(perceptron_->GetClassNameMLP()) +
                   "перцептрон...",
               0, 2, 1);
  perceptron_->LoadWeights(filepath);
}

void Perceptron::LoadLayers(s21::DataNetwork &data_config) {
  Logging::Log("Активирован" +
                   QString::fromStdString(perceptron_->GetClassNameMLP()) +
                   "перцептрон. Подготовка к обучению... ",
               0, 2, 1);
  perceptron_->Init(data_config);
  perceptron_->PrintConf();
}

void Perceptron::SetEpoch(int value) { epoch_ = value; }

void Perceptron::SetGraph(Graph *graph) {
  graph_ = graph;
  QObject::connect(this, &Perceptron::SendDataToGraph, graph_,
                   &Graph::ReceiveData);
  QObject::connect(this, &Perceptron::SendSettingsToGraph, graph_,
                   &Graph::ReceiveSettings);
  QObject::connect(this, &Perceptron::SendStatToGraph, graph_,
                   &Graph::ReceiveStat);
}

void Perceptron::SetSettingsToGraph(int epoch) {
  x_.clear();
  y_.clear();
  emit SendSettingsToGraph(epoch, 100);
}

void Perceptron::AbortTraining() { abort_training_ = true; }

void Perceptron::CrossValidationSwitcher(bool switcher) {
  cross_valid_ = switcher;
}

double Perceptron::Recall(s21::Matrix::matrix_type &conf_matrix,
                          Matrix::matrix_type &stat) {
  double recall{};
  int count{};

  for (int i = 0; i < (int)conf_matrix.Rows(); ++i) {
    double tp{}, fn{};
    for (int j = 0; j < (int)conf_matrix.Cols(); ++j) {
      if (i != j) fn += conf_matrix[j][i];
    }
    tp += conf_matrix[i][i];
    if ((tp + fn) > 0) {
      double temp = (tp / (tp + fn));
      stat[i][2] = temp;
      recall += temp;
      ++count;
    }
  }
  stat[26][2] = recall / count;

  return recall / count;
}

double Perceptron::AverageAccuracy(Matrix::matrix_type &conf_matrix,
                                   Matrix::matrix_type &stat) {
  double avg_accurancy{};
  int count{};
  for (int i = 0; i < (int)conf_matrix.Rows(); ++i) {
    double tp{}, tn{}, fp{}, fn{};
    for (int k = 0; k < (int)conf_matrix.Rows(); ++k) {
      for (int j = 0; j < (int)conf_matrix.Cols(); ++j) {
        if (i != j && i == k) fp += conf_matrix[i][j];
        if (i != j && i == k) fn += conf_matrix[j][i];
        if (i != j && i != k) tn += conf_matrix[i][j];
      }
    }
    tp += conf_matrix[i][i];
    if ((tp + tn + fp + fn) > 0) {
      double temp = (tp + tn) / (tp + tn + fp + fn);
      stat[i][0] = temp;
      avg_accurancy += temp;
      ++count;
    }
  }

  stat[26][0] = avg_accurancy / count;

  return avg_accurancy / count;
}

double Perceptron::Precision(s21::Matrix::matrix_type &conf_matrix,
                             Matrix::matrix_type &stat) {
  double precision{};
  int count{};

  for (int i = 0; i < (int)conf_matrix.Rows(); ++i) {
    double tp{}, fp{};
    for (int j = 0; j < (int)conf_matrix.Cols(); ++j) {
      if (i != j) fp += conf_matrix[i][j];
    }
    tp += conf_matrix[i][i];
    if ((tp + fp) > 0) {
      double temp = (tp / (tp + fp));
      stat[i][1] = temp;
      precision += temp;
      ++count;
    }
  }
  stat[26][1] = precision / count;
  return precision / count;
}

double Perceptron::FMeasure(
    [[maybe_unused]] s21::Matrix::matrix_type &conf_matrix,
    Matrix::matrix_type &stat) {
  double recall{};
  double precision{};
  for (int i = 0; i < (int)stat.Rows(); ++i) {
    recall = stat[i][2];
    precision = stat[i][1];
    stat[i][3] = 2 * recall * precision / (recall + precision);
  }
  return stat[26][3];
}

void Perceptron::CalcMeasures(double &tp, double &tn, double &fp, double &fn,
                              s21::Matrix::matrix_type &conf_matrix) {
  for (int i = 0; i < (int)conf_matrix.Rows(); ++i) {
    for (int k = 0; k < (int)conf_matrix.Rows(); ++k) {
      for (int j = 0; j < (int)conf_matrix.Cols(); ++j) {
        if (i != j && i == k) fp += conf_matrix[i][j];
        if (i != j && i == k) fn += conf_matrix[j][i];
        if (i != j && i != k) tn += conf_matrix[i][j];
      }
    }
    tp += conf_matrix[i][i];
  }
}

}  // namespace s21
