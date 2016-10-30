#include "ImageProcess.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>

void PointScan(Picture photo,char *NumTable){
	
	/*
		1.清空数组用memset
	*/
	memset(NumTable,0,photo.rowSize*photo.columnSize*sizeof(char));   // 清空二维数组
	
	/*
	    2.实现八连通分析，返回包括表示连通域编号的图片
	*/
	
	/*
	    3.对包括表示连通域编号的图片的各连通域求质心，返回组织为结构体数组,
		该长度按八连通分析的最大编号值作为结构体数组长度，动态使用malloc函数
		#后续记得用free回收
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
		将星体的位置按照公式转换为矢量信息
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