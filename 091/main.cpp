#include <iostream>
#include <opencv.hpp>

using namespace cv;
using namespace std;

void Histogram(Mat src, float *hist)
{
	int cellSize = 16;
	int nX = src.cols / cellSize;  //列
	int nY = src.rows / cellSize; //行

								  //计算梯度及角度
	Mat gx, gy;
	Mat mag, angle;


	Sobel(src, gx, CV_32F, 1, 0, 1);
	Sobel(src, gy, CV_32F, 0, 1, 1);

	//x方向梯度，y方向梯度，梯度，角度，决定输出角度还是弧度
	//默认是弧度radians,可以选择角度degrees
	cartToPolar(gx, gy, mag, angle, true);

	for (int cellX = 0; cellX < nX; cellX++)
	{
		for (int cellY = 0; cellY < nY; cellY++)
		{
			int pixelStartX = 16 * cellX;
			int pixelEndX = pixelStartX + 16;
			int pixelStartY = 16 * cellY;
			int pixelEndY = pixelStartY + 16;

			int cellN = cellX * nX + cellY;

			for (int i = pixelStartX; i < pixelEndX; i++)   //X为行
			{
				for (int j = pixelStartY; j < pixelEndY; j++)	//Y为列
				{
					int angle_average = angle.at<float>(j, i);
					hist[(angle_average / 45 + cellN * 8)] += mag.at<float>(j, i);
				}
			}
		}
	}

}

float distanceCal(float *src_hist, float *dst_hist, int bin)
{
	float sum = 0;
	for (int i = 0; i < bin; i++)
	{
		sum += (src_hist[i] - dst_hist[i])*(src_hist[i] - dst_hist[i]);
	}
	sum = sqrt(sum);
	return sum;
}

int main()
{
	Mat srcMat = imread("C:\\Users\\HS\\Desktop\\hogTemplate.jpg");
	Mat	imgMat1 = imread("C:\\Users\\HS\\Desktop\\img1.jpg");
	Mat	imgMat2 = imread("C:\\Users\\HS\\Desktop\\img2.jpg");

	int cellSize = 16;
	int nX = srcMat.cols / cellSize;  //列
	int nY = srcMat.rows / cellSize; //行

	int bin = nX * nY * 8;
	float *src_hist = new float[bin];
	memset(src_hist, 0, sizeof(float)*bin);

	float *img1_hist = new float[bin];
	memset(img1_hist, 0, sizeof(float)*bin);

	float *img2_hist = new float[bin];
	memset(img2_hist, 0, sizeof(float)*bin);

	Histogram(srcMat, src_hist);
	Histogram(imgMat1, img1_hist);
	Histogram(imgMat2, img2_hist);

	float img1dist = distanceCal(src_hist, img1_hist, bin);
	float img2dist = distanceCal(src_hist, img2_hist, bin);


	cout << img1dist << endl;
	std::cout << img2dist << endl;

	if (img1dist > img2dist)
	{
		cout << "第二张图更像" << endl;
	}
	else
	{
		cout << "第一张图更像" << endl;
	}
	delete[] src_hist;
	delete[] img1_hist;
	delete[] img2_hist;

	return 0;
}