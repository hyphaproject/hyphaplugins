// Copyright (c) 2015-2016 Hypha

#ifndef STREAMSERVER_H
#define STREAMSERVER_H

#include <Poco/Net/HTTPServer.h>
namespace hypha {
namespace plugin {
namespace video {
class Video;
class StreamServer {
 public:
  StreamServer();
  ~StreamServer();

  void start();
  void stop();
  void setPort(int port);
  void setVideo(Video *video);

 protected:
  Poco::Net::HTTPServer *srv = 0;
  int port;
  Video *video = 0;
};
}
}
}
#endif  // STREAMSERVER_H
