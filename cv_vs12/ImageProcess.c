#include "ImageProcess.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>

void PointScan(Picture photo,char *NumTable){
	
	/*
		1.���������memset
	*/
	memset(NumTable,0,photo.rowSize*photo.columnSize*sizeof(char));   // ��ն�ά����
	
	/*
	    2.ʵ�ְ���ͨ���������ذ�����ʾ��ͨ���ŵ�ͼƬ
	*/
	
	/*
	    3.�԰�����ʾ��ͨ���ŵ�ͼƬ�ĸ���ͨ�������ģ�������֯Ϊ�ṹ������,
		�ó��Ȱ�����ͨ�����������ֵ��Ϊ�ṹ�����鳤�ȣ���̬ʹ��malloc����
		#�����ǵ���free����
	*/
}


StarVector* DetermineVector(Coordinate * StarsCoord,int Number){

	int Count;
	float Norm;
	StarVector * nowVector;
	StarVector * AllocVector;
	Coordinate * nowCoord;

	AllocVector = (StarVector*)malloc(sizeof(StarVector)*Number);
	if (AllocVector == NULL)
		return NULL;

	nowVector = AllocVector;
	nowCoord = StarsCoord;

	/*
		�������λ�ð��չ�ʽת��Ϊʸ����Ϣ
	*/
	for(Count = 0; Count<Number; Count++)
	{
		nowVector->v[0] = nowCoord[Count].row;
		nowVector->v[1] = nowCoord[Count].column;
		nowVector->v[2] = (FocusLenth/PixelDistance);
		Norm = sqrt(pow(nowVector->v[0],2)+pow(nowVector->v[1],2)+pow(nowVector->v[2],2));
		nowVector->v[0] = nowVector->v[0]/Norm;
		nowVector->v[1] = nowVector->v[1]/Norm;
		nowVector->v[2] = nowVector->v[2]/Norm;
		nowVector ++;
		nowCoord ++;
	}


}