// Copyright (c) 2015-2016 Hypha

#ifndef VIDEO_H
#define VIDEO_H

#include <hypha/plugin/hyphaplugin.h>
#include <mutex>
#include <opencv2/opencv.hpp>
#include <string>

namespace hypha {
namespace plugin {
namespace video {
class StreamServer;
class Video : public HyphaPlugin {
#define MAXCAMERAS 4

 public:
  void doWork();
  void setup();
  std::string communicate(std::string message);
  const std::string name() { return "video"; }
  const std::string getTitle() { return "Video"; }
  const std::string getVersion() { return "0.1"; }
  const std::string getDescription() {
    return "Plugin to capture videos from webcam.";
  }
  const std::string getConfigDescription() { return "{}"; }
  void loadConfig(std::string json);
  std::string getConfig();
  HyphaPlugin *getInstance(std::string id);

  void receiveMessage(std::string message);

  void startVideoCapture();
  void stopVideoCapture();
  cv::Mat getCurrentImage();
  std::string getUser();
  std::string getPassword();

 protected:
  enum State { IDLE, FILM, RECORDING };

  int cameras = 0;
  int fps = 5;
  int width = 320;
  int height = 240;
  int port = 8080;
  bool doCapture = false;
  bool findPerson = false;

  std::mutex user_mutex;
  std::string user = "";
  std::mutex password_mutex;
  std::string password = "";

  int frameCounter = 0;
  int filmCounter = 0;

  std::string device[MAXCAMERAS];
  bool upsideDown[MAXCAMERAS];

  std::mutex capture_mutex[MAXCAMERAS];
  cv::VideoCapture capture[MAXCAMERAS];

  std::mutex videoWriter_mutex;
  cv::VideoWriter *videoWriter = 0;

  std::mutex hog_mutex[MAXCAMERAS];
  cv::HOGDescriptor hog[MAXCAMERAS];

  std::mutex captureMat_mutex[MAXCAMERAS];
  cv::Mat captureMat[MAXCAMERAS];

  std::mutex currentImage_mutex;
  cv::Mat currentImage;

  StreamServer *srv = 0;

  void captureCameras();
  void captureCamera(int camNumber);
  void findPersons();
  std::mutex findPerson_mutex[MAXCAMERAS];
  void findPersonInCamera(int camNumber);

  cv::Mat rotateMat(cv::Mat mat);
  cv::Mat fillMat(cv::Mat mat);

  void concatImages();
  void drawText();
  void writeVideo();

  State currentState = IDLE;
  State filmState = IDLE;
  std::mutex state_mutex;

  State getState();
  void setState(State state);

  State getFilmState();
  void setFilmState(State state);

  std::string fileName;
  std::mutex fileName_mutex;
  std::string getFileName();
  void setFileName(std::string filename);
};
}
}
}
#endif  // ESPEAK_H
