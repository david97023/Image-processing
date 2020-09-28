#include<opencv2\opencv.hpp>
#include<iostream>
using namespace cv;
using namespace std;
int main() {
	int thresh, hist[256], histsize = 256, histWidth = 256, histHeight = 256;
	float hranges[] = { 0, 256 };
	const float* ranges[] = { hranges };
	double minValue = 0, maxValue = 0;
	cout << "輸入臨界值 :";
	cin >> thresh;
	Mat img = imread("lenna.jpg", 0), grayHist;
	imshow("Gray", img);
//計算值方圖
	calcHist(&img, 1, 0, Mat(), grayHist, 1, &histsize, ranges, true, false);
	Mat histImage(histWidth, histHeight, CV_8U, Scalar(0));
	minMaxLoc(grayHist, &minValue, &maxValue, 0, 0);
	int hpt = saturate_cast<int>(0.9 * histsize);
	for (int i = 0; i < histsize; i++) {
		float binValue = grayHist.at<float>(i);
		int realValue = saturate_cast<int>(binValue * hpt / maxValue);
		line(histImage, Point(i, histHeight - 1), Point(i, histHeight - realValue), Scalar(255));
	}
	imshow("Histogram", histImage);

	for (int i = 0; i < histsize; i++) {
		hist[i] = 0;
	}
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			hist[img.at<uchar>(i, j)] += 1;
			if (img.at<uchar>(i, j) > thresh) {
				img.at<uchar>(i, j) = 255;
			}
			else {
				img.at<uchar>(i, j) = 0;
			}
		}
	}
	for (int i = 0; i < 256; i++) {
		cout << "pixel " << i << " 數量 : " << hist[i] << endl;
	}
	imshow("Binarization", img);
	waitKey(0);
}
