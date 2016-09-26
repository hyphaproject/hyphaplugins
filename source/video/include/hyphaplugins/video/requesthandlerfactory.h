// Copyright (c) 2015-2016 Hypha

#ifndef REQUESTHANDLERFACTORY_H
#define REQUESTHANDLERFACTORY_H

#include <boost/algorithm/string/predicate.hpp>

#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPServerRequest.h>

#include "roothandler.h"
#include "streamhandler.h"

namespace hypha {
namespace plugin {
namespace video {
class Video;
class RequestHandlerFactory : public Poco::Net::HTTPRequestHandlerFactory {
 public:
  RequestHandlerFactory();

  Poco::Net::HTTPRequestHandler *createRequestHandler(
      const Poco::Net::HTTPServerRequest &request);

  void setVideo(Video *video);

 protected:
  Video *video;
};
}
}
}

#endif  // REQUESTHANDLERFACTORY_H
