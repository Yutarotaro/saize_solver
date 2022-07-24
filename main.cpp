#include <cmath>
#include <opencv2/opencv.hpp>
#include <vector>

#include "findSquares.hpp"
#include "homography.hpp"
#include "image.hpp"

using namespace std;

double calcDistance(cv::Point a, cv::Point b) {
  return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

int main(int argc, char** argv) {
  if (argc < 2) {
    cout << "Input Image!!" << endl;
    return 0;
  }
  cv::Mat src = cv::imread(argv[1], 1);
  cv::resize(src, src, cv::Size(0, 0), 0.3, 0.3);
  vector<vector<cv::Point>> squares;

  findSquares(src, squares);

  int squaresNum = squares.size();
  int candiIndex = -1;
  int tmpRatio = 0;

  for (int i = 0; i < N; ++i) {
    auto p0 = squares[i][0];
    auto p1 = squares[i][1];
    auto p2 = squares[i][2];

    double d1 = calcDistance(p0, p1);
    double d2 = calcDistance(p0, p2);

    if (d1 * d2 < 0.1 * src.rows * src.cols) continue;

    double ratio = max(d1, d2) / min(d1, d2);

    if (abs(ratio - 2.) < abs(2 - tmpRatio)) {
      tmpRatio = ratio;
      candiIndex = i;
    }
  }

  int a = 2400;
  int b = 1200;
  vector<cv::Point> pts_src{cv::Point2f(0, 0), cv::Point2f(a, 0),
                            cv::Point2f(a, b), cv::Point2f(0, b)};
  vector<cv::Point> pts_dst = squares[candiIndex];
  cv::Mat h = cv::findHomography(pts_src, pts_dst);

  cv::Mat dst;
  cv::warpPerspective(src, dst, h.inv(), cv::Size(a, b));

  Image left(dst(cv::Rect(0, 0, b, b)));
  Image right(dst(cv::Rect(b, 0, b, b)));

  left.detectKeyPoints();
  right.detectKeyPoints();

  cv::Mat H = getHomography(left, right);

  Image warped_left(
      cv::Mat::zeros(right.image.rows, right.image.cols, CV_8UC3));
  cv::warpPerspective(left.image, warped_left.image, H, right.image.size());

  right.cvtToGray();
  warped_left.cvtToGray();

  Image diff1(right.gray_image - warped_left.gray_image);
  Image diff2(warped_left.gray_image - right.gray_image);

  diff1.show("diff1");
  diff2.show("diff2");

  cv::Mat binned_diff1;
  cv::threshold(diff1.image, binned_diff1, 0, 255,
                CV_THRESH_BINARY | CV_THRESH_OTSU);

  cv::Mat binned_diff2;
  cv::threshold(diff2.image, binned_diff2, 0, 255,
                CV_THRESH_BINARY | CV_THRESH_OTSU);

  cv::Mat tmp;
  cv::bitwise_or(binned_diff1, binned_diff2, tmp);

  const int itr = 0;  //回数お好みで調整
  cv::erode(tmp, tmp, cv::Mat(), cv::Point(-1, -1), itr);
  cv::dilate(tmp, tmp, cv::Mat(), cv::Point(-1, -1), itr);

  cv::imshow("M", tmp);

  cv::waitKey();
}
