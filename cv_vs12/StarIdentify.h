#ifndef _StarIdentify_h
#define _StarIdentify_h

#include "ImageProcess.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

extern struct Coordinate;
extern struct StarVector;


void StarIdentify(StarVector *,int Count);

#ifdef __cplusplus
}
#endif


#endif