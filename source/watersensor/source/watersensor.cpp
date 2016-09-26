// Copyright (c) 2015-2016 Hypha

#include "hyphaplugins/watersensor/watersensor.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

#include <Poco/ClassLibrary.h>

#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QProcess>

#include <hypha/plugin/hyphaplugin.h>
#include <hypha/utils/logger.h>

using namespace hypha::utils;
using namespace hypha::plugin;
using namespace hypha::plugin::watersensor;

void WaterSensor::doWork() {
  if (alarm) {
    measure();
    if (water) {
      QJsonObject object;
      object["alarm"] = true;
      object["id"] = QString::fromStdString(this->id);
      object["type"] = QString::fromStdString(name());
      object["water"] = water;
      object["value"] = water;
      QJsonDocument document(object);
      sendMessage(QString(document.toJson()).toStdString());
      std::this_thread::sleep_for(std::chrono::seconds(20));
    } else {
      QJsonObject object;
      object["alarm"] = false;
      object["id"] = QString::fromStdString(this->id);
      object["type"] = QString::fromStdString(name());
      object["water"] = water;
      object["value"] = water;
      QJsonDocument document(object);
      sendMessage(QString(document.toJson()).toStdString());
    }
  }
  std::this_thread::sleep_for(std::chrono::seconds(1));
}

void WaterSensor::setup() { wiringPiSetup(); }

std::string WaterSensor::communicate(std::string UNUSED(message)) {
  return getStatusMessage();
}

std::string WaterSensor::getStatusMessage() {
  measure();
  return water ? "Water" : "Dry";
}

void WaterSensor::loadConfig(std::string json) {
  QJsonDocument document =
      QJsonDocument::fromJson(QString::fromStdString(json).toUtf8());
  QJsonObject object = document.object();
  if (object.contains("alarm")) {
    alarm = object.value("alarm").toBool();
  }
  if (object.contains("pin")) {
    PIN = object.value("pin").toInt(6);
  }
}

std::string WaterSensor::getConfig() { return "{}"; }

HyphaPlugin *WaterSensor::getInstance(std::string id) {
  WaterSensor *instance = new WaterSensor();
  instance->setId(id);
  return instance;
}

void WaterSensor::receiveMessage(std::string UNUSED(message)) {}

void WaterSensor::measure() {
  measure_mutex.lock();
  this->water = (digitalRead(PIN) == HIGH);
  measure_mutex.unlock();
}

POCO_BEGIN_MANIFEST(HyphaPlugin)
POCO_EXPORT_CLASS(WaterSensor)
POCO_END_MANIFEST
