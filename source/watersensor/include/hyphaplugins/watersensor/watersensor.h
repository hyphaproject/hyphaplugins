// Copyright (c) 2015-2016 Hypha

#ifndef WATERSENSOR_H
#define WATERSENSOR_H

#include <hypha/plugin/hyphaplugin.h>
#include <mutex>
#include <string>

namespace hypha {
namespace plugin {
namespace watersensor {
class WaterSensor : public HyphaPlugin {
 public:
  void doWork();
  void setup();
  const std::string name() { return "watersensor"; }
  const std::string getTitle() { return "WaterSensor"; }
  const std::string getVersion() { return "0.1"; }
  const std::string getDescription() { return "Plugin to read water sensor"; }

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
  bool water = false;
  int PIN = 6;
};
}
}
}
#endif  // WATERSENSOR_H
