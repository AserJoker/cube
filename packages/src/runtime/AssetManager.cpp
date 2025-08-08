#include "runtime/AssetManager.hpp"
#include "core/File.hpp"
#include "core/Object.hpp"
#include "core/Registry.hpp"
#include "runtime/IAssetTransformer.hpp"
#include <cctype>
#include <filesystem>
#include <mutex>
#include <shared_mutex>
#include <string>
#include <vector>
using namespace std::filesystem;
namespace cube::runtime {

AssetManager::AssetManager() {
  _types["unknown"] = {
      .transformers = {},
  };
}

void AssetManager::resolveNode(const std::vector<std::string> &ns,
                               const std::filesystem::path &path) {
  if (!is_directory(path)) {
    std::string type;
    auto filename = path.filename().string();
    if (path.has_extension()) {
      type = path.extension().string();
      filename = filename.substr(0, filename.size() -
                                        path.extension().string().size());
      type = type.substr(1);
      std::transform(type.begin(), type.end(), type.begin(), tolower);
    } else {
      type = "unknown";
    }
    std::string name = filename;
    std::string group = type;
    if (_typeGroups.contains(type)) {
      group = _typeGroups.at(type);
    };

    AssetNamespace *nsNode = nullptr;
    for (size_t idx = 0; idx < ns.size() - 1; idx++) {
      auto &n = ns[idx];
      if (!nsNode) {
        nsNode = &_stores[group].namespaces[n];
      } else {
        nsNode = &nsNode->children[n];
      }
    }
    auto &node = nsNode->nodes[name];
    node.path = path.string();
    node.type = type;
    if (node.asset) {
      delete node.asset;
      node.asset = nullptr;
    }
    return;
  }
  for (auto &item : directory_iterator(path)) {
    if (is_directory(item)) {
      std::vector<std::string> currentNs = ns;
      currentNs.push_back(item.path().filename().string());
      resolveNode(currentNs, item);
    } else {
      std::string type;
      auto filename = item.path().filename().string();
      if (item.path().has_extension()) {
        type = item.path().extension().string();
        filename = filename.substr(
            0, filename.size() - item.path().extension().string().size());
        type = type.substr(1);
        std::transform(type.begin(), type.end(), type.begin(), tolower);
      } else {
        type = "unknown";
      }
      std::string name = filename;
      std::string group = type;
      if (_typeGroups.contains(type)) {
        group = _typeGroups.at(type);
      };

      AssetNamespace *nsNode = nullptr;
      for (auto &n : ns) {
        if (!nsNode) {
          nsNode = &_stores[group].namespaces[n];
        } else {
          nsNode = &nsNode->children[n];
        }
      }
      auto &node = nsNode->nodes[name];
      node.path = item.path().string();
      node.type = type;
      if (node.asset) {
        delete node.asset;
        node.asset = nullptr;
      }
    }
  }
}

void AssetManager::initStore(const std::string path) {
  if (!is_directory(path)) {
    return;
  }
  for (auto &ns : directory_iterator(path)) {
    if (ns.is_directory()) {
      for (auto &item : directory_iterator(ns)) {
        resolveNode(
            {ns.path().filename().string(), item.path().filename().string()},
            item.path());
      }
    }
  }
}
void AssetManager::initType(const std::string &name, const std::string &group,
                            const std::vector<std::string> &transformers) {
  _types[name] = {transformers};
  if (!group.empty()) {
    _typeGroups[name] = group;
  }
}

void AssetManager::setGroup(const std::string &name, const std::string &group) {
  _groups[group].assets.push_back(name);
}
const std::vector<std::string> &
AssetManager::getGroup(const std::string &name) const {
  return _groups.at(name).assets;
}
std::vector<std::string> AssetManager::getGroups() const {
  std::vector<std::string> groups;
  for (auto &[group, _] : _groups) {
    groups.push_back(group);
  }
  return groups;
}

void AssetManager::load(const std::string &name) {
  try {
    auto node = get(name);
    if (node && !node->asset) {
      std::unique_lock<std::shared_mutex> lock(node->mutex);
      if (node->path.empty()) {
        return;
      }
      core::File file;
      file.open(node->path);
      core::Object *asset = file.read();
      if (asset && _types.contains(node->type)) {
        for (auto &transformer : _types[node->type].transformers) {
          auto trans =
              core::Registry::get(transformer)->cast<IAssetTransformer>();
          if (trans) {
            asset = trans->transform(asset);
          } else {
            _logger->error("Failed to load asset '{}':unknown transformer '{}'",
                           name, transformer);
            delete asset;
            break;
          }
          if (!asset) {
            break;
          }
        }
      }
      if (asset) {
        asset->setParent(this);
      }
      node->asset = asset;
    }
  } catch (std::exception &e) {
    _logger->error("Failed to load asset '{}': {}", name, e.what());
  }
}

void AssetManager::unload(const std::string &name) {
  auto node = get(name);
  if (node && node->asset) {
    std::unique_lock<std::shared_mutex> lock(node->mutex);
    delete node->asset;
    node->asset = nullptr;
  }
}
void AssetManager::loadGroup(const std::string &group) {
  if (!_groups.contains(group)) {
    return;
  }
  auto &g = _groups.at(group);
  for (auto &item : g.assets) {
    load(item);
  }
}
void AssetManager::unloadGroup(const std::string &group) {
  if (!_groups.contains(group)) {
    return;
  }
  auto &g = _groups.at(group);
  for (auto &item : _groups.at(group).assets) {
    unload(item);
  }
}
void AssetManager::store(const std::string &ns, const std::string &name,
                         const std::string &type, core::Object *object,
                         const std::string &path, const std::string &group) {
  auto &store = _stores[type];
  std::string nsName;
  AssetNamespace *nsNode = nullptr;
  for (auto &ch : ns) {
    if (ch == '.') {
      if (!nsNode) {
        nsNode = &store.namespaces[nsName];
      } else {
        nsNode = &nsNode->children[nsName];
      }
      nsName.clear();
    } else {
      nsName += ch;
    }
  }
  if (!nsNode) {
    nsNode = &store.namespaces[nsName];
  } else {
    nsNode = &nsNode->children[nsName];
  }
  auto &node = nsNode->nodes[name];
  std::unique_lock<std::shared_mutex> lock(node.mutex);
  if (node.asset && node.asset != object) {
    delete node.asset;
  }
  node.asset = object;
  if (object) {
    object->setParent(this);
  }
  node.type = type;
  node.path = path;
};

AssetManager::AssetNode *AssetManager::get(const std::string &name) {
  auto typeEnd = name.find_first_of(':');
  if (typeEnd == std::string::npos) {
    return nullptr;
  }
  std::string type = name.substr(0, typeEnd);
  auto nsEnd = name.find_last_of('.');
  std::string ns = name.substr(typeEnd + 1, nsEnd - typeEnd - 1);
  AssetNamespace *nsNode = getNamespace(type, ns);
  if (!nsNode) {
    return nullptr;
  }
  std::string assetName = name.substr(nsEnd + 1);
  if (!nsNode->nodes.contains(assetName)) {
    return nullptr;
  }
  return &nsNode->nodes.at(assetName);
}

AssetManager::AssetNamespace *
AssetManager::getNamespace(const std::string &type, const std::string &nsName) {
  if (!_stores.contains(type)) {
    return nullptr;
  }
  std::string ns;
  AssetNamespace *nsNode = nullptr;
  auto &group = _stores.at(type);
  for (auto &ch : nsName) {
    if (ch == '.') {
      if (!nsNode) {
        if (!group.namespaces.contains(ns)) {
          return nullptr;
        }
        nsNode = &group.namespaces[ns];
      } else {
        if (!nsNode->children.contains(ns)) {
          return nullptr;
        }
        nsNode = &nsNode->children[ns];
      }
      ns.clear();
    } else {
      ns += ch;
    }
  }
  if (!ns.empty()) {
    if (!nsNode) {
      if (!group.namespaces.contains(ns)) {
        return nullptr;
      }
      nsNode = &group.namespaces[ns];
    } else {
      if (!nsNode->children.contains(ns)) {
        return nullptr;
      }
      nsNode = &nsNode->children[ns];
    }
  }
  return nsNode;
}

core::Object *AssetManager::query(const std::string &name) {
  auto node = get(name);
  if (node) {
    if (!node->asset) {
      load(name);
    }
    std::shared_lock<std::shared_mutex> lock(node->mutex);
    return node->asset;
  }
  return nullptr;
}
core::Object *AssetManager::query(const AssetManager::Asset &name) {
  auto nsNode = getNamespace(name.type, name.ns);
  if (!nsNode) {
    return nullptr;
  }
  auto &node = nsNode->nodes.at(name.name);
  if (!node.asset) {
    load(std::format("{}:{}.{}", name.type, name.ns, name.name));
  }
  return node.asset;
}

void AssetManager::filter(std::vector<Asset> &output, AssetNamespace &nsNode,
                          const std::string &ns,
                          const std::function<bool(const Asset &)> &callback) {
  for (auto &[name, node] : nsNode.nodes) {
    if (!callback || callback({node.type, ns, name})) {
      output.push_back({node.type, ns, name});
    }
  }
  for (auto &[name, child] : nsNode.children) {
    filter(output, child, std::format("{}.{}", ns, name), callback);
  }
}

std::vector<AssetManager::Asset>
AssetManager::queryAll(const std::string &type) {
  std::vector<AssetManager::Asset> result;
  if (_stores.contains(type)) {
    auto &store = _stores.at(type);
    for (auto &[ns, nsNode] : store.namespaces) {
      filter(result, nsNode, ns);
    }
  }
  return result;
}

std::vector<AssetManager::Asset>
AssetManager::filter(const std::function<bool(const Asset &)> &callback) {
  std::vector<AssetManager::Asset> result;
  for (auto &[type, storeNode] : _stores) {
    for (auto &[ns, nsNode] : storeNode.namespaces) {
      filter(result, nsNode, ns, callback);
    }
  }
  return result;
}
} // namespace cube::runtime