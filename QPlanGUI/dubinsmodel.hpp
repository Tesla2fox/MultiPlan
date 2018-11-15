#ifndef DUBINSMODEL_HPP
#define DUBINSMODEL_HPP

#include <vector>
#include "dubins.h"
#include "bgeometry.h"
#include <math.h>
struct MyDubinsSturct
{
    double x[10000];
    double y[10000];
    double w[10000];
    size_t size = 0;
};

inline int myDubinsTest(double *q, double nothing, void *user_data)
{
    auto wtf = reinterpret_cast<MyDubinsSturct*>(user_data);
    wtf->size += 1;
    wtf->x[wtf->size - 1] = q[0];
    wtf->y[wtf->size - 1] = q[1];
    wtf->w[wtf->size - 1] = q[2];
    return 0;
}

inline std::vector<double> getDubinsPath(double * q0, double * q1)
{
    DubinsPath d_path;
    int err = dubins_init(q0,q1,0.1,&d_path);
    MyDubinsSturct data;
    dubins_path_sample_many(&d_path, myDubinsTest, 0.2, &data);
    std::vector<double> res;

    for (size_t i = 0; i< data.size; i ++)
    {
        res.push_back(data.x[i]);
        res.push_back(data.y[i]);
        res.push_back(data.w[i]);
    }
    return res;
}


inline bgeo::DLineString getDubinsPath(bgeo::DLineString const & path)
{
    bgeo::DLineString res;
    std::vector<double> v_yaw;
    v_yaw.push_back(0);
    for (size_t i = 0; i < path .size() - 1; i++ )
    {
        double yaw = 0;
        if (path.at(i + 1).x() == path.at(i).x())
            if (path.at(i + 1).y() > path.at(i).y())
                yaw = M_PI_2;
            if (path.at(i + 1).y() > path.at(i).y())
                yaw = -M_PI_2;
        else if (path.at(i + 1).x() > path.at(i).x())
            {double  tanVal = (path.at(i + 1).y() - path.at(i).y())/(path.at(i + 1).x() - path.at(i).x());
                yaw = atan(tanVal);
            }
             else
             {   double  tanVal = (path.at(i + 1).y() - path.at(i).y())/(path.at(i + 1).x() - path.at(i).x());
                yaw = atan(tanVal);
            }
        v_yaw.push_back(yaw);
    }
    for (size_t i = 0; i < path.size() - 1 ; i++)
    {
        double q0[3];
        double q1[3];
        q0[0]  = path.at(i).x();
        q0[1]  = path.at(i).y();
        q0[2]  = v_yaw.at(i);

        q1[0]  = path.at(i + 1).x();
        q1[1]  = path.at(i + 1).y();
        q1[2]  = v_yaw.at(i + 1);

        auto r_path = getDubinsPath(q0,q1);

        for (size_t j = 0; j<r_path.size() /3 ;j++)
        {
            res.push_back(bgeo::DPoint(r_path.at(3*j),r_path.at(3*j+ 1)));
        }
    }
        return res;
}


#endif // DUBINSMODEL_HPP
