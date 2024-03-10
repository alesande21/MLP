#include "MainWindow/MainWindow.h"

#include "ui_mainwindow.h"

namespace s21 {

std::mutex MainWindow::string_mutex_ = std::mutex();

MainWindow::MainWindow(Controller &controller, s21::DataNetwork &data_config,
                       Logging &logging, QWidget *parent)
    : QMainWindow(parent),
      ui_(new Ui::MainWindow),
      data_config_(data_config),
      controller_(controller),
      logging_(logging) {
  ui_->setupUi(this);
  setWindowTitle("MLP");
  setGeometry(1000, 1000, 1000, 1000);
  this->setAttribute(Qt::WA_QuitOnClose);
  logging_.SetOutput(ui_->logging);
  drawing_window_ = new DrawingWidget(controller_);
  temp_window_ = new TempWindowFactory(controller_, data_config_);
  graph_ = new Graph();
  controller_.SetGraph(graph_);
  Logging::Log(
      "Ку-ку! Параметры и веса для обучения нейросети загружены по умолчанию! ",
      0, 0, 0);
  //  ui_->logging->append("Ку-ку! Параметры и веса для обучения нейросети
  //  загружены по умолчанию! ");
  ui_->logging->moveCursor(QTextCursor::Start);

  //  setBackgroundRole(QColor(61, 61, 61));
  //  setBackground(QColor(61, 61, 61));
  //  SetParamObj("");
  connect(ui_->call_settings_ui, &QPushButton::clicked,
          [this]() { OpenTempWindow(0); });

  //  connect(ui_->open_csv, SIGNAL(triggered()), this, SLOT(OpenFile()));
  //  connect(ui_->save_weight, SIGNAL(triggered()), this, SLOT(SaveWeight()));
  //  connect(ui_->load_weight, SIGNAL(triggered()), this, SLOT(LoadWeight()));

  connect(ui_->load_bmp, QOverload<bool>::of(&QPushButton::clicked), this,
          &MainWindow::LoadBmp);

  connect(ui_->call_load, QOverload<bool>::of(&QPushButton::clicked), this,
          &MainWindow::LoadWeight);
  connect(ui_->call_save, QOverload<bool>::of(&QPushButton::clicked), this,
          &MainWindow::SaveWeight);
  connect(ui_->call_draw_window, QOverload<bool>::of(&QPushButton::clicked),
          this, &MainWindow::OpenDrawWidget);
  connect(ui_->call_graph, QOverload<bool>::of(&QPushButton::clicked), this,
          &MainWindow::OpenGraph);
  connect(ui_->run_edu, QOverload<bool>::of(&QPushButton::clicked), this,
          &MainWindow::ModeSelection);
  connect(ui_->abort_training, QOverload<bool>::of(&QPushButton::clicked), this,
          &MainWindow::AbortTraining);
}

MainWindow::~MainWindow() {
  delete drawing_window_;
  delete temp_window_;
  delete graph_;
  delete ui_;
}

void MainWindow::closeEvent(QCloseEvent *event) {
  HideAllOpenWin();
  event->accept();
}

void MainWindow::OpenTempWindow(int num_window) {
  if (window_ != nullptr && window_->isVisible()) {
    window_->hide();
  } else if (window_ != nullptr && !window_->isVisible()) {
    window_->show();
  }

  if (window_ == nullptr) {
    window_ = temp_window_->SetTempWin(num_window);
    if (window_ != nullptr) window_->show();
  }
}

void MainWindow::OpenFile() {
  QString file =
      QFileDialog::getOpenFileName(this, "Upload file ", "", "TXT (*.txt)");
  if (!file.isEmpty()) {
    //    ui_->widget->SendController().Parse(file.toUtf8().constData());
    //    ui_->widget->SetData();
  }
}

void MainWindow::SaveWeight() {
#if 1
  QString filepath = QFileDialog::getSaveFileName(this, "Save as...",
                                                  "Weights.txt", "MLP (*.txt)");
  if (!filepath.isEmpty()) {
    controller_.SaveWeight(filepath.toUtf8().constData());
  }

#endif
}

void MainWindow::LoadWeight() {
#if 1
  QString filepath = QFileDialog::getOpenFileName(this, "Load from...",
                                                  "Weights.txt", "MLP (*.txt)");
  if (!filepath.isEmpty()) {
    controller_.LoadWeight(filepath.toUtf8().constData());
    temp_window_->GetConfigSettings()->OnOffWeightsSettings(true);
  }
#endif
}

void MainWindow::LoadBmp() {
  QString filepath = QFileDialog::getOpenFileName(this, "Load from...",
                                                  "image.bmp", "MLP (*.bmp)");
  if (!filepath.isEmpty()) {
    controller_.LoadAndConvertBmp(filepath.toUtf8().constData());
  }
}

void MainWindow::OpenDrawWidget() {
  if (drawing_window_->isVisible())
    drawing_window_->hide();
  else
    drawing_window_->show();
}

void MainWindow::ModeSelection() {
  if (temp_window_->GetConfigSettings()->TesingModeOn()) {
    RunTesting();
  } else {
    RunTeaching();
  }
}

#if 1
void MainWindow::RunTesting() {
  temp_window_->GetConfigSettings()->SetIsEnabledReport();
  QFutureWatcher<void> *watcher = new QFutureWatcher<void>(this);

  AccessSwitcher(false);
  connect(watcher, &QFutureWatcher<void>::finished, this,
          [this]() { AccessSwitcher(true); });

  watcher->setFuture(QtConcurrent::run(
      [this]() { temp_window_->GetConfigSettings()->RunTesting(); }));
}
#endif

#if 1
void MainWindow::RunTeaching() {
  if (temp_window_->GetConfigSettings()->LoadFromSettings())
    controller_.LoadLayers(data_config_);
  QFutureWatcher<void> *watcher = new QFutureWatcher<void>(this);
  AccessSwitcher(false);
  connect(watcher, &QFutureWatcher<void>::finished, this,
          [this]() { AccessSwitcher(true); });

  watcher->setFuture(QtConcurrent::run([this]() {
    temp_window_->GetConfigSettings()->SetSettingsToModel();
    temp_window_->GetConfigSettings()->StartTraining();
  }));
}
#endif

void MainWindow::AbortTraining() { controller_.AbortTraining(); }

void MainWindow::AccessSwitcher(bool value) {
  ui_->call_save->setEnabled(value);
  ui_->call_load->setEnabled(value);
  ui_->run_edu->setEnabled(value);
  temp_window_->GetConfigSettings()->OnOffAllSettings(value);
}

void MainWindow::HideAllOpenWin() {
  this->show();
  if (drawing_window_->isVisible()) drawing_window_->hide();
  if (graph_->isVisible()) graph_->hide();
  if (graph_->isVisible()) graph_->hide();
  if (window_ != nullptr && window_->isVisible()) window_->hide();
}

void MainWindow::OpenGraph() {
  if (graph_->isVisible())
    graph_->hide();
  else
    graph_->show();
}

}  // namespace s21
