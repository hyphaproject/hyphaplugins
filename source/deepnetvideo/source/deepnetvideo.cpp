// Copyright (c) 2015-2016 Hypha

#include "hyphaplugins/deepnetvideo/deepnetvideo.h"

#include <cmath>
#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <thread>
#include <vector>

#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include <dlib/opencv.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

#include <Poco/ClassLibrary.h>
#include <Poco/DateTime.h>
#include <Poco/DateTimeFormatter.h>
#include <Poco/LocalDateTime.h>
#include <Poco/Timezone.h>

#include <hypha/plugin/hyphaplugin.h>
#include <hypha/utils/logger.h>

using namespace hypha::utils;
using namespace hypha::plugin;
using namespace hypha::plugin::deepnetvideo;

using namespace cv;
using namespace cv::dnn;

using namespace std;

DeepNetVideo::~DeepNetVideo() {}

/* Find best class for the blob (i. e. class with maximal probability) */
void getMaxClass(dnn::Blob &probBlob, int *classId, double *classProb) {
  Mat probMat = probBlob.matRefConst().reshape(
      1, 1);  // reshape the blob to 1x1000 matrix
  Point classNumber;
  minMaxLoc(probMat, NULL, classProb, NULL, &classNumber);
  *classId = classNumber.x;
}

std::vector<String> readClassNames(const char *filename = "synset_words.txt") {
  std::vector<String> classNames;
  std::ifstream fp(filename);
  if (!fp.is_open()) {
    std::cerr << "File with classes labels not found: " << filename
              << std::endl;
    throw std::runtime_error("File with classes labels not found");
  }
  std::string name;
  while (!fp.eof()) {
    std::getline(fp, name);
    if (name.length()) classNames.push_back(name.substr(name.find(' ') + 1));
  }
  fp.close();
  return classNames;
}

void DeepNetVideo::doWork() {
  std::this_thread::sleep_for(std::chrono::seconds(1) / fps);
  try {
    std::thread captureT = std::thread([this] { captureCamera(); });
    if (captureT.joinable()) captureT.join();
    currentImage_mutex.lock();
    currentImage = captureMat;
    currentImage_mutex.unlock();
  } catch (cv::Exception &e) {
    Logger::error(e.what());
  } catch (std::exception e) {
    Logger::error(e.what());
  } catch (...) {
    Logger::error("Something terrible happened.");
  }

  std::vector<std::string> classes = classify();
  Logger::info("current class: ");
  for (std::string className : classes) Logger::info(className);
  // sendMessage(className);
}

void DeepNetVideo::captureCamera() {
  if (getFilmState() == FILM) {
    if (filmCounter++ >= 100) setFilmState(IDLE);
    if (capture_mutex.try_lock()) {
      try {
        cv::Mat capt;
        if (capture.isOpened()) {
          // read five times to clear buffered old images
          capture.read(capt);
          capture.read(capt);
          capture.read(capt);
          capture.read(capt);
          capture.read(capt);
        }
        if (captureMat_mutex.try_lock()) {
          captureMat = capt.clone();
          captureMat_mutex.unlock();
        }
        capture_mutex.unlock();
      } catch (cv::Exception &e) {
        Logger::error("Could not capture from camera");
        Logger::error(e.what());
      } catch (std::exception &e) {
        Logger::warning("Could not capture from camera");
      }
    }
  }
}

cv::Mat DeepNetVideo::rotateMat(cv::Mat mat) {
  if (mat.rows < 1) return mat;
  cv::Point2f src_center(mat.cols / 2.0F, mat.rows / 2.0F);
  cv::Mat rot_mat = cv::getRotationMatrix2D(src_center, 180, 1.0);
  cv::warpAffine(mat, mat, rot_mat, mat.size());
  return mat;
}

cv::Mat DeepNetVideo::fillMat(cv::Mat mat) {
  if (mat.rows != height)
    cv::vconcat(mat, cv::Mat(height - mat.rows, mat.cols, mat.type()), mat);
  if (mat.cols != width)
    cv::hconcat(mat, cv::Mat(mat.rows, width - mat.cols, mat.type()), mat);
  return mat;
}

void DeepNetVideo::setup() {
  if (!device.empty()) {
    capture.open(std::stoi(device));
    capture.set(CV_CAP_PROP_FRAME_WIDTH, width);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT, height);
    capture.set(CV_CAP_PROP_FPS, fps);
    captureMat = cv::Mat(height, width, CV_8UC3);
  }
  filmCounter = 0;
  filmState = FILM;

  loadNet();
}

std::string DeepNetVideo::communicate(std::string UNUSED(message)) {
  return "SUCCESS";
}

void DeepNetVideo::loadConfig(std::string json) {
  boost::property_tree::ptree pt;
  std::stringstream ss(json);
  boost::property_tree::read_json(ss, pt);
  if (pt.get_optional<int>("fps")) {
    fps = pt.get<int>("fps");
  }
  if (pt.get_optional<int>("width")) {
    width = pt.get<int>("width");
  }
  if (pt.get_optional<int>("height")) {
    height = pt.get<int>("height");
  }
  if (pt.get_optional<std::string>("device")) {
    device = pt.get<std::string>("device");
  }

  if (pt.get_optional<std::string>("modelTxt")) {
    modelTxt = pt.get<std::string>("modelTxt");
  }

  if (pt.get_optional<std::string>("modelBin")) {
    modelBin = pt.get<std::string>("modelBin");
  }

  if (pt.get_optional<std::string>("classNamesFile")) {
    classNamesFile = pt.get<std::string>("classNamesFile");
  }
}

