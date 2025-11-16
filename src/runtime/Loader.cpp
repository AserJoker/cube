#include "runtime/Loader.hpp"
#include <filesystem>
#include <string>
#include <vector>
using namespace cube;
using namespace cube::runtime;

auto Loader::addDomain(const std::string &name, const std::string &path)
    -> bool {
  std::vector<std::string> parts;
  std::string part;
  auto p = std::filesystem::absolute(path);
  for (auto &ch : p.string()) {
    if (ch == '/' || ch == '\\') {
      if (part == ".") {
        part.clear();
      } else if (part == "..") {
        if (!parts.empty()) {
          parts.pop_back();
        } else {
          return false;
        }
        part.clear();
      } else {
        parts.push_back(part);
        part.clear();
      }
    } else {
      part += ch;
    }
  }
  if (part == "..") {
    if (parts.empty()) {
      return false;
    } else {
      parts.pop_back();
    }
  } else if (part != "." && !part.empty()) {
    parts.push_back(part);
  }
  std::string resolvedPath;
  for (size_t i = 0; i < parts.size(); ++i) {
    if (parts[i].empty() && i != 0) {
      return false;
    }
    resolvedPath += parts[i] + "/";
  }
  _domains[name] = resolvedPath;
  return true;
}

auto Loader::resolvePath(const std::string &fullpath) const -> std::string {
  auto pos = fullpath.find(":");
  auto domain = fullpath.substr(0, pos);
  auto path = fullpath.substr(pos + 1);
  if (path.starts_with('/')) {
    return "";
  }
  if (!_domains.contains(domain)) {
    return "";
  }
  std::vector<std::string> parts;
  std::string part;
  for (auto &ch : path) {
    if (ch == '/' || ch == '\\') {
      if (part == ".") {
        part.clear();
      } else if (part == "..") {
        if (!parts.empty()) {
          parts.pop_back();
        } else {
          return "";
        }
        part.clear();
      } else {
        if (part.empty()) {
          return "";
        }
        parts.push_back(part);
        part.clear();
      }
    } else {
      part += ch;
    }
  }
  if (part == ".") {
    return "";
  } else if (part == "..") {
    return "";
  } else if (!part.empty()) {
    parts.push_back(part);
    part.clear();
  }
  path = _domains.at(domain);
  for (size_t i = 0; i < parts.size(); ++i) {
    path += parts[i];
    if (i != parts.size() - 1) {
      path += "/";
    }
  }
  return path;
}

auto Loader::load(const std::string &fullpath) const
    -> std::shared_ptr<core::Buffer> {
  auto path = resolvePath(fullpath);
  if (path.empty()) {
    return nullptr;
  }
  FILE *file = fopen(path.c_str(), "rb");
  if (!file) {
    return nullptr;
  }
  fseek(file, 0, SEEK_END);
  size_t size = ftell(file);
  fseek(file, 0, SEEK_SET);
  auto buffer = std::make_shared<core::Buffer>(size);
  fread(const_cast<void *>(buffer->getData()), 1, size, file);
  fclose(file);
  return buffer;
}