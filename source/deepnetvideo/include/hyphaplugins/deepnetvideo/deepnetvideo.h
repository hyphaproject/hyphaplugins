// Copyright (c) 2015-2016 Hypha

#pragma once

#include <mutex>
#include <opencv2/dnn.hpp>
#include <opencv2/opencv.hpp>
#include <string>

#include <dlib/data_io.h>
#include <dlib/dnn.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_transforms.h>
#include <iostream>

#include <hypha/plugin/hyphaplugin.h>

using namespace dlib;

namespace hypha {
namespace plugin {
namespace deepnetvideo {
class StreamServer;
class DeepNetVideo : public HyphaPlugin {
  // ----------------------------------------------------------------------------------------

  // This block of statements defines the resnet-34 network

  template <template <int, template <typename> class, int, typename>
            class block,
            int N, template <typename> class BN, typename SUBNET>
  using residual = add_prev1<block<N, BN, 1, tag1<SUBNET>>>;

  template <template <int, template <typename> class, int, typename>
            class block,
            int N, template <typename> class BN, typename SUBNET>
  using residual_down = add_prev2<
      avg_pool<2, 2, 2, 2, skip1<tag2<block<N, BN, 2, tag1<SUBNET>>>>>>;

  template <int N, template <typename> class BN, int stride, typename SUBNET>
  using block =
      BN<con<N, 3, 3, 1, 1, relu<BN<con<N, 3, 3, stride, stride, SUBNET>>>>>;

  template <int N, typename SUBNET>
  using ares = relu<residual<block, N, affine, SUBNET>>;
  template <int N, typename SUBNET>
  using ares_down = relu<residual_down<block, N, affine, SUBNET>>;

  using anet_type = loss_multiclass_log<fc<
      1000,
      avg_pool_everything<ares<
          512,
          ares<
              512,
              ares_down<
                  512,
                  ares<
                      256,
                      ares<
                          256,
                          ares<
                              256,
                              ares<
                                  256,
                                  ares<
                                      256,
                                      ares_down<
                                          256,
                                          ares<
                                              128,
                                              ares<
                                                  128,
                                                  ares<
                                                      128,
                                                      ares_down<
                                                          128,
                                                          ares<
                                                              64,
                                                              ares<
                                                                  64,
                                                                  ares<
                                                                      64,
                                                                      max_pool<
                                                                          3, 3,
                                                                          2, 2,
                                                                          relu<affine<con<
                                                                              64,
                                                                              7,
                                                                              7,
                                                                              2,
                                                                              2,
                                                                              input_rgb_image_sized<
                                                                                  227>>>>>>>>>>>>>>>>>>>>>>>>;

  // ----------------------------------------------------------------------------------------

 public:
  virtual ~DeepNetVideo();

  void doWork();
  void setup();
  std::string communicate(std::string message);
  const std::string name() { return "deepnetvideo"; }
  const std::string getTitle() { return "DeepNetVideo"; }
  const std::string getVersion() { return "0.1"; }
  const std::string getDescription() {
    return "Plugin to detect objects with a camera.";
  }
  const std::string getConfigDescription() { return "{}"; }
  void loadConfig(std::string json);
  std::string getConfig();
  HyphaPlugin *getInstance(std::string id);

  void receiveMessage(std::string message);

  void loadNet();
  std::vector<std::string> classify();

  cv::Mat getCurrentImage();

  enum State { IDLE, FILM };

  int cameras = 0;
  int fps = 5;
  int width = 320;
  int height = 240;
  bool doCapture = false;

  int frameCounter = 0;
  int filmCounter = 0;

 protected:
  std::string device = "0";
  bool upsideDown = false;

  std::string modelTxt = "data/deepnetvideo/bvlc_googlenet.prototxt";
  std::string modelBin = "data/deepnetvideo/bvlc_googlenet.caffemodel";
  std::string classNamesFile = "data/deepnetvideo/synset_words.txt";

  int classId;
  double classProb;

  std::vector<std::string> labels;
  anet_type net;
  softmax<anet_type::subnet_type> snet;
  dlib::rand rnd;
  dlib::image_window win;

  std::mutex capture_mutex;
  cv::VideoCapture capture;

  std::mutex captureMat_mutex;
  cv::Mat captureMat;

  std::mutex currentImage_mutex;
  cv::Mat currentImage;

  void captureCamera();

  cv::Mat rotateMat(cv::Mat mat);
  cv::Mat fillMat(cv::Mat mat);

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
