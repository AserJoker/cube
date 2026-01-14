#ifndef _H_CUBE_RUNTIME_LOGEER_TARGET_
#define _H_CUBE_RUNTIME_LOGEER_TARGET_
#include <fstream>
#include <streambuf>
#include <unordered_map>
#include <vector>
namespace cube::runtime {
class LoggerTarget : public std::streambuf {
private:
  std::vector<std::streambuf *> _streams;
  std::unordered_map<std::string, std::ofstream> _files;

protected:
  auto overflow(int c) -> int override;
  auto sync() -> int override;

public:
  auto addTarget(std::ostream &target) -> void;
  auto addTarget(std::streambuf *target) -> void;
  auto addTarget(const std::string &path) -> void;
};
} // namespace cube::runtime
#endif