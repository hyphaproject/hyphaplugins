// Copyright (c) 2015-2016 Hypha

#ifndef LIGHTSENSOR_H
#define LIGHTSENSOR_H

#include <hypha/plugin/hyphaplugin.h>
#include <mutex>
#include <string>

namespace hypha {
namespace plugin {
namespace lightsensor {
class LightSensor : public HyphaPlugin {
 public:
  void doWork();
  void setup();
  const std::string name() { return "lightsensor"; }
  const std::string getTitle() { return "LightSensor"; }
  const std::string getVersion() { return "0.1"; }
  const std::string getDescription() { return "Plugin to read light sensor"; }
  const std::string getConfigDescription() { return "{}"; }
  void loadConfig(std::string json);
  std::string getConfig();
  std::string getStatusMessage();

  void measure();
  HyphaPlugin *getInstance(std::string id);

  void receiveMessage(std::string message);
  std::string communicate(std::string message);

 private:
  std::mutex measure_mutex;
  bool alarm = false;
  bool light = false;
};
}
}
}
#endif  // LIGHTSENSOR_H
