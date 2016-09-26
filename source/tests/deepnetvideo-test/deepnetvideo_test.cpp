
#include <gmock/gmock.h>
#include <hyphaplugins/deepnetvideo/deepnetvideo.h>

class deepnetvideo_test : public testing::Test {
 public:
};

GTEST_TEST(deepnetvideo_test, CheckSomeResults) {
  hypha::plugin::deepnetvideo::DeepNetVideo deepNetVideo;
  std::string config =
      "{\"fps\":5,\"width\":800,\"height\":600,\"device\":\"0\","
      "\"modelTxt\":\"data/deepnetvideo/bvlc_googlenet.prototxt\","
      "\"modelBin\":\"data/deepnetvideo/"
      "resnet34_1000_imagenet_classifier.dnn\","
      "\"classNamesFile\":\"data/deepnetvideo/synset_words.txt\"}";
  EXPECT_EQ("deepnetvideo", deepNetVideo.name());
  EXPECT_NO_THROW(deepNetVideo.loadConfig(config));
  EXPECT_EQ(800, deepNetVideo.width);
  EXPECT_NO_THROW(deepNetVideo.setup());
  for (int i = 0; i < 100; ++i) {
    EXPECT_NO_THROW(deepNetVideo.doWork());
  }
}
