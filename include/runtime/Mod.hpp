#ifndef _H_CUBE_RUNTIME_MOD_
#define _H_CUBE_RUNTIME_MOD_
#include "core/Instance.hpp"
#include "core/Object.hpp"
#include "core/Value.hpp"
#include "core/Version.hpp"
#include <unordered_map>
namespace cube::runtime {
class Mod : public core::Instance {
public:
  struct Manifest : public core::Object {
    std::string name;
    core::Version version;
    core::Version engine;
    std::string description;
    std::string icon;
    std::unordered_map<std::string, core::Version> dependences;
    std::unordered_map<std::string, core::Version> optionalDependences;
    std::unordered_map<std::string, std::string> languages;
    std::unordered_map<std::string,
                       std::unordered_map<std::string, std::string>>
        preload;
    Manifest();
    Manifest(const core::Value &value);
  };
  enum class State { ENABLE, DISABLE, ERR, READY };

private:
  State _state = State::DISABLE;
  std::string _message = "";
  Manifest _manifest = {};
  std::string _domain = "";

public:
  Mod(const Manifest &manifest = {}, const std::string &domain = {});
  const Manifest &getManifest() const;
  auto getDomain() const -> const std::string &;
  void setError(const std::string &message);
  const State &getState() const;
  const std::string &getMesage() const;
  void enable();
  void disable();
  void ready();
};
} // namespace cube::runtime
#endif