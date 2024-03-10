#include "Draw/DrawingWidget.h"

#include <ostream>

#include "ui_drawingwidget.h"

namespace s21 {

DrawingWidget::DrawingWidget(Controller &controller, QWidget *parent)
    : QWidget(parent), ui_(new Ui::DrawingWidget), controller_(controller) {
  ui_->setupUi(this);
  setWindowTitle("Drawing window");
  setGeometry(280, 280, 280, 280);
  setFocusPolicy(Qt::StrongFocus);
  //    setBackgroundBrush(QColor(61, 61, 61));
  //    this->setFocus();

  connect(ui_->clean, QOverload<bool>::of(&QPushButton::clicked), this,
          &DrawingWidget::ClearVector);
  connect(ui_->recognize, QOverload<bool>::of(&QPushButton::clicked), this,
          &DrawingWidget::SendToRecognize);
}

DrawingWidget::~DrawingWidget() { delete ui_; }

void DrawingWidget::paintEvent(QPaintEvent *) {
  QPalette pal(palette());
  pal.setColor(QPalette::Window, Qt::black);
  setAutoFillBackground(true);
  setPalette(pal);
  QPainter painter(this);
  //    painter.begin(this);
  painter.setRenderHint(QPainter::Antialiasing, true);
  painter.setPen(QPen(Qt::green, 10, Qt::SolidLine));
  for (int i = 0; i < vec_.size(); ++i)
    if (i > 0) painter.drawEllipse(vec_[i - 1], 12, 12);
  painter.end();
}

void DrawingWidget::mousePressEvent([[maybe_unused]] QMouseEvent *event) {
  drawing = true;
}

void DrawingWidget::mouseMoveEvent(QMouseEvent *event) {
  if (drawing) {
    vec_.push_back(event->pos());
  }
  update();
}

void DrawingWidget::mouseReleaseEvent([[maybe_unused]] QMouseEvent *event) {
  drawing = false;
}

void DrawingWidget::ClearVector() {
  vec_.clear();
  update();
}

void DrawingWidget::SendToRecognize() {
  QPixmap scr;
  scr = grab();
  controller_.LetterRecognition(scr);
}

}  // namespace s21
