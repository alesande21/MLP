#ifndef DRAWINGWIDGET_H
#define DRAWINGWIDGET_H

#include <QFile>
#include <QPainter>
#include <QPalette>
#include <QPixmap>
#include <QVector>
#include <QWidget>
#include <QtWidgets>
#include <iostream>

#include "Controller.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class DrawingWidget;
}
QT_END_NAMESPACE

namespace s21 {
class DrawingWidget : public QWidget {
  Q_OBJECT

 public:
  explicit DrawingWidget(Controller &controller, QWidget *parent = nullptr);
  DrawingWidget(DrawingWidget &other) = delete;
  DrawingWidget(DrawingWidget &&other) = delete;
  DrawingWidget &operator=(const DrawingWidget other) = delete;
  DrawingWidget &operator=(DrawingWidget &&other) = delete;
  ~DrawingWidget();

 private:
  Ui::DrawingWidget *ui_;
  Controller &controller_;
  QVector<QPointF> vec_;
  bool drawing = false;

  void paintEvent(QPaintEvent *) override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;

  void ClearVector();
  void SendToRecognize();
};
}  // namespace s21

#endif  // DRAWINGWIDGET_H
