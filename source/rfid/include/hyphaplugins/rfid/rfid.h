// Copyright (c) 2015-2016 Hypha

#ifndef RFID_H
#define RFID_H

#include <hypha/plugin/hyphaplugin.h>
#include <QtSerialPort/QSerialPort>
#include <string>

namespace hypha {
namespace plugin {
namespace rfid {
class RFID : public HyphaPlugin {
 public:
  void doWork();
  void setup();
  const std::string name() { return "rfid"; }
  const std::string getTitle() { return "RFID"; }
  const std::string getVersion() { return "0.1"; }
  const std::string getDescription() { return "Plugin to detect rfid cards."; }
  const std::string getConfigDescription() { return "{}"; }
  void loadConfig(std::string json);
  std::string getConfig();
  HyphaPlugin *getInstance(std::string id);

  void receiveMessage(std::string message);
  std::string communicate(std::string message);

 private:
  QSerialPort port;
  int fd;
  void setRGY(bool red, bool green, bool yellow);
  void setDoor(bool open);
  void beep();
  int read_until(int fd, char *buf, char until);
};
}
}
}
#endif  // RFID_H
