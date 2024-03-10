#ifndef TEMPWINDOWFACTORY_H
#define TEMPWINDOWFACTORY_H

#include "ConfigSettings/ConfigSettings.h"
#include "Controller.h"

namespace s21 {
class TempWindowFactory {
 public:
  TempWindowFactory() = delete;
  TempWindowFactory(TempWindowFactory &other) = delete;
  TempWindowFactory(TempWindowFactory &&other) = delete;
  TempWindowFactory &operator=(const TempWindowFactory other) = delete;
  TempWindowFactory &operator=(TempWindowFactory &&other) = delete;
  TempWindowFactory(Controller &controller, DataNetwork &data_config);
  QWidget *SetTempWin(int windowType);
  ConfigSettings *GetConfigSettings();

 private:
  ConfigSettings *settings_;
};
}  // namespace s21
#endif  // TEMPWINDOWFACTORY_H