std::string DeepNetVideo::getConfig() { return "{}"; }

HyphaPlugin *DeepNetVideo::getInstance(std::string id) {
  DeepNetVideo *instance = new DeepNetVideo();
  instance->setId(id);
  return instance;
}

void DeepNetVideo::receiveMessage(std::string message) {
  try {
    boost::property_tree::ptree pt;
    std::stringstream ss(message);
    boost::property_tree::read_json(ss, pt);
    if (pt.get_optional<bool>("run")) {
      if (pt.get<bool>("run")) {
        setState(FILM);
      } else {
        setState(IDLE);
      }
    }
  } catch (std::exception &e) {
    Logger::error(e.what());
  }
}

void DeepNetVideo::loadNet() {
  deserialize(this->modelBin) >> net >> labels;
  snet.subnet() = net.subnet();
}

// ----------------------------------------------------------------------------------------

dlib::rectangle make_random_cropping_rect_resnet(const matrix<rgb_pixel> &img,
                                                 dlib::rand &rnd) {
  // figure out what rectangle we want to crop from the image
  double mins = 0.466666666, maxs = 0.875;
  auto scale = mins + rnd.get_random_double() * (maxs - mins);
  auto size = scale * std::min(img.nr(), img.nc());
  dlib::rectangle rect(size, size);
  // randomly shift the box around
  point offset(rnd.get_random_32bit_number() % (img.nc() - rect.width()),
               rnd.get_random_32bit_number() % (img.nr() - rect.height()));
  return move_rect(rect, offset);
}

// ----------------------------------------------------------------------------------------

void randomly_crop_images(const matrix<rgb_pixel> &img,
                          dlib::array<matrix<rgb_pixel>> &crops,
                          dlib::rand &rnd, long num_crops) {
  std::vector<chip_details> dets;
  for (long i = 0; i < num_crops; ++i) {
    auto rect = make_random_cropping_rect_resnet(img, rnd);
    dets.push_back(chip_details(rect, chip_dims(227, 227)));
  }

  extract_image_chips(img, dets, crops);

  for (auto &&img : crops) {
    // Also randomly flip the image
    if (rnd.get_random_double() > 0.5) img = fliplr(img);

    // And then randomly adjust the colors.
    apply_random_color_offset(img, rnd);
  }
}

dlib::matrix<rgb_pixel> MatToMatrix(cv::Mat mat) {
  dlib::matrix<rgb_pixel> result(mat.rows, mat.cols);

  rgb_pixel *p;
  for (int i = 0; i < mat.rows; i++) {
    p = mat.ptr<rgb_pixel>(i);
    for (int j = 0; j < mat.cols; j++) {
      result(i, j) = p[j];
    }
  }

  return result;
}

// ----------------------------------------------------------------------------------------

std::vector<std::string> DeepNetVideo::classify() {
  std::vector<std::string> tags;
  Mat cvimg = getCurrentImage();
  if (cvimg.empty()) {
    std::cerr << "can't get image" << std::endl;
    throw std::runtime_error("can't get image");
  }
  // cv_image<bgr_pixel> cimg(img);
  dlib::array<matrix<rgb_pixel>> images;
  matrix<rgb_pixel> mimg, crop;
  cv::cvtColor(cvimg, cvimg, CV_BGR2RGB);
  mimg = MatToMatrix(cvimg);
  const int num_crops = 8;
  // Grab 16 random crops from the image.  We will run all of them through the
  // network and average the results.
  randomly_crop_images(mimg, images, rnd, num_crops);
  // p(i) == the probability the image contains object of class i.
  matrix<float, 1, 1000> p =
      sum_rows(dlib::mat(snet(images.begin(), images.end()))) / num_crops;

  win.set_image(mimg);
  // Print the 5 most probable labels
  for (int k = 0; k < 1; ++k) {
    unsigned long predicted_label = index_of_max(p);
    cout << p(predicted_label) << ": " << labels[predicted_label] << endl;
    p(predicted_label) = 0;
  }
  return tags;
}

cv::Mat DeepNetVideo::getCurrentImage() {
  setFilmState(FILM);
  cv::Mat frame;
  currentImage_mutex.lock();
  frame = currentImage.clone();
  currentImage_mutex.unlock();
  return frame;
}

DeepNetVideo::State DeepNetVideo::getState() {
  State state = IDLE;
  state_mutex.lock();
  state = currentState;
  state_mutex.unlock();
  return state;
}

void DeepNetVideo::setState(State state) {
  state_mutex.lock();
  currentState = state;
  state_mutex.unlock();
}

DeepNetVideo::State DeepNetVideo::getFilmState() {
  State state = IDLE;
  state_mutex.lock();
  state = filmState;
  state_mutex.unlock();
  return state;
}

void DeepNetVideo::setFilmState(State state) {
  state_mutex.lock();
  if (filmState == IDLE && state == FILM) {
    filmState = FILM;
    if (!device.empty()) {
      capture.open(std::stoi(device));
      capture.set(CV_CAP_PROP_FRAME_WIDTH, width);
      capture.set(CV_CAP_PROP_FRAME_HEIGHT, height);
      capture.set(CV_CAP_PROP_FPS, fps);
      cameras++;
    }
  } else if (filmState == FILM && state == IDLE) {
    filmState = IDLE;
    if (!device.empty()) {
      capture.release();
    }
    cameras = 0;
  }
  state_mutex.unlock();
}

POCO_BEGIN_MANIFEST(HyphaPlugin)
POCO_EXPORT_CLASS(DeepNetVideo)
POCO_END_MANIFEST
