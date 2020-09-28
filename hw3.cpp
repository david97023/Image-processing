#include<opencv2\opencv.hpp>
#include<math.h>
#define X_SIZE 256
#define Y_SIZE 256
#define RAND_MAX 32767
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
//去除雜訊(移動平均法)
void smooth(unsigned char image_in[Y_SIZE][X_SIZE], unsigned char image_out[Y_SIZE][X_SIZE], int type) {
	int i, j, m, n, k, x, y;
	double sum, num;
	k = type / 2;
	num = (double)type * type;
	for ( i = 0; i < Y_SIZE; i++) {
		for ( j = 0; j < X_SIZE; j++) {
			sum = 0.0;
			for ( m = -k;  m <= k ;  m++){
				for ( n = -k; n <= k; n++){
					y = i + m;
					x = j + n;
					if (y < 0) { y = 0; }
					if (x < 0) { x = 0; }
					if (y > Y_SIZE) { y = Y_SIZE; }
					if (x > X_SIZE) { x = X_SIZE; }
					sum += image_in[y][x];
				}
			}
			sum = sum / num;
			if (sum<0) { sum = 0; }
			if (sum>255) { sum = 255; }
			image_out[i][j] = (unsigned char)sum;
		}
	}
}
//附加隨機雜訊
void noise_rand(unsigned char image_in[Y_SIZE][X_SIZE], unsigned char image_out[Y_SIZE][X_SIZE], int level) {
	int i, j, data, noise;
	for ( i = 0; i < Y_SIZE; i++) {
		for ( j = 0; j < X_SIZE; j++) {
			noise = (int)((rand() / (double)RAND_MAX - 0.5) * level * 2.0);
			data = image_in[i][j] + noise;
			if (data > 255) { image_out[i][j] = 255; }
			else if (data < 0) { image_out[i][j] = 0; }
			else { image_out[i][j] = data; }
		}
	}
}
int main() {
	Mat image = imread("lenna.jpg" , 0);
	Mat imgout(Y_SIZE, X_SIZE, CV_8UC3, Scalar(0, 0, 0));
	uchar im[Y_SIZE][X_SIZE];
	uchar im_o[Y_SIZE][X_SIZE];
	ConvertToUchar(image, im);
	noise_rand(im, im_o, 50);
	imgout = ConvertToMat(image, im_o);
	imshow("noise", imgout);
	smooth(im_o, im_o, 7);
	imgout = ConvertToMat(image, im_o);
	imshow("smooth", imgout);
	waitKey(0);
}
