#pragma once
#include "ILogger.hpp"
namespace cube::core {
class Logger : public ILogger {
protected:
  void write(const Level &level, const std::string &message) override;
};
} // namespace cube::core