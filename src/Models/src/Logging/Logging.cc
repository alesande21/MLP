//
// Created by Slava on 17.12.2023.
//

#include "Logging/Logging.h"

#include <iostream>

namespace s21 {

QTextEdit* s21::Logging::output_ = nullptr;
std::mutex Logging::output_mutex_ = std::mutex();

Logging::Logging(QObject* parent) : QObject(parent) {}

void Logging::Log([[maybe_unused]] const QString& message, int indentation,
                  int spaces, int empty_lines) {
  if (!output_) return;
  std::lock_guard<std::mutex> lock(output_mutex_);

  QTextCharFormat format;
  format.setFont(QFont("Verdana", 13, 50, true));
  format.setForeground(QBrush(Qt::white));
  //            format.setFont(QFont(Qt::))

  output_->setCurrentCharFormat(format);
  for (int i = 0; i < empty_lines; ++i) {
    output_->append("");
  }

  QString indents{};
  for (int i = 0; i < indentation; ++i) {
    indents += "\t";
  }

  QString space{};

  for (int i = 0; i < spaces; ++i) {
    space += " ";
  }

  std::string result_m = (indents + space + message).toStdString();

  output_->append(result_m.c_str());
}

void Logging::LogError(const QString& message, int indentation, int spaces,
                       int empty_lines) {
  if (!output_) return;

  //            std::lock_guard<std::mutex> lock(output_mutex_);
  QTextCharFormat format;
  format.setFont(QFont("Verdana", 13, 80, true));
  format.setForeground(QBrush(Qt::red));

  //            format.setFont(QFont(Qt::))
  output_->setCurrentCharFormat(format);

  for (int i = 0; i < empty_lines; ++i) {
    output_->append("");
  }

  QString indents{};
  for (int i = 0; i < indentation; ++i) {
    indents += "\t";
  }

  QString space{};

  for (int i = 0; i < spaces; ++i) {
    space += " ";
  }

  std::string result_m = (indents + space + message).toStdString();
  output_->append(result_m.c_str());
}

void Logging::SetOutput(QTextEdit* output) {
  if (output) {
    output_ = output;
  }
}

void Logging::LogLog(const std::string& message, int indentation, int spaces,
                     int empty_lines) {
  if (output_) {
    QTextCharFormat format;
    format.setFont(QFont("Verdana", 13, 50, true));
    format.setForeground(QBrush(Qt::white));
    output_->setCurrentCharFormat(format);

    for (int i = 0; i < empty_lines; ++i) {
      output_->append("");
    }

    std::string indents{};
    for (int i = 0; i < indentation; ++i) {
      indents += "\t";
    }

    std::string space{};

    for (int i = 0; i < spaces; ++i) {
      space += " ";
    }

    std::string result_m = (indents + space + message);
    if (result_m.empty()) {
      qDebug() << "result_m empty";
      output_->append("result_m empty");
      return;
    }

    output_->append(result_m.c_str());
  }
}

}  // namespace s21
