// Copyright (c) 2015-2016 Hypha

#include "hyphaplugins/espeak/espeak.h"

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include <Poco/ClassLibrary.h>

#include <QtCore/QProcess>

#include <hypha/plugin/hyphaplugin.h>
#include <hypha/utils/logger.h>

using namespace hypha::plugin;
using namespace hypha::plugin::espeak;
using namespace hypha::utils;

void ESpeak::doWork() { std::this_thread::sleep_for(std::chrono::seconds(1)); }

void ESpeak::setup() { receiveMessage("{\"say\":\"Starting.\"}"); }

std::string ESpeak::communicate(std::string UNUSED(message)) {
  return "SUCCESS";
}

void ESpeak::loadConfig(std::string json) {
  boost::property_tree::ptree pt;
  std::stringstream ss(json);
  boost::property_tree::read_json(ss, pt);
  if (pt.get_optional<std::string>("language")) {
    language = pt.get<std::string>("language");
  }
}

std::string ESpeak::getConfig() {
  return "{\"language\":\"" + language + "\"}";
}

HyphaPlugin *ESpeak::getInstance(std::string id) {
  ESpeak *instance = new ESpeak();
  instance->setId(id);
  return instance;
}

void ESpeak::receiveMessage(std::string message) {
  boost::property_tree::ptree pt;
  std::stringstream ss(message);
  boost::property_tree::read_json(ss, pt);
  if (pt.get_optional<std::string>("language")) {
    language = pt.get<std::string>("language");
  }
  if (pt.get_optional<std::string>("say")) {
    std::string say = pt.get<std::string>("say");
    QProcess process;
    QStringList arguments;
    arguments << "-a 200";
    arguments << "-v" + QString::fromStdString(language) + "+f5";
    arguments << QString::fromStdString(say);
    if (process.execute("espeak", arguments) != 0) {
      sendMessage("FAILED");
    }
  }
}

POCO_BEGIN_MANIFEST(HyphaPlugin)
POCO_EXPORT_CLASS(ESpeak)
POCO_END_MANIFEST
