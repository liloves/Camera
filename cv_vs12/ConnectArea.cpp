

#include <iostream> 
#include <opencv2/core/core.hpp>
#include <opencv/cv.h>

using namespace cv;
using namespace std;

//Just some convienience macros 
#define CV_CVX_WHITE CV_RGB(0xff,0xff,0xff) 
#define CV_CVX_BLACK CV_RGB(0x00,0x00,0x00) 


void ConnectedComponents(Mat &mask_process, int poly1_hull0, float perimScale, int number = 0,
	Rect &bounding_box = Rect(), Point &contour_centers = Point(-1, -1))
{
	/*����4�������Ϊ�˼���ԭ�����ӿڣ����ڲ�ʹ�õ���c��񣬵�����ӿ���c++����*/
	IplImage *mask = &mask_process.operator IplImage();
	int *num = &number;
	CvRect *bbs = &bounding_box.operator CvRect();
	CvPoint *centers = &contour_centers.operator CvPoint();
	static CvMemStorage* mem_storage = NULL;
	static CvSeq* contours = NULL;
	//CLEAN UP RAW MASK 
	//���������ã�ƽ��������ȥ��ϸ��,�Ͽ�ȱ�� 
	cvMorphologyEx(mask, mask, NULL, NULL, CV_MOP_OPEN, 1);//������mask���п�������CVCLOSE_ITRΪ�������Ĵ��������Ϊmaskͼ�� 
	//���������ã�ƽ������������ȱ�� 
	cvMorphologyEx(mask, mask, NULL, NULL, CV_MOP_CLOSE, 1);//������mask���бղ�����CVCLOSE_ITRΪ�ղ����Ĵ��������Ϊmaskͼ�� 
	//FIND CONTOURS AROUND ONLY BIGGER REGIONS 
	if (mem_storage == NULL) mem_storage = cvCreateMemStorage(0);
	else cvClearMemStorage(mem_storage);
	//CV_RETR_EXTERNAL=0����types_c.h�ж���ģ�CV_CHAIN_APPROX_SIMPLE=2Ҳ���ڸ��ļ��ж���� 
	CvContourScanner scanner = cvStartFindContours(mask, mem_storage, sizeof(CvContour), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	CvSeq* c;
	int numCont = 0;
	//��while�ڲ�ֻ��ԱȽϴ���������߽����滻���� 
	while ((c = cvFindNextContour(scanner)) != NULL)
	{
		double len = cvContourPerimeter(c);
		double q = (mask->height + mask->width) / perimScale; //calculate perimeter len threshold 
		if (len < q) //Get rid of blob if it's perimeter is too small 
		{
			cvSubstituteContour(scanner, NULL); //��NULL����ԭ�����Ǹ����� 
		}
		else //Smooth it's edges if it's large enough 
		{
			CvSeq* c_new;
			if (poly1_hull0) //Polygonal approximation of the segmentation 
				c_new = cvApproxPoly(c, sizeof(CvContour), mem_storage, CV_POLY_APPROX_DP, 2, 0);
			else //Convex Hull of the segmentation 
				c_new = cvConvexHull2(c, mem_storage, CV_CLOCKWISE, 1);
			cvSubstituteContour(scanner, c_new); //�ʼ��������͹�����߶���ʽ��������滻 
			numCont++;
		}
	}
	contours = cvEndFindContours(&scanner); //�����������Ҳ��� 
	// PAINT THE FOUND REGIONS BACK INTO THE IMAGE 
	cvZero(mask);
	IplImage *maskTemp;
	//CALC CENTER OF MASS AND OR BOUNDING RECTANGLES 
	if (*num != 0)
	{
		int N = *num, numFilled = 0, i = 0;
		CvMoments moments;
		double M00, M01, M10;
		maskTemp = cvCloneImage(mask);
		for (i = 0, c = contours; c != NULL; c = c->h_next, i++) //h_nextΪ���������е���һ������ 
		{
			if (i < N) //Only process up to *num of them 
			{
				//CV_CVX_WHITE�ڱ��������ǰ�ɫ����˼ 
				cvDrawContours(maskTemp, c, CV_CVX_WHITE, CV_CVX_WHITE, -1, CV_FILLED, 8);
				//Find the center of each contour 
				if (centers != &cvPoint(-1, -1))
				{
					cvMoments(maskTemp, &moments, 1); //����maskͼ�����ߴ�3�׵ľ� 
					M00 = cvGetSpatialMoment(&moments, 0, 0); //��ȡx��0�κ�y��0�ξ� 
					M10 = cvGetSpatialMoment(&moments, 1, 0); //��ȡx��1�κ�y��0�ξ� 
					M01 = cvGetSpatialMoment(&moments, 0, 1); //��ȡx��0�κ�y��1�ξ� 
					centers[i].x = (int)(M10 / M00); //���þصĽ��������������ĵ����� 
					centers[i].y = (int)(M01 / M00);
				}
				//Bounding rectangles around blobs 
				if (bbs != &CvRect())
				{
					bbs[i] = cvBoundingRect(c); //�������c����Ӿ��� 
				}
				cvZero(maskTemp);
				numFilled++;
			}
			//Draw filled contours into mask 
			cvDrawContours(mask, c, CV_CVX_WHITE, CV_CVX_WHITE, -1, CV_FILLED, 8); //draw to central mask 
		} //end looping over contours 
		*num = numFilled;
		cvReleaseImage(&maskTemp);
	}
	//ELSE JUST DRAW PROCESSED CONTOURS INTO THE MASK 
	else
	{
		for (c = contours; c != NULL; c = c->h_next)
		{
			cvDrawContours(mask, c, CV_CVX_WHITE, CV_CVX_BLACK, -1, CV_FILLED, 8);
		}
	}
}

/*
int main()
{
	Mat src, mask;
	src = imread("test.png", 0); //�ԻҶ�ͼ����� 
	imshow("src", src);

	mask = src > 0; //ת��Ϊ��ֵͼ�� 
	imshow("mask", mask);

	ConnectedComponents(mask, 1, 8.0, 1, Rect(), Point(-1, -1)); //���ö������ϴ��� 
	imshow("out1", mask);

	ConnectedComponents(mask, 0, 8.0, 1, Rect(), Point(-1, -1)); //c����͹�����д��� 
	imshow("out2", mask);
	waitKey(0);

	return 0;
}*/