// Copyright (c) 2015-2016 Hypha

#include "hyphaplugins/video/streamserver.h"
#include "hyphaplugins/video/requesthandlerfactory.h"

using namespace hypha::plugin::video;
StreamServer::StreamServer() {}

StreamServer::~StreamServer() {
  stop();
  delete srv;
}

void StreamServer::start() {
  if (!srv) {
    Poco::UInt16 port = this->port;
    Poco::Net::HTTPServerParams *params = new Poco::Net::HTTPServerParams;
    params->setMaxQueued(10);
    params->setMaxThreads(4);
    Poco::Net::ServerSocket socket(port);
    RequestHandlerFactory *factory = new RequestHandlerFactory();
    factory->setVideo(video);
    srv = new Poco::Net::HTTPServer(factory, socket, params);
  }
  srv->start();
}

void StreamServer::stop() { srv->stop(); }

void StreamServer::setPort(int port) { this->port = port; }

void StreamServer::setVideo(Video *video) { this->video = video; }
