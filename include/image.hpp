#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

class Image {
 public:
  cv::Mat image, original, gray_image;
  // Homography計算用
  std::vector<cv::KeyPoint> keypoints;
  cv::Mat descriptors;
  cv::Ptr<cv::Feature2D> feature;

  Image() {}

  Image(cv::Mat temp) {
    image = temp;
    original = temp;
  }

  Image(std::string path, int color = cv::IMREAD_COLOR) {
    image = cv::imread(path, color);
    if (image.empty()) {
      std::cout << path << " image not found" << std::endl;
      throw 1;
    }
    image.copyTo(original);
  }

  //可視化用
  void show(std::string name = "tmp") { cv::imshow(name, image); };

  //特徴点計算
  void detectKeyPoints() {
    feature = cv::AKAZE::create();
    feature->detectAndCompute(image, cv::Mat(), keypoints, descriptors);
  }

  void cvtToGray(cv::Mat img) {
    cv::cvtColor(img, gray_image, cv::COLOR_BGR2GRAY);
  }
  void cvtToGray() { cv::cvtColor(image, gray_image, cv::COLOR_BGR2GRAY); }
};
