// Copyright (c) 2015-2016 Hypha

#ifndef EMAIL_H
#define EMAIL_H

#include <hypha/plugin/hyphaplugin.h>
#include <string>

namespace hypha {
namespace plugin {
namespace email {
class EMail : public HyphaPlugin {
 public:
  void doWork();
  void setup();
  std::string communicate(std::string message);
  const std::string name() { return "email"; }
  const std::string getTitle() { return "EMail"; }
  const std::string getVersion() { return "0.1"; }
  const std::string getDescription() { return "Plugin to send emails."; }
  const std::string getConfigDescription() { return "{}"; }
  void loadConfig(std::string json);
  std::string getConfig();
  HyphaPlugin *getInstance(std::string id);

  void receiveMessage(std::string message);
  void sendMail(std::string content);

 private:
  std::string host;
  int port;
  std::string user;
  std::string password;
  std::string recipient;
};
}
}
}
#endif  // EMAIL_H
