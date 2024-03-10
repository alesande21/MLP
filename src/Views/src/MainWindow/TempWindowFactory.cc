#include "MainWindow/TempWindowFactory.h"

namespace s21 {

TempWindowFactory::TempWindowFactory(Controller& controller,
                                     DataNetwork& data_config) {
  settings_ = new ConfigSettings(controller, data_config);
}

QWidget* TempWindowFactory::SetTempWin(int window_type) {
  QWidget* window = nullptr;
  switch (window_type) {
    case 0:
      window = settings_;
      break;
  }
  return window;
}

ConfigSettings* TempWindowFactory::GetConfigSettings() { return settings_; }

}  // namespace s21
