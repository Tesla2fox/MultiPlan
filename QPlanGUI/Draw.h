#pragma once

#include "qcustomplot.h"
#include "bgeometry.h"
#include <vector>
using namespace bgeo;
class QDraw
{
public:
	QDraw();
	~QDraw();
	QCustomPlot *qplot = nullptr;
	bool  Ring(bgeo::DRing const &ring);
	bool  test(DRing ttt);
	bool  LineString(bgeo::DLineString const & lineString);

	bool  LineString(bgeo::DLineString const & lineString , int const &id);

	bool  slowLine(bgeo::DLineString const & lineString);

	bool  Polygon(bgeo::DPolygon const & polygon);
	bool  vPoint(std::vector<bgeo::DPoint> const & vpoint);
	bool  Ring(bgeo::DRing const &ring, const QColor &color);
	//»­Ïß¶Î
	bool  segment(bgeo::DSegment const &seg);

private:
	bool m_BInitalization = false;
	bool Initalization();

};


