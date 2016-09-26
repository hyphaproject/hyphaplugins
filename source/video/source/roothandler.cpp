// Copyright (c) 2015-2016 Hypha

#include "hyphaplugins/video/roothandler.h"

using namespace hypha::plugin::video;
void RootHandler::handleRequest(Poco::Net::HTTPServerRequest &request,
                                Poco::Net::HTTPServerResponse &response) {
  response.setChunkedTransferEncoding(true);
  response.setContentType("text/html");
  std::ostream &ostr = response.send();
  ostr << "<html><head><title>Video Stream</title></head>";
  ostr << "<body>";
  ostr << "<img src=/stream/>";
  ostr << "</body></html>";
}
