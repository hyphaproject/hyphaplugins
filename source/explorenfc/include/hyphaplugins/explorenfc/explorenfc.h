// Copyright (c) 2015-2016 Hypha

#ifndef EXPLORENFC_H
#define EXPLORENFC_H

#include <hypha/plugin/hyphaplugin.h>
#include <string>

namespace hypha {
namespace plugin {
namespace explorenfc {
class ExploreNFC : public HyphaPlugin {
 public:
  void doWork();
  void setup();
  std::string communicate(std::string message);
  const std::string name() { return "explorenfc"; }
  const std::string getTitle() { return "ExploreNFC"; }
  const std::string getVersion() { return "0.1"; }
  const std::string getDescription() { return "Plugin to detect rfid cards."; }
  const std::string getConfigDescription() { return "{}"; }
  void loadConfig(std::string json);
  std::string getConfig();
  HyphaPlugin *getInstance(std::string id);

  void receiveMessage(std::string message);
};
}
}
}
#endif  // EXPLORENFC_H
