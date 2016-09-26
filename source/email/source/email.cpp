// Copyright (c) 2015-2016 Hypha

#include "hyphaplugins/email/email.h"

#include <Poco/ClassLibrary.h>
#include <Poco/Net/MailMessage.h>
#include <Poco/Net/MailRecipient.h>
#include <Poco/Net/NetException.h>
#include <Poco/Net/SMTPClientSession.h>

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include <hypha/plugin/hyphaplugin.h>
#include <hypha/utils/logger.h>

using namespace hypha::utils;
using namespace hypha::plugin;
using namespace hypha::plugin::email;

void EMail::doWork() { std::this_thread::sleep_for(std::chrono::seconds(1)); }

void EMail::setup() {}

std::string EMail::communicate(std::string UNUSED(message)) {
  return "NOTSUPPORTED";
}

void EMail::loadConfig(std::string json) {
  boost::property_tree::ptree pt;
  std::stringstream ss(json);
  boost::property_tree::read_json(ss, pt);
  if (pt.get_optional<std::string>("user")) {
    user = pt.get<std::string>("user");
  }
  if (pt.get_optional<std::string>("host")) {
    host = pt.get<std::string>("host");
  }
  if (pt.get_optional<int>("port")) {
    port = pt.get<int>("port");
  }
  if (pt.get_optional<std::string>("password")) {
    password = pt.get<std::string>("password");
  }
  if (pt.get_optional<std::string>("recipient")) {
    recipient = pt.get<std::string>("recipient");
  }
}

std::string EMail::getConfig() { return "{}"; }

HyphaPlugin *EMail::getInstance(std::string id) {
  EMail *instance = new EMail();
  instance->setId(id);
  return instance;
}

void EMail::receiveMessage(std::string message) {
  boost::property_tree::ptree pt;
  std::stringstream ss(message);
  boost::property_tree::read_json(ss, pt);
  if (pt.get_optional<std::string>("mail")) {
    std::string mail = pt.get<std::string>("mail");
    sendMail(mail);
  }
}

void EMail::sendMail(std::string content) {
  Poco::Net::MailMessage message;
  message.setSender(this->user);
  message.addRecipient(Poco::Net::MailRecipient(
      Poco::Net::MailRecipient::PRIMARY_RECIPIENT, this->recipient));
  message.setSubject("Message from Hypha plugin: " + id);
  message.setContentType("text/plain; charset=UTF-8");
  message.setContent(content, Poco::Net::MailMessage::ENCODING_8BIT);
  try {
    Poco::Net::SMTPClientSession session(host, port);
    session.open();
    try {
      session.login(Poco::Net::SMTPClientSession::AUTH_NONE, user, password);
      session.sendMessage(message);
      session.close();
    } catch (Poco::Net::SMTPException &e) {
      Logger::warning("[EMail Plugin][" + id + "]" + e.displayText());
      session.close();
      return;
    }
  } catch (Poco::Net::NetException &e) {
    Logger::warning("[EMail Plugin][" + id + "]" + e.displayText());
    return;
  }
}

POCO_BEGIN_MANIFEST(HyphaPlugin)
POCO_EXPORT_CLASS(EMail)
POCO_END_MANIFEST
