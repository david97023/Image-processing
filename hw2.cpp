#include<opencv2\opencv.hpp>
#include<math.h>
#define X_SIZE 256
#define Y_SIZE 256
#define TMP 128
int cx_d[9] = { 0,0,0,0,1,-1,0,0,0 };
int cy_d[9] = { 0,0,0,0,1,0,0,-1,0 };
int cx_r[9] = { 0,0,0,0,1,0,0,0,-1 };
int cy_r[9] = { 0,0,0,0,0,1,0,-1,0 };
int cx_s[9] = { -1,0,1,-2,0,2,-1,0,1 };
int cy_s[9] = { -1,-2,-1,0,0,0,1,2,1 };
int ncon(int p[9]) {
	int i, i1, i2;
	int q[9];
	int n = 0;
	for (i = 0; i < 9; i++) {
		if ((p[i] == 1) || (p[i] == -1)) { q[i] = 0; }
		else { q[i] = 1; }
	}
	for (i = 0; i < 9; i += 2) {
		i1 = i + 1;
		i2 = i + 2;
		if (i2 == 9) { i2 = 1; }
		n = n + q[i] - q[i] * q[i1] * q[i2];
	}return n;
}

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
//二值化
void threshold(unsigned char image_in[Y_SIZE][X_SIZE], unsigned char image_out[Y_SIZE][X_SIZE], int thresh, int type) {
	int i, j;
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			switch (type) {
			case 2:
				if ((int)image_in[i][j] <= thresh) { image_out[i][j] = 255; }
				else { image_out[i][j] = 0; }
				break;
			default:
				if ((int)image_in[i][j] >= thresh) { image_out[i][j] = 255; }
				else { image_out[i][j] = 0; }
				break;
			}
		}
	}
}
//一階微分擷取輪廓
void gradient(unsigned char im_in[Y_SIZE][X_SIZE], unsigned char im_ot[Y_SIZE][X_SIZE], double amp, int cx[9], int cy[9]) {
	int d[9];
	int i, j, dat;
	double  xx, yy, zz;
	for (i = 1; i < Y_SIZE - 1; i++) {
		for (j = 1; j < X_SIZE - 1; j++) {
			d[0] = im_in[i - 1][j - 1];
			d[1] = im_in[i - 1][j];
			d[2] = im_in[i - 1][j + 1];
			d[3] = im_in[i][j - 1];
			d[4] = im_in[i][j];
			d[5] = im_in[i][j + 1];
			d[6] = im_in[i + 1][j - 1];
			d[7] = im_in[i + 1][j];
			d[8] = im_in[i + 1][j + 1];
			xx = (double)(cx[0] * d[0] + cx[1] * d[1] + cx[2] * d[2] + cx[3] * d[3] + cx[4] * d[4] + cx[5] * d[5] + cx[6] * d[6] + cx[7] * d[7] + cx[8] * d[8]);
			yy = (double)(cy[0] * d[0] + cy[1] * d[1] + cy[2] * d[2] + cy[3] * d[3] + cy[4] * d[4] + cy[5] * d[5] + cy[6] * d[6] + cy[7] * d[7] + cy[8] * d[8]);
			zz = (double)(amp * sqrt(xx * xx + yy * yy));
			dat = (int)zz;
			if (dat > 255) { dat = 255; }
			im_ot[i][j] = (char)dat;
		}
	}
}
//通常得差分方法
void gradient_difference(unsigned char im_in[Y_SIZE][X_SIZE], unsigned char im_ot[Y_SIZE][X_SIZE], double amp) {
	gradient(im_in, im_ot, amp, cx_d, cy_d);
}
//Roberts方法
void gradient_roberts(unsigned char im_in[Y_SIZE][X_SIZE], unsigned char im_ot[Y_SIZE][X_SIZE], double amp) {
	gradient(im_in, im_ot, amp, cx_r, cy_r);
}
//Sobel方法
void gradient_sobel(unsigned char im_in[Y_SIZE][X_SIZE], unsigned char im_ot[Y_SIZE][X_SIZE], double amp) {
	gradient(im_in, im_ot, amp, cx_s, cy_s);
}
//二值影像細線化
void thinning(unsigned char image_in[Y_SIZE][X_SIZE], unsigned char image_out[Y_SIZE][X_SIZE]) {
	int flg = 1;
	int i, j, k, n;
	int p[9];
	for (i = 0; i < Y_SIZE; i++) {
		for (j = 0; j < X_SIZE; j++) {
			image_out[i][j] = image_in[i][j];
		}
	}
	while (flg != 0) {
		flg = 0;
		for (i = 0; i < Y_SIZE - 1; i++) {
			for (j = 0; j < X_SIZE - 1; j++) {
				p[0] = image_out[i][j];
				p[1] = image_out[i][j+1];
				p[2] = image_out[i-1][j+1];
				p[3] = image_out[i-1][j];
				p[4] = image_out[i-1][j-1];
				p[5] = image_out[i][j-1];
				p[6] = image_out[i+1][j-1];
				p[7] = image_out[i+1][j];
				p[8] = image_out[i+1][j+1];
				for (k = 0; k < 9; k++) {
					if (p[k] == 255) { p[k] = 1; }
					else if (p[k] == 0) { p[k] = 0; }
					else { p[k] = -1; }
				}
				if (p[0] != 1) { continue; }
				if (p[1] * p[3] * p[5] * p[7] != 0) { continue; }
				n = 0;
				for (k = 1; k < 9; k++) if (p[k] == 1) n++;
				if (n < 2) { continue; }
				n = 0;
				for (k = 1; k < 9; k++) if (p[k] == 1) n++;
				if (n < 1) { continue; }
				if (ncon(p) != 1) { continue; }
				n = 0;
				for (k = 1; k < 9; k++) {
					if (p[k] != -1) { n++; }
					else if (p[k] == -1) {
						p[k] = 0;
						if (ncon(p) == 1) { n++; }
						p[k] = -1;
					}
				}
				if (n < 8) { continue; }
				image_out[i][j] = TMP;
				flg++;
			}
		}
		for (i = 1; i < Y_SIZE; i++) {
			for (j = 1; j < X_SIZE; j++) {
				if (image_out[i][j] == TMP) { image_out[i][j] = 0; }
			}
		}
	}
}
int main() {
	Mat image = imread("lenna.jpg" , 0);
	Mat imgout(Y_SIZE, X_SIZE, CV_8UC3, Scalar(0, 0, 0));
	uchar im[Y_SIZE][X_SIZE];
	uchar im_o[Y_SIZE][X_SIZE];
	double	am = 2;
	ConvertToUchar(image, im);
	gradient_difference(im, im_o, am);
	imgout = ConvertToMat(image, im_o);
	imshow("difference", imgout);

	gradient_sobel(im, im_o, am);
	imgout = ConvertToMat(image, im_o);
	imshow("Sobel", imgout);

	gradient_roberts(im, im_o, am);
	imgout = ConvertToMat(image, im_o);
	imshow("Roberts", imgout);

	threshold(im_o, im_o, 90, 1);
	imgout = ConvertToMat(image, im_o);

	thinning(im_o, im_o);
	imgout = ConvertToMat(image, im_o);
	imshow("thinning", imgout);
	waitKey(0);
}
