#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cv.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>
#include <iostream>
#include <math.h>
using namespace cv;
using namespace std;


void bwLabel(const Mat& imgBw, Mat & imgLabeled);
int main(int argc, char** argv)
{
	/*
	if (argc != 2)
	{
		cout << " Usage: display_image ImageToLoadAndDisplay" << endl;
		return -1;
	}
	*/
	cout << "input the photo file name（maybe 1.jpg or 2.jpg）: ";
	string FileName;
	cin >> FileName;

	Mat FilePic;
	FilePic = imread(FileName.c_str(), IMREAD_COLOR);
	if (!FilePic.data)
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	
	Mat gray_image;
	cvtColor(FilePic, gray_image, CV_BGR2GRAY);
	namedWindow("Gray window", WINDOW_AUTOSIZE); // Create a window for display.
	imshow("Gray window", gray_image); // Show our image inside it.

	Mat FiltPic;
	GaussianBlur(gray_image, FiltPic, Size(3, 3), 0, 0);	
	namedWindow("Filter window", WINDOW_AUTOSIZE); // Create a window for display.
	imshow("Filter window", FiltPic); // Show our image inside it.

	Mat mask = FiltPic > 255 * 0.5; //转换为二值图像 
	namedWindow("mask", WINDOW_AUTOSIZE); // Create a window for display.
	imshow("mask", mask);
	
	Mat label = mask;
	bwLabel(mask, label);
	namedWindow("Label", WINDOW_AUTOSIZE); // Create a window for display.
	imshow("Label", label);

	
	waitKey(0);
	return 0;

}