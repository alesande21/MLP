//
// Created by Alesander Britni on 11/20/23.
//

#include <QApplication>
#include <QSurfaceFormat>

#include "Controller.h"
#include "DataStruct/DataNetwork.h"
#include "Logging/Logging.h"
#include "MainWindow/MainWindow.h"
#include "Perceptron/Perceptron.h"

int main(int argc, char *argv[]) {
  s21::Logging logging;
  s21::DataNetwork data_config({784, 100, 100, 26}, 0.7, 0.1);
  s21::Perceptron perceptron;
  s21::Controller controller(perceptron);
  QSurfaceFormat format;
  format.setVersion(4, 1);
  format.setProfile(QSurfaceFormat::CoreProfile);
  QSurfaceFormat::setDefaultFormat(format);
  QApplication a(argc, argv);
  s21::MainWindow w(controller, data_config, logging);
  //  w.setAttribute(Qt::WA_QuitOnClose);
  w.show();
  return a.exec();
}
