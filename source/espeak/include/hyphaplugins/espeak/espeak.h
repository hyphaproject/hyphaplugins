// Copyright (c) 2015-2016 Hypha

#ifndef ESPEAK_H
#define ESPEAK_H

#include <hypha/plugin/hyphaplugin.h>
#include <string>

namespace hypha {
namespace plugin {
namespace espeak {
class ESpeak : public HyphaPlugin {
 public:
  void doWork();
  void setup();
  std::string communicate(std::string message);
  const std::string name() { return "espeak"; }
  const std::string getTitle() { return "ESpeak"; }
  const std::string getVersion() { return "0.1"; }
  const std::string getDescription() {
    return "Plugin to speak text with espeak.";
  }
  const std::string getConfigDescription() {
    return "{"
           "\"confdesc\":["
           "{\"name\":\"language\", "
           "\"type\":\"string\",\"value\":\"en\",\"description\":\"spoken "
           "language\"}"
           "]}";
  }
  void loadConfig(std::string json);
  std::string getConfig();
  HyphaPlugin *getInstance(std::string id);

  void receiveMessage(std::string message);

 private:
  std::string language = "en";
};
}
}
}
#endif  // ESPEAK_H
