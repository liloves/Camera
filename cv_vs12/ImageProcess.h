#ifndef _ImageProcess_h
#define _ImageProcess_h


#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#define PictureRow 1024u
#define PictureColumn 768u
#define FocusLenth 35e-3           // 焦距，单位米
#define PixelDistance 6.7e-6       // 像元间距，单位米
#define FocusEqual (FocusLenth/PixelDistance)


typedef struct TheStar{
	float v[3];
}StarVector;

typedef struct TheCoord{
	float row;
	float column;
}Coordinate;

typedef struct{
	char *data;
	int rowSize;
	int columnSize;
}Picture;

void load(Picture, char *);
void PointScan(Picture,char *);
StarVector* DetermineVector(Coordinate * ,int );

#ifdef __cplusplus
}
#endif

#endif