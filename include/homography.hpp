#pragma once
#include <opencv2/opencv.hpp>
#include <vector>

#include "image.hpp"

cv::Mat getHomography(Image Src1, Image Src2) {
  // matching
  std::vector<std::vector<cv::DMatch>> knnmatch_points;
  cv::BFMatcher match(cv::NORM_HAMMING);
  match.knnMatch(Src1.descriptors, Src2.descriptors, knnmatch_points, 2);

  //対応点を絞る
  const double match_par = 0.80;  //候補点を残す場合のしきい値 大きいほど厳しい
                                  // ratio test
  std::vector<cv::DMatch> goodMatch;
  // KeyPoint -> Point2d
  std::vector<cv::Point2f> match_point1;
  std::vector<cv::Point2f> match_point2;

  for (size_t i = 0; i < knnmatch_points.size(); ++i) {
    double distance1 = knnmatch_points[i][0].distance;
    double distance2 = knnmatch_points[i][1].distance;

    //第二候補点から距離値が離れている点のみ抽出（いい点だけ残す）
    if (distance1 <= distance2 * match_par) {
      goodMatch.push_back(knnmatch_points[i][0]);
      match_point1.push_back(Src1.keypoints[knnmatch_points[i][0].queryIdx].pt);
      match_point2.push_back(Src2.keypoints[knnmatch_points[i][0].trainIdx].pt);
    }
  }

  // Homography 推定
  cv::Mat masks;
  cv::Mat H = cv::Mat::zeros(3, 3, CV_32F);
  try {
    H = cv::findHomography(match_point1, match_point2, masks, cv::RANSAC, 3);
  } catch (cv::Exception &e) {
    std::cerr << e.what() << std::endl;
    return H;
  }

  // RANSACで使われた対応点のみ抽出
  std::vector<cv::DMatch> inlierMatch;

  for (int i = 0; i < masks.rows; ++i) {
    uchar *inlier = masks.ptr<uchar>(i);
    if (inlier[0] == 1) {
      inlierMatch.push_back(goodMatch[i]);
    }
  }

  std::cout << "Inlierの個数: " << inlierMatch.size() << std::endl;

  //マッチングの可視化
  // cv::Mat drawmatch;
  // cv::drawMatches(Src1, keypoints1, Src2, keypoints2, goodMatch, drawmatch);

  //インライアの対応点のみ表示
  //  cv::Mat drawMatch_inlier;
  // cv::drawMatches(Src1.image, Src1.keypoints, Src2.image, Src2.keypoints,
  // inlierMatch, drawMatch_inlier);

  //  cv::imshow("drawMatch_inlier", drawMatch_inlier);

  return H;
}
