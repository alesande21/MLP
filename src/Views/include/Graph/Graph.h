#ifndef GRAPH_H
#define GRAPH_H

#include <QWidget>

#include "qcustomplot.h"

namespace Ui {
class Graph;
}

namespace s21 {
class Graph : public QWidget {
  Q_OBJECT

 public:
  explicit Graph(QWidget *parent = nullptr);
  ~Graph();

 public slots:
  void ReceiveSettings(int x, int y);
  void ReceiveData(QVector<double> x, QVector<double> y);
  void ReceiveStat(double av_accuracy, double precision, double recall,
                   double f_measure, double time_spent);

 private:
  Ui::Graph *ui_;

  void LoadSettings();
};

}  // namespace s21

#endif  // GRAPH_H
