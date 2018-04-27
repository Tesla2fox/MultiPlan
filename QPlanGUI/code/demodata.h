#ifndef DEMODATA_H
#define DEMODATA_H

#include "Point.h"
#include "PointArray.h"
#include "bgeometry.h"
#include "Draw.h"

class DemoData
{
public:
    DemoData();
    geo::Point Target[4];
    geo::Point currentLocation[4];
    geo::Point humanWayPoint[4][10];
    geo::Point humanAvdPoint[4][10];

    PointGPS TargetGps[4];
    PointGPS currentLocationGps[4];
    PointGPS humanWayPointGps[4][10];
    PointGPS humanAvdPointGps[4][10];

    //    std::vector<bgeo::DPoint>  Path;

    bgeo::DLineString Path;

    QDraw  Maindraw;
    void drawPath();
    void drawPath(int const & i);

    int  waySize[4] = {0,0,0,0};
    int  avdSize[4] = {0,0,0,0};




    bool saveData(const char *configflie);
    bool loadData(const char *configflie);
};

#endif // DEMODATA_H
