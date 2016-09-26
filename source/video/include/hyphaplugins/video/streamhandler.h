// Copyright (c) 2015-2016 Hypha

#ifndef STREAMHANDLER_H
#define STREAMHANDLER_H

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

namespace hypha {
namespace plugin {
namespace video {
class Video;
class StreamHandler : public Poco::Net::HTTPRequestHandler {
 public:
  void handleRequest(Poco::Net::HTTPServerRequest &request,
                     Poco::Net::HTTPServerResponse &response);
  void setVideo(Video *video);

 protected:
  Video *video;
};
}
}
}
#endif  // STREAMHANDLER_H
