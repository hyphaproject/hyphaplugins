// Copyright (c) 2015-2016 Hypha

#ifndef FLAME_H
#define FLAME_H

#include <hypha/plugin/hyphaplugin.h>
#include <mutex>
#include <string>

namespace hypha {
namespace plugin {
namespace flame {
class Flame : public HyphaPlugin {
 public:
  void doWork();
  void setup();
  const std::string name() { return "flame"; }
  const std::string getTitle() { return "Flame"; }
  const std::string getVersion() { return "0.1"; }
  const std::string getDescription() {
    return "Plugin to read flame sensor data";
  }
  const std::string getConfigDescription() {
    return "{"
           "\"confdesc\":["
           "{\"name\":\"alarm\", "
           "\"type\":\"boolean\",\"value\":true,\"description\":\"Alarm\"}"
           "]}";
  }
  void loadConfig(std::string json);
  std::string getConfig();
  std::string getStatusMessage();

  void measure();
  HyphaPlugin *getInstance(std::string id);

  void receiveMessage(std::string message);
  std::string communicate(std::string message);

 private:
  std::mutex measure_mutex;
  bool flame = false;
  bool alarm = false;
};
}
}
}
#endif  // FLAME_H
