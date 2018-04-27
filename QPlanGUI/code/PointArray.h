#pragma once

#ifdef __cplusplus
extern "C"{
#endif

typedef struct{
	double lat, lon;
}PointGPS;

typedef struct{
	unsigned int   size;
	PointGPS *pts;
}PointArrary;

typedef struct {
	unsigned int   size;
	PointGPS *pts;
	double   *vel;
}PointArraryEx;

typedef struct{
    unsigned int   size;
    PointGPS *pts;
    double   *vel;
    long     *ID;
}PointArraryExID;

typedef struct{
	unsigned int   size;
    PointGPS *pts;
    double   *vel;
    long     *ID;
	
}PointArraryExTh;

#ifdef __cplusplus
}
#endif
