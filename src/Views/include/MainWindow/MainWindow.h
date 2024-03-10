#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <qstring.h>

#include <QCoreApplication>
#include <QFileDialog>
#include <QFrame>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMainWindow>
#include <QStackedLayout>
#include <QThread>
#include <QtConcurrent/QtConcurrent>
#include <mutex>

#include "ConfigSettings/ConfigSettings.h"
#include "Controller.h"
#include "Draw/DrawingWidget.h"
#include "Graph/Graph.h"
#include "Logging/Logging.h"
#include "MainWindow/TempWindowFactory.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

namespace s21 {
class MainWindow : public QMainWindow {
  Q_OBJECT
 public:
  explicit MainWindow(Controller &controller, DataNetwork &data_config,
                      Logging &logging, QWidget *parent = nullptr);
  ~MainWindow();

 protected:
  void closeEvent(QCloseEvent *event) override;

 private:
  Ui::MainWindow *ui_;
  QWidget *window_{};
  DataNetwork &data_config_;
  Controller &controller_;
  Logging &logging_;
  Graph *graph_;
  DrawingWidget *drawing_window_;
  TempWindowFactory *temp_window_;
  static std::mutex string_mutex_;

 private slots:
  void OpenFile();
  void OpenTempWindow(int num_window);
  void SaveWeight();
  void LoadWeight();
  void LoadBmp();
  void ModeSelection();
  void RunTeaching();
  void RunTesting();
  void OpenDrawWidget();
  void OpenGraph();
  void AbortTraining();
  void AccessSwitcher(bool value);
  void HideAllOpenWin();
};
}  // namespace s21

#endif  // MAINWINDOW_H
