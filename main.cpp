#include <cmath>
#include <opencv2/opencv.hpp>
#include <vector>

#include "findSquares.hpp"

using namespace std;

double calcDistance(cv::Point a, cv::Point b) {
  return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

int main() {
  cv::Mat src = cv::imread("../saize.png", 1);
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

  // TODO:homography変換

  // drawSquares(src, squares[candiIndex]);
  // cv::waitKey();
}
