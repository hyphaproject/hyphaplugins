// Copyright (c) 2015-2016 Hypha

#ifndef RPIGPIO_H
#define RPIGPIO_H

#include <hypha/plugin/hyphaplugin.h>
#include <string>

namespace hypha {
namespace plugin {
namespace rpigpio {
class RpiGpio : public HyphaPlugin {
 public:
  void doWork();
  void setup();
  const std::string name() { return "rpigpio"; }
  const std::string getTitle() { return "RPI GPIO"; }
  const std::string getVersion() { return "0.1"; }
  const std::string getDescription() {
    return "Plugin to access gpio of the raspberry pi";
  }
  const std::string getConfigDescription() { return "{}"; }
  void loadConfig(std::string json);
  std::string getConfig();
  HyphaPlugin *getInstance(std::string id);

  void receiveMessage(std::string message);
  std::string communicate(std::string message);

 private:
};
}
}
}
#endif  // RPIGPIO_H
