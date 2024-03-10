//
// Created by Alesander Britni on 9/19/23.
//

#include "Parser/ParseFile.h"

namespace s21 {

ParseFile::data_type ParseFile::Parser(std::string name_file) {
  std::ifstream file(name_file);
  if (!file.is_open()) {
#if 1
//    Logging::LogError("Ошибка открытия файла: " +
//    QString::fromStdString(name_file), 0, 6, 0);
#endif
    //    throw std::invalid_argument("Invalid file extension");
    return input_;
  }
  ClearData();

  std::string line = "";
#if 1
  while (std::getline(file, line)) {
    if (line.empty()) {
      break;
    }
    ReadLine(line);
  }
#endif
  file.close();
  return input_;
}

void ParseFile::ReadLine(const std::string& line) {
  std::istringstream iss(line);
  DataInput row;
  std::string val;
  std::getline(iss, val, ',');
  try {
    row.answer = std::stoi(val);
  } catch (const std::invalid_argument& e) {
    Logging::LogError("Некорректный аргумент: ", 0, 6, 0);
  } catch (const std::out_of_range& e) {
    Logging::LogError("Выход за пределы диапазона: ", 0, 6, 0);
  } catch (...) {
    Logging::LogError(
        "Необработанное исключение при считывании файла. Процесс прерван!", 0,
        6, 0);
  }

  while (std::getline(iss, val, ',')) {
    row.pixels.push_back(std::stod(val) / 255.);
  }

  input_.push_back(row);
}

void ParseFile::ProcessLines() {
#if 0
    while (true) {
        std::string line;
        {
            std::lock_guard<std::mutex> lock(file_mutex_);
            if (lines_.empty() && file_.eof()) {
                break;  // Выход из цикла, если файл прочитан
            }
            if (lines_.empty()) {
                continue;  // Пропускаем итерацию, если нет доступных строк
            }
            line = lines_.front();
            lines_.pop_front();
        }
        ReadLine(line);
    }
#endif
}

ParseFile::data_type ParseFile::SendVec() { return input_; }

}  // namespace s21
