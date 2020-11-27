#include <iostream>
#include <opencv.hpp>

using namespace cv;
using namespace std;

void Histogram(Mat src, float *hist)
{
	int cellSize = 16;
	int nX = src.cols / cellSize;  //��
	int nY = src.rows / cellSize; //��

								  //�����ݶȼ��Ƕ�
	Mat gx, gy;
	Mat mag, angle;


	Sobel(src, gx, CV_32F, 1, 0, 1);
	Sobel(src, gy, CV_32F, 0, 1, 1);

	//x�����ݶȣ�y�����ݶȣ��ݶȣ��Ƕȣ���������ǶȻ��ǻ���
	//Ĭ���ǻ���radians,����ѡ��Ƕ�degrees
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

			for (int i = pixelStartX; i < pixelEndX; i++)   //XΪ��
			{
				for (int j = pixelStartY; j < pixelEndY; j++)	//YΪ��
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
	int nX = srcMat.cols / cellSize;  //��
	int nY = srcMat.rows / cellSize; //��

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
		cout << "�ڶ���ͼ����" << endl;
	}
	else
	{
		cout << "��һ��ͼ����" << endl;
	}
	delete[] src_hist;
	delete[] img1_hist;
	delete[] img2_hist;

	return 0;
}