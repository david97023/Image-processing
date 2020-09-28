#include<opencv2\opencv.hpp>
#include<math.h>
#define X_SIZE 256
#define Y_SIZE 256
using namespace cv;
Mat ConvertToMat(Mat img, uchar m[Y_SIZE][X_SIZE]) {
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			img.at<uchar>(i, j) = m[i][j];
		}
	}return img;
}
void ConvertToUchar(Mat img, uchar m[Y_SIZE][X_SIZE]) {
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			m[i][j] = img.at<uchar>(i, j);
		}
	}
}
//調整亮度
void amplify(unsigned char image_in[Y_SIZE][X_SIZE], unsigned char image_out[Y_SIZE][X_SIZE], double a, double b) {
	int i, j, d;
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			d = (int)(image_in[i][j] * a + b);
			if (d < 0) { d = 0; }
			if (d > 255) { d = 255; }
			image_out[i][j] = (unsigned char)d;
		}
	}
}
//求出亮度範圍
void range(unsigned char image_in[Y_SIZE][X_SIZE], int* fmax, int* fmin) {
	int i, j, n;
	*fmax = 0;
	*fmin = 255;
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			n = (int)image_in[i][j];
			if (n > * fmax) { *fmax = n; }
			if (n < *fmin) { *fmin = n; }
		}
	}
}
//擴大亮度範圍
void expand(unsigned char image_in[Y_SIZE][X_SIZE], unsigned char image_out[Y_SIZE][X_SIZE], int fmax, int fmin) {
	double a, b;
	a = 255.0 / (double)(fmax - fmin);
	b = -255.0 * fmin / (double)(fmax - fmin);
	amplify(image_in, image_out, a, b);
}
int main() {
	Mat image = imread("lenna.jpg", 0);
	Mat imgout(Y_SIZE, X_SIZE, CV_8UC3, Scalar(0, 0, 0));
	uchar im[Y_SIZE][X_SIZE];
	uchar im_o[Y_SIZE][X_SIZE];
	int max = 0, min = 0;
	imshow("original", image);

	ConvertToUchar(image, im);
	amplify(im, im_o, 2, 0);
	imgout = ConvertToMat(image, im_o);
	imshow("amplify", imgout);

	ConvertToUchar(image, im);
	range(im, &max, &min);
	std::cout << max << "&" << min;
	imgout = ConvertToMat(image, im);

	ConvertToUchar(image, im);
	expand(im, im_o, max, min);
	imgout = ConvertToMat(image, im_o);
	imshow("expand", imgout);
	waitKey(0);
}
