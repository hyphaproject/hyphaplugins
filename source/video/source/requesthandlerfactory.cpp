// Copyright (c) 2015-2016 Hypha

#include "hyphaplugins/video/requesthandlerfactory.h"

using namespace hypha::plugin::video;
RequestHandlerFactory::RequestHandlerFactory() {}

Poco::Net::HTTPRequestHandler *RequestHandlerFactory::createRequestHandler(
    const Poco::Net::HTTPServerRequest &request) {
  if (boost::starts_with(request.getURI(), "/stream/")) {
    StreamHandler *sh = new StreamHandler();
    sh->setVideo(video);
    return sh;
  } else
    return new RootHandler();
}

void RequestHandlerFactory::setVideo(Video *video) { this->video = video; }
