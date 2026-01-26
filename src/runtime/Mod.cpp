#include "runtime/Mod.hpp"
#include "core/Value.hpp"
#include "core/Version.hpp"
#include <unordered_map>
using namespace cube;
using namespace cube::runtime;
Mod::Manifest::Manifest() {}
Mod::Manifest::Manifest(const core::Value &value) {
  if (value.getType() != core::Value::Type::OBJECT) {
    return;
  }
  this->name = value.getField("name").getString();
  this->version = core::Version::parse(value.getField("version").getString());
  this->engine = core::Version::parse(value.getField("engine").getString());
  this->description = value.getField("description").getString();
  this->icon = value.getField("icon").getString();
  auto dependences = value.getField("dependences").getObject();
  for (auto &[key, value] : dependences) {
    this->dependences[key] = core::Version::parse(value.getString());
  }
  auto optionalDependences =
      value.getField("optionalDependences").getObject();
  for (auto &[key, value] : optionalDependences) {
    this->optionalDependences[key] = core::Version::parse(value.getString());
  }
  auto languages = value.getField("languages").getObject();
  for (auto &[key, value] : languages) {
    this->languages[key] = value.getString();
  }
  auto preload = value.getField("preload").getObject();
  for (auto &[type, resources] : preload) {
    auto &res = this->preload[type];
    auto items = resources.getObject();
    for (auto &[name, item] : items) {
      res[name] = item.getString();
    }
  }
}
Mod::Mod(const Manifest &manifest, const std::string &domain)
    : _manifest(manifest), _domain(domain) {}
auto Mod::getManifest() const -> const Mod::Manifest & { return _manifest; }
auto Mod::getDomain() const -> const std::string & { return _domain; }
auto Mod::setError(const std::string &message) -> void {
  _message = message;
  _state = State::ERR;
}
auto Mod::getState() const -> const State & { return _state; }
auto Mod::getMesage() const -> const std::string & { return _message; };
auto Mod::enable() -> void { _state = State::ENABLE; }
auto Mod::disable() -> void { _state = State::DISABLE; }
auto Mod::ready() -> void { _state = State::READY; }