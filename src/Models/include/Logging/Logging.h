//
// Created by Slava on 17.12.2023.
//

#ifndef MLP_LOGGING_H
#define MLP_LOGGING_H

//#pragma once

#include <QObject>
#include <QTextEdit>

#if 0
#include <QMutex>
#include <QMutexLocker>
#endif

#include <mutex>

namespace s21 {

class Logging : public QObject {
  Q_OBJECT

 public:
  Logging(QObject* parent = nullptr);
  virtual ~Logging(){};

  static void Log(const QString& message, int indentation, int spaces,
                  int empty_lines);
  static void LogError(const QString& message, int indentation, int spaces,
                       int empty_lines);
  void SetOutput(QTextEdit* output);
  void LogLog(const std::string& message, int indentation, int spaces,
              int empty_lines);

 private:
  static QTextEdit* output_;
  static std::mutex output_mutex_;
};

}  // namespace s21

#endif  // MLP_LOGGING_H
