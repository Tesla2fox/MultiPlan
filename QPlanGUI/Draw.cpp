#include "Draw.h"
//
//
//
QDraw::QDraw()
{
}

QDraw::~QDraw()
{
}

bool QDraw::Initalization()
{
	if (qplot == nullptr) {
		this->m_BInitalization = false;
		return false;
	}
	this->m_BInitalization = true;
	return true;
}

bool QDraw::Ring(bgeo::DRing const &ring)
{
	this->Initalization();
	if (this->m_BInitalization)
	{

		QCPCurve *RingCurve = new QCPCurve(qplot->xAxis, qplot->yAxis);
		////QVector
		QVector<QCPCurveData> RingCurveData;



		for (size_t i = 0; i < ring.size(); i++)
		{
			//
			RingCurveData.push_back(QCPCurveData(i, ring.at(i).x(), ring.at(i).y()));

		}
		RingCurve->data()->set(RingCurveData, false);

		qplot->xAxis->setScaleRatio(qplot->yAxis);
		//RingCurve->setScatterStyle(QCPScatterStyle((QCPScatterStyle::ScatterShape)(1)));

		QPen pen;
		pen.setColor(Qt::GlobalColor::magenta);
		//pen.setColor
		pen.setWidth(1);
		RingCurve->setPen(pen);
		qplot->replot();
		return true;
	}
	else
	{
		return false;
	}
}



bool QDraw::Ring(bgeo::DRing const &ring, const QColor &color)
{
	this->Initalization();
	if (this->m_BInitalization)
	{

		QCPCurve *RingCurve = new QCPCurve(qplot->xAxis, qplot->yAxis);
		////QVector
		QVector<QCPCurveData> RingCurveData;



		for (size_t i = 0; i < ring.size(); i++)
		{
			//
			RingCurveData.push_back(QCPCurveData(i, ring.at(i).x(), ring.at(i).y()));

		}
		RingCurve->data()->set(RingCurveData, false);

		qplot->xAxis->setScaleRatio(qplot->yAxis);
		QPen pen;
		pen.setColor(color);
		//pen.setColor
		pen.setWidth(1);
		RingCurve->setPen(pen);
		qplot->replot();
		return true;
	}
	else
	{
		return false;
	}
}


bool QDraw::LineString(bgeo::DLineString const & lineString)
{

	this->Initalization();
	if (this->m_BInitalization)
	{

		QCPCurve *lineStringCurve = new QCPCurve(qplot->xAxis, qplot->yAxis);
		////QVector
		QVector<QCPCurveData> lineStringCurveData;
		for (size_t i = 0; i < lineString.size(); i++)
		{
			lineStringCurveData.push_back(QCPCurveData(i, lineString.at(i).x(), lineString.at(i).y()));

            QCPItemText *txt = new QCPItemText(this->qplot);
            txt->position->setCoords(lineString.at(i).x() + 1, lineString.at(i).y() + 1);
            QString QSt_index = QString("p-%1").arg(i);
            txt->setText(QSt_index);

		}
		lineStringCurve->data()->set(lineStringCurveData, false);

		qplot->xAxis->setScaleRatio(qplot->yAxis);
		lineStringCurve->setScatterStyle(QCPScatterStyle((QCPScatterStyle::ScatterShape)(5)));

		QPen pen;
        pen.setColor(Qt::GlobalColor::red);
        pen.setWidth(4);
		lineStringCurve->setPen(pen);

		//lineStringCurve->setScatterStyle(QCPScatterStyle((QCPScatterStyle::ScatterShape)(5)));
		qplot->replot();
		return true;
	}
	else
	{
		return false;
    }
}

bool QDraw::LineString(const DLineString &lineString, const int &id)
{
    this->Initalization();
    if (this->m_BInitalization)
    {

        QCPCurve *lineStringCurve = new QCPCurve(qplot->xAxis, qplot->yAxis);
        ////QVector
        QVector<QCPCurveData> lineStringCurveData;
        for (size_t i = 0; i < lineString.size(); i++)
        {
            lineStringCurveData.push_back(QCPCurveData(i, lineString.at(i).x(), lineString.at(i).y()));
        }
        QCPItemText *txt = new QCPItemText(this->qplot);
        txt->position->setCoords(lineString.at(0).x() + 1, lineString.at(0).y() + 1);
        QString QSt_index = QString("Robot-%1").arg(id);
        txt->setText(QSt_index);


        QCPItemText *Ttxt = new QCPItemText(this->qplot);
        Ttxt->position->setCoords(lineString.back().x() + 1, lineString.back().y() + 1);
        QString TQSt_index = QString("Target-%1").arg(id);
        Ttxt->setText(TQSt_index);


        lineStringCurve->data()->set(lineStringCurveData, false);

        qplot->xAxis->setScaleRatio(qplot->yAxis);
        lineStringCurve->setScatterStyle(QCPScatterStyle((QCPScatterStyle::ScatterShape)(5)));

        QPen pen;
        switch (id) {
        case 0:
            pen.setColor(Qt::GlobalColor::darkCyan);
            break;
        case 1:
            pen.setColor(Qt::GlobalColor::darkYellow);
            break;
        case 2:
            pen.setColor(Qt::GlobalColor::blue);
            break;
        case 3:
            pen.setColor(Qt::GlobalColor::black);
            break;
        default:
            break;
        }
//        pen.setColor(Qt::GlobalColor::red);
        pen.setWidth(3);
        lineStringCurve->setPen(pen);


        //lineStringCurve->setScatterStyle(QCPScatterStyle((QCPScatterStyle::ScatterShape)(5)));
        qplot->replot();
        return true;
    }
    else
    {
        return false;
    }
}

