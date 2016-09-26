// Copyright (c) 2015-2016 Hypha

#include "hyphaplugins/flame/flame.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <Poco/ClassLibrary.h>

#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QProcess>

#include <wiringPi.h>

#include <hypha/plugin/hyphaplugin.h>
#include <hypha/utils/logger.h>

#define PIN 0

using namespace hypha::utils;
using namespace hypha::plugin;
using namespace hypha::plugin::flame;

void Flame::doWork() {
  if (alarm) {
    measure();
    if (flame) {
      QJsonObject object;
      object["alarm"] = true;
      object["id"] = QString::fromStdString(this->id);
      object["type"] = QString::fromStdString(name());
      object["flame"] = flame;
      object["value"] = flame;
      QJsonDocument document(object);
      sendMessage(QString(document.toJson()).toStdString());
      std::this_thread::sleep_for(std::chrono::seconds(20));
    } else {
      QJsonObject object;
      object["alarm"] = false;
      object["id"] = QString::fromStdString(this->id);
      object["type"] = QString::fromStdString(name());
      object["flame"] = flame;
      object["value"] = flame;
      QJsonDocument document(object);
      sendMessage(QString(document.toJson()).toStdString());
    }
  }
  std::this_thread::sleep_for(std::chrono::seconds(1));
}

void Flame::setup() { wiringPiSetup(); }

std::string Flame::communicate(std::string UNUSED(message)) {
  return getStatusMessage();
}

std::string Flame::getStatusMessage() {
  measure();
  return flame ? "Fire" : "No Flame";
}

void Flame::loadConfig(std::string json) {
  QJsonDocument document =
      QJsonDocument::fromJson(QString::fromStdString(json).toUtf8());
  QJsonObject object = document.object();
  if (object.contains("alarm")) {
    alarm = object.value("alarm").toBool();
  }
}

std::string Flame::getConfig() { return "{}"; }

HyphaPlugin *Flame::getInstance(std::string id) {
  Flame *instance = new Flame();
  instance->setId(id);
  return instance;
}

void Flame::receiveMessage(std::string UNUSED(message)) {}

void Flame::measure() {
  measure_mutex.lock();
  this->flame = (digitalRead(PIN) == HIGH);
  measure_mutex.unlock();
}

POCO_BEGIN_MANIFEST(HyphaPlugin)
POCO_EXPORT_CLASS(Flame)
POCO_END_MANIFEST
