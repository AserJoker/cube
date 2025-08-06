#pragma once
#include "core/ILogger.hpp"
#include "core/Object.hpp"
#include "core/Singleton.hpp"
#include <filesystem>
#include <functional>
#include <shared_mutex>
#include <string>
#include <unordered_map>
#include <vector>
namespace cube::runtime {
class AssetManager : public core::Object {
public:
  struct Asset {
    std::string type;
    std::string ns;
    std::string name;
  };

private:
  core::ILogger *_logger = core::Singleton<core::ILogger, false>::get();

private:
  struct AssetMetadata {
    std::string kind;
    std::string ns;
    std::string path;
  };

  struct AssetType {
    std::vector<std::string> transformers;
  };

  struct AssetNode {
    std::string type;
    std::string path;
    core::Object *asset = nullptr;
    std::shared_mutex mutex;
  };

  struct AssetNamespace {
    std::unordered_map<std::string, AssetNode> nodes;
    std::unordered_map<std::string, AssetNamespace> children;
  };

  struct AssetStore {
    std::unordered_map<std::string, AssetNamespace> namespaces;
  };

  struct AssetGroup {
    std::vector<std::string> assets;
  };

  std::unordered_map<std::string, std::string> _typeGroups;
  std::unordered_map<std::string, AssetType> _types;
  std::unordered_map<std::string, AssetStore> _stores;
  std::unordered_map<std::string, AssetGroup> _groups;

private:
  void resolveNode(const std::vector<std::string> &ns,
                   const std::filesystem::path &path);

private:
  AssetNode *get(const std::string &name);
  AssetNamespace *getNamespace(const std::string &type, const std::string &ns);

  void filter(std::vector<Asset> &output, AssetNamespace &nsNode,
              const std::string &ns,
              const std::function<bool(const Asset &)> &callback = {});

public:
  AssetManager();
  void initStore(const std::string path);
  void initType(const std::string &name, const std::string &group = "",
                const std::vector<std::string> &transformers = {});
  void setGroup(const std::string &name, const std::string &group);
  const std::vector<std::string> &getGroup(const std::string &name) const;
  std::vector<std::string> getGroups() const;
  void load(const std::string &name);
  void unload(const std::string &name);
  void loadGroup(const std::string &group);
  void unloadGroup(const std::string &group);
  core::Object *query(const std::string &name);
  core::Object *query(const AssetManager::Asset &name);
  void store(const std::string &ns, const std::string &name,
             const std::string &type, core::Object *object,
             const std::string &path = "", const std::string &group = "");

  std::vector<Asset> queryAll(const std::string &type);
  std::vector<Asset> filter(const std::function<bool(const Asset &)> &callback);
};
} // namespace cube::runtime