bool QDraw::slowLine(bgeo::DLineString const & lineString)
{

	this->Initalization();
	if (this->m_BInitalization)
	{

		QCPCurve *lineStringCurve = new QCPCurve(qplot->xAxis, qplot->yAxis);
		QVector<QCPCurveData> lineStringCurveData;
		for (size_t j = 0; j < lineString.size(); j++)
		{
			for (size_t i = 0; i < j; i++)
			{
				lineStringCurveData.push_back(QCPCurveData(i, lineString.at(i).x(), lineString.at(i).y()));
			}
			lineStringCurve->data()->set(lineStringCurveData, false);

			qplot->xAxis->setScaleRatio(qplot->yAxis);
			lineStringCurve->setScatterStyle(QCPScatterStyle((QCPScatterStyle::ScatterShape)(5)));

			QPen pen;
            pen.setColor(Qt::GlobalColor::red);
            pen.setWidth(4);
			lineStringCurve->setPen(pen);
            //unix;
            usleep(100000);
            //Sleep(100);
			//lineStringCurve->setScatterStyle(QCPScatterStyle((QCPScatterStyle::ScatterShape)(5)));
			qplot->replot();
		}
		return true;
	}
	else
	{
		return false;
	}
}



bool QDraw::segment(bgeo::DSegment const &seg)
{
	this->Initalization();
	if (this->m_BInitalization)
	{

		QCPCurve *segCurve = new QCPCurve(qplot->xAxis, qplot->yAxis);
		////QVector
		QVector<QCPCurveData> segCurveData;

		//
		segCurveData.push_back(QCPCurveData(0, seg.first.x(), seg.first.y()));
		segCurveData.push_back(QCPCurveData(1, seg.second.x(), seg.second.y()));


		segCurve->data()->set(segCurveData, false);

		qplot->xAxis->setScaleRatio(qplot->yAxis);
		//segCurve->setScatterStyle(QCPScatterStyle((QCPScatterStyle::ScatterShape)(5)));

		QPen pen;
		pen.setColor(Qt::GlobalColor::black);
		//pen.setColor
		pen.setWidth(1);
		segCurve->setPen(pen);
		qplot->replot();
		//Sleep(200);
		return true;
	}
	else
	{
		return false;
	}
}
////bool QDraw::Polygon(bgeo::DPolygon const & polygon) {
////	this->Initalization();
////	if (this->m_BInitalization)
////	{
////
////		QCPCurve *polygonCurve = new QCPCurve(qplot->xAxis, qplot->yAxis);
////		////QVector
////		QVector<QCPCurveData> polygonCurveData;
////		
////		for (size_t i = 0; i < polygon.outer.size(); i++)
////		{
////			polygonCurveData.push_back(QCPCurveData(i, polygon.outer.at(i).x, polygon.outer.at(i).y));
////		}
////		polygonCurve->data()->set(polygonCurveData, false);
////
////		qplot->xAxis->setScaleRatio(qplot->yAxis);
////		//polygonCurve->setScatterStyle(QCPScatterStyle((QCPScatterStyle::ScatterShape)(5)));
////
////		QPen pen;
////		pen.setColor(Qt::GlobalColor::green);
////		pen.setWidth(2);
////		polygonCurve->setPen(pen);
////
////		polygonCurve->setScatterStyle(QCPScatterStyle((QCPScatterStyle::ScatterShape)(5)));
////		qplot->replot();
////		return true;
////	}
////	else
////	{
////		return false;
////	}
////}
//
//bool  QDraw::vPoint(std::vector<bgeo::DPoint> const & vpoint)
//{
//
//	this->Initalization();
//	if (this->m_BInitalization)
//	{
//
//		QCPCurve *vpointCurve = new QCPCurve(qplot->xAxis, qplot->yAxis);
//		////QVector
//		QVector<QCPCurveData> vpointCurveData;
//		for (size_t i = 0; i < vpoint.size(); i++)
//		{
//			vpointCurveData.push_back(QCPCurveData(i, &vpoint.at(i).x, &vpoint.at(i).y));
//		}
//		vpointCurve->data()->set(vpointCurveData, false);
//
//		qplot->xAxis->setScaleRatio(qplot->yAxis);
//		vpointCurve->setScatterStyle(QCPScatterStyle((QCPScatterStyle::ScatterShape)(3)));
//
//		QPen pen;
//		pen.setColor(Qt::GlobalColor::black);
//		pen.setWidth(2);
//		vpointCurve->setPen(pen);
//
//		//vpointCurve->setScatterStyle(QCPScatterStyle((QCPScatterStyle::ScatterShape)(5)));
//		qplot->replot();
//		return true;
//	}
//	else
//	{
//		return false;
//	}
//
//
//}
