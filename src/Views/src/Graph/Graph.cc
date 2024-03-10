#include "Graph/Graph.h"

#include "ui_graph.h"

namespace s21 {

Graph::Graph(QWidget *parent) : QWidget(parent), ui_(new Ui::Graph) {
  ui_->setupUi(this);
  setWindowTitle("Graph");
  LoadSettings();
}

Graph::~Graph() { delete ui_; }

void Graph::ReceiveSettings(int x, int y) {
  ui_->widget->clearGraphs();
  ui_->widget->addGraph();
  ui_->widget->xAxis->setNumberPrecision(0);
  ui_->widget->xAxis->setRange(0, x);
  ui_->widget->yAxis->setRange(0, y);
  ui_->widget->graph(0)->setPen(QPen(Qt::white, 2));
  ui_->widget->replot();
}

void Graph::ReceiveData(QVector<double> x, QVector<double> y) {
  ui_->widget->graph(0)->addData(x, y);
  ui_->widget->replot();
}

void Graph::ReceiveStat(double av_accuracy, double precision, double recall,
                        double f_measure, double time_spent) {
  ui_->av_accurancy->setText(QString::number(av_accuracy, 'f', 2));
  ui_->precision->setText(QString::number(precision, 'f', 2));
  ui_->recall->setText(QString::number(recall, 'f', 2));
  ui_->f_measure->setText(QString::number(f_measure, 'f', 2));
  ui_->time_spent->setText(QString::number(time_spent, 'f', 2));
}

void Graph::LoadSettings() {
  ui_->widget->yAxis->setLabel("процент ошибок");
  ui_->widget->xAxis->setLabel("эпохи");
  ui_->widget->setBackground(QColor(61, 61, 61, 60));
  ui_->widget->xAxis->setTickLabelColor(Qt::white);
  ui_->widget->xAxis->setBasePen(QPen(Qt::white));
  ui_->widget->xAxis->setLabelColor(Qt::white);
  ui_->widget->xAxis->setTickPen(QPen(Qt::white));
  ui_->widget->xAxis->setSubTickPen(QPen(Qt::white));
  ui_->widget->yAxis->setTickLabelColor(Qt::white);
  ui_->widget->yAxis->setBasePen(QPen(Qt::white));
  ui_->widget->yAxis->setLabelColor(Qt::white);
  ui_->widget->yAxis->setTickPen(QPen(Qt::white));
  ui_->widget->yAxis->setSubTickPen(QPen(Qt::white));
}

}  // namespace s21
