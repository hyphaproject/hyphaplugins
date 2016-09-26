// Copyright (c) 2015-2016 Hypha

#ifndef PYTHONPLUGIN_H
#define PYTHONPLUGIN_H

#include <hypha/plugin/hyphaplugin.h>
#include <boost/python.hpp>
#include <string>

namespace hypha {
namespace plugin {
namespace pythonplugin {
class PythonPlugin : public HyphaPlugin {
 public:
  void doWork();
  void setup();
  const std::string name() { return "pythonplugin"; }
  const std::string getTitle() { return "Python Plugin"; }
  const std::string getVersion() { return "0.1"; }
  const std::string getDescription() { return "Plugin to use Python code."; }
  const std::string getConfigDescription() { return "{}"; }
  void loadConfig(std::string json);
  std::string getConfig();
  HyphaPlugin *getInstance(std::string id);

  void receiveMessage(std::string message);
  std::string communicate(std::string message);
  std::string parse_python_exception();

 protected:
  std::string config;
  std::string pythonmodule;
  std::string pythonclass;
  boost::python::object main;
  boost::python::object global;
  boost::python::object result;
  boost::python::object py_class;
};
}
}
}
#endif  // PYTHONPLUGIN_H
