#include "mainwindow.h"
#include "ui_mainwindow.h"


#include "gps2local.h"
#include "qdebug.h"
#include "bgeometry.h"

#include "readmap.hpp"
#include "ssconfig.hpp"
#include "polygonob.h"

#include "code/pathPlanning.h"
#include "dubinsmodel.hpp"

#include <vector>
#include <map>
#include "time.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(QString("the version  %1_%2").arg(PROJECT_VERSION_MAJOR).arg(PROJECT_VERSION_MINOR));
    ui->QGraph->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom);
    connect(ui->QGraph, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress(QMouseEvent*)));
    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(boxChange(int)));
    connect(ui->comboBox_2,SIGNAL(currentIndexChanged(int)),this,SLOT(idboxChange(int)));
    connect(ui->comboBox_3,SIGNAL(currentIndexChanged(int)),this,SLOT(modeChange(int)));

    GPSSetAnchor(39.96300, 116.30440, 0);

    std::map<int, std::string> MapRead;
    MapRead.insert(std::pair<int, std::string>(0, "obWood"));
    MapRead.insert(std::pair<int, std::string>(1, "obHuge"));
    MapRead.insert(std::pair<int, std::string>(2, "obLittle"));
    MapRead.insert(std::pair<int, std::string>(3, "obYang"));
    MapRead.insert(std::pair<int, std::string>(4, "obPigHole"));

    double x,y;
    GPS2Local(39.96257281,116.3038457,0,&x,&y,NULL);
    qDebug()<<" x= "<<x <<" y  = "<<y;

    GPS2Local(39.96248664,116.3041191,0,&x,&y,NULL);
    qDebug()<<" x= "<<x <<" y  = "<<y;


    std::string fname = "/home/robot/qt_code/multiPlan/data/data-Lnew.txt";
    for (size_t i = 0; i < MapRead.size(); i++)
    {
        auto st_name = MapRead[i];
        std::vector<GPSPolygon> vobGps;
        read_map(fname, st_name, vobGps);
        std::vector<double> vlat, vlon;
        for (auto &it : vobGps)
        {
            std::vector<double> vdx, vdy;
            vdx.resize(it.vlat.size());
            vdy.resize(it.vlon.size());
            for (size_t j = 0; j < it.vlat.size(); j++)
            {
                GPS2Local(it.vlat.at(j), it.vlon.at(j), 0, &vdx.at(j), &vdy.at(j), nullptr);
            }
            ob::PolygonOb UnitOb(it.vlat, it.vlon, ob::GPSPolygonOb, i);
            vdx.push_back(vdx.front());
            vdy.push_back(vdy.front());
            bgeo::DRing ring = bgeo::v2Ring(vdx, vdy);
            MainMap.addOb(UnitOb);
            MainMap.addObRing(ring);
        }
    }

    //std::string shFname = "/home/robot/qt_code/multiPlan/data/LineOb.txt";
    //sscfg::ConfigFile cfg_ob = sscfg::ConfigFile::load(shFname);
    std::vector<double> vpx, vpy, vqx, vqy;

    // cfg_ob.get("obLine_LinePx", vpx);
    // cfg_ob.get("obLine_LinePy", vpy);
    //cfg_ob.get("obLine_LineQx", vqx);
    // cfg_ob.get("obLine_LineQy", vqy);

    this->_inLine.load("/home/robot/qt_code/multiPlan/data/inLine.txt");
    this->_OutLine.load("/home/robot/qt_code/multiPlan/data/outLine.txt");
    for(auto &it : _inLine.obLine)
    {
        vpx.push_back(it.p.x);
        vpy.push_back(it.p.y);
        vqx.push_back(it.q.x);
        vqy.push_back(it.q.y);
    }

    MainMap.addShoulder(vpx, vpy, vqx, vqy);
    vpx.clear();
    vpy.clear();
    vqx.clear();
    vqy.clear();

    for(auto &it : _OutLine.obLine)
    {
        vpx.push_back(it.p.x);
        vpy.push_back(it.p.y);
        vqx.push_back(it.q.x);
        vqy.push_back(it.q.y);
    }
    MainMap.addRange(vpx,vpy,vqx,vqy);

    MainMap.qplot = ui->QGraph;
    m_aplan.qplot = ui->QGraph;
    m_splan.qplot = ui->QGraph;
    MainMap.drawObRing();
    MainMap.drawShoulder();
    MainMap.drawRange();

    if(false)
    {
        MainMap.Map2grid();
        MainMap.Map2Agrid();
        MainMap.writeMapGrid();
    }
    else
    {
        MainMap.loadMapGrid("/home/robot/qt_code/multiPlan/data/OBCG.xml");
        auto wtfMap = this->MainMap;
        qDebug()<<"wtf";
    }
    if (false)
    {
        if (false)
        {
            MainMap.drawgrid(ob::MapType::AggregationMap);
        }
        else
        {
            if(true)
            {
                MainMap.drawgrid(ob::MapType::GroupMap);
            }
            else{
                MainMap.drawgrid(ob::MapType::SearchMap);
                MainMap.drawEdge();
            }
        }
    }
    //MainMap.drawEdge();

    this->planEvent = QPlanEvent::noplan;

    m_aplan.loadMap(this->MainMap);
    m_splan.loadMap(this->MainMap);
    qDebug("success Mainmap input ");

    //QcomboBox *

    this->staItemNum = ui->QGraph->itemCount();
    this->staPloNum = ui->QGraph->plottableCount();
    this->staGraphNum = ui->QGraph->graphCount();

    qDebug()<<"Item = "<<staItemNum<< "  Plot= "<<staPloNum << "  graph "<< staGraphNum;

    ui->QGraph->xAxis->setRange(-75,75);
    ui->QGraph->yAxis->setRange(-70,10);
    ui->QGraph->xAxis->setScaleRatio(ui->QGraph->yAxis);


    InitPathPlanning("/home/robot/qt_code/multiPlan/data/OBC.xml");

    this->MainData.Maindraw.qplot = ui->QGraph;
    ui->QGraph->replot();

}


void MainWindow::resizeEvent(QResizeEvent *event)
{
    ui->QGraph->xAxis->setScaleRatio(ui->QGraph->yAxis);
}

MainWindow::~MainWindow()
{
    delete ui;
}



//siwtch
void MainWindow::on_pushButton_3_clicked()
{
    if(  this->planEvent == QPlanEvent::Aggregation)
    {
        this->planEvent == QPlanEvent::search;
        qDebug()<<"search";
    }
    else
    {
        this->planEvent == QPlanEvent::Aggregation;
        qDebug()<<"Aggregation";
    }
}


void MainWindow::mousePress(QMouseEvent *event)
{
    ui->QGraph->xAxis->setScaleRatio(ui->QGraph->yAxis);
    if (qApp->mouseButtons() == Qt::LeftButton)
    {
        switch (this->planEvent)
        {
        case QPlanEvent::Scout:
        case QPlanEvent::Aggregation:
        {
            //            int x_pos = event->pos().x();
            //            int y_pos = event->pos().y();
            //            float x_val = ui->QGraph->xAxis->pixelToCoord(x_pos);
            //            float y_val = ui->QGraph->yAxis->pixelToCoord(y_pos);

            int x_pos = event->pos().x();
            int y_pos = event->pos().y();
            float x_val = ui->QGraph->xAxis->pixelToCoord(x_pos);
            float y_val = ui->QGraph->yAxis->pixelToCoord(y_pos);

            m_aplan.getStartPnt(x_val, y_val);

            if(LclickTime>3)
            {
                LclickTime = 3;
            }
            MainData.currentLocation[LclickTime].x = x_val;
            MainData.currentLocation[LclickTime].y = y_val;


            Local2GPS(x_val,y_val,0,
                      &this->MainData.currentLocationGps[LclickTime].lat,&MainData.currentLocationGps[LclickTime].lon,NULL);

            qDebug()<<" currentlocation " << LclickTime;
            LclickTime ++;

            if(false)
            {
                while (this->staPloNum<ui->QGraph->plottableCount())
                {
                    ui->QGraph->removePlottable(ui->QGraph->plottableCount()-1);
                }
                while (this->staItemNum<ui->QGraph->itemCount())
                {
                    ui->QGraph->removeItem(ui->QGraph->itemCount()-1);
                }
            }
            ui->QGraph->replot();
            qDebug() << "startPnt.x  =  " << x_val;
            qDebug() << "startPnt.y  =  " << y_val;
            break;
        }
        case QPlanEvent::search:
        {
            int x_pos = event->pos().x();
            int y_pos = event->pos().y();
            float x_val = ui->QGraph->xAxis->pixelToCoord(x_pos);
            float y_val = ui->QGraph->yAxis->pixelToCoord(y_pos);
            //m_aplan.getStartPnt(x_val, y_val);
            vx.push_back(x_val);
            vy.push_back(y_val);
            if(vx.size()==1)
            {
                while (this->staPloNum<ui->QGraph->plottableCount())
                {
                    ui->QGraph->removePlottable(ui->QGraph->plottableCount()-1);
                }
                while (this->staItemNum<ui->QGraph->itemCount())
                {
                    ui->QGraph->removeItem(ui->QGraph->itemCount()-1);
                }
            }
            qDebug() <<vx.size()<< "SearchPnt.x  =  " << x_val;
            qDebug() <<vx.size()<< "SearchPnt.y  =  " << y_val;

            QCPCurve* ScoutArea = new QCPCurve(ui->QGraph->xAxis, ui->QGraph->yAxis);
            ScoutAreaData.push_back(QCPCurveData(lclickTimes, x_val, y_val));

            ScoutArea->data()->set(ScoutAreaData, false);

            QPen pen;
            pen.setStyle(Qt::SolidLine);
            pen.setWidth(1);
            pen.setColor(Qt::GlobalColor::gray);

            ui->QGraph->replot();
            lclickTimes++;
            break;
        }
        case QPlanEvent::human_avoid:
        {
            int x_pos = event->pos().x();
            int y_pos = event->pos().y();
            float x_val = ui->QGraph->xAxis->pixelToCoord(x_pos);
            float y_val = ui->QGraph->yAxis->pixelToCoord(y_pos);

            //            x_val = 42.5;
            //            y_val = -31;
            //            //this->MainData.avdSize[mID];
            auto  &avdID =  this->MainData.avdSize[mID];
            Local2GPS(x_val,y_val,0,
                      &MainData.humanAvdPointGps[mID][avdID].lat,&MainData.humanAvdPointGps[mID][avdID].lon,NULL);


            QCPItemEllipse *circle = new QCPItemEllipse(ui->QGraph);
            circle->topLeft->setCoords(x_val - 2, y_val +2);
            circle->bottomRight->setCoords(x_val +2 , y_val -2);

            QCPItemText *txt = new QCPItemText(ui->QGraph);
            txt->position->setCoords(x_val, y_val);
            QString QSt_index = QString("avd%1-%2").arg(this->mID).arg(++avdID);
            txt->setText(QSt_index);
            this->m_aplan.setOb(x_val,y_val);
            ui->QGraph->replot();
            break;
        }
        case QPlanEvent::virtualTarget:
        {

            int x_pos = event->pos().x();
            int y_pos = event->pos().y();
            float x_val = ui->QGraph->xAxis->pixelToCoord(x_pos);
            float y_val = ui->QGraph->yAxis->pixelToCoord(y_pos);

            v_virtualTarget.push_back(bgeo::DPoint(x_val,y_val));
            QCPItemEllipse *circle = new QCPItemEllipse(ui->QGraph);
            circle->topLeft->setCoords(x_val - 2, y_val +2);
            circle->bottomRight->setCoords(x_val +2 , y_val -2);

            QCPItemText *txt = new QCPItemText(ui->QGraph);
            txt->position->setCoords(x_val, y_val);
            QString QSt_index = QString("VirtualTarget");
            txt->setText(QSt_index);
            //this->m_aplan.setOb(x_val,y_val);
            ui->QGraph->replot();
            break;
        }
        default:
            break;
        }
    }
    if (qApp->mouseButtons() == Qt::RightButton)
    {
        switch (this->planEvent)
        {
        case QPlanEvent::Scout:
        case QPlanEvent::Aggregation:
        {
            //            qDebug() << "TargetPnt.x  =  " << x_val;
            //            qDebug() << "TargetPnt.y  =  " << y_val;
            int x_pos = event->pos().x();
            int y_pos = event->pos().y();
            float x_val = ui->QGraph->xAxis->pixelToCoord(x_pos);
            float y_val = ui->QGraph->yAxis->pixelToCoord(y_pos);
            m_aplan.getTargetPnt(x_val, y_val);
            if(RclickTime>3)
            {
                RclickTime = 3;
            }
            MainData.Target[RclickTime].x = x_val;
            MainData.Target[RclickTime].y = y_val;
            Local2GPS(x_val,y_val,0,
                      &this->MainData.TargetGps[RclickTime].lat,&MainData.TargetGps[RclickTime].lon,NULL);
            qDebug()<<"targetlocation " << RclickTime;


            RclickTime ++;
            break;

        }
        case QPlanEvent::human_way:
        {
            int x_pos = event->pos().x();
            int y_pos = event->pos().y();
            float x_val = ui->QGraph->xAxis->pixelToCoord(x_pos);
            float y_val = ui->QGraph->yAxis->pixelToCoord(y_pos);
            //this->MainData.avdSize[mID];
            auto  &avdID =  this->MainData.waySize[mID];
            Local2GPS(x_val,y_val,0,
                      &MainData.humanWayPointGps[mID][avdID].lat,&MainData.humanWayPointGps[mID][avdID].lon,NULL);


            qDebug()<<"WayPnt - wtf"<<endl;
            QCPItemEllipse *circle = new QCPItemEllipse(ui->QGraph);
            circle->topLeft->setCoords(x_val - 1, y_val +1);
            circle->bottomRight->setCoords(x_val +1 , y_val -1);

            QCPItemText *txt = new QCPItemText(ui->QGraph);
            txt->position->setCoords(x_val, y_val);
            QString QSt_index = QString("way-ID%1-%2").arg(this->mID).arg(++avdID);
            txt->setText(QSt_index);
            ui->QGraph->replot();
            break;

        }
        case QPlanEvent::search:
        {
            int x_pos = event->pos().x();
            int y_pos = event->pos().y();
            float x_val = ui->QGraph->xAxis->pixelToCoord(x_pos);
            float y_val = ui->QGraph->yAxis->pixelToCoord(y_pos);
            //m_aplan.getStartPnt(x_val, y_val);
            vx.push_back(x_val);
            vy.push_back(y_val);
            qDebug() <<vx.size()<< "SearchPnt.x  =  " << x_val;
            qDebug() <<vx.size()<< "SearchPnt.y  =  " << y_val;

            vx.push_back(vx.front());
            vy.push_back(vy.front());

            qDebug() <<vx.size()<< "SearchPnt.x  =  " << vx.back();
            qDebug() <<vx.size()<< "SearchPnt.y  =  " << vy.back();

            QCPCurve* ScoutArea = new QCPCurve(ui->QGraph->xAxis, ui->QGraph->yAxis);
            ScoutAreaData.push_back(QCPCurveData(lclickTimes,x_val,y_val));
            ScoutAreaData.push_back(QCPCurveData(lclickTimes,
                                                 ScoutAreaData.front().key
                                                 , ScoutAreaData.front().value));
            ScoutArea->data()->set(ScoutAreaData, false);

            ScoutArea->rescaleAxes(true);
            QPen pen;
            pen.setStyle(Qt::SolidLine);
            pen.setWidth(1);
            pen.setColor(Qt::GlobalColor::black);

            this->sRegionData = true;
            ScoutArea->setPen(pen);


            this->m_splan.setRange(vx,vy);

            //qDebug() << "startPnt.x  =  " << x_val;
            //qDebug() << "startPnt.y  =  " << y_val;
            //            vx.clear();
            //            vy.clear();
            //            //this->staGraphNum = ui->QGraph->graphCount();
            ScoutAreaData.clear();
            lclickTimes = 0 ;
            ui->QGraph->replot();
            break;
        }
        default:
            break;
        }
    }
}

void MainWindow::boxChange(int p)
{
    this->planEvent=p;
}

void MainWindow::idboxChange(int p)
{
    this->mID = p;
}

void MainWindow::modeChange(int p)
{
    this->m_mode = p ;
}
//motion planning
void MainWindow::on_pushButton_clicked()
{
    if (false)
    {
    }
    else
    {
        //        m_aplan.getStartPnt(-10.25,-62.25);
        //        m_aplan.getTargetPnt(-50.25,-40.25);
        //        m_aplan.getStartPnt(-25.5,-52);
        //        m_aplan.getTargetPnt(-33.5,-51.5);

        //        if(true)
        //        {
        //            while (this->staPloNum<ui->QGraph->plottableCount())
        //            {
        //                ui->QGraph->removePlottable(ui->QGraph->plottableCount()-1);
        //            }
        //            while (this->staItemNum<ui->QGraph->itemCount())
        //            {
        //                ui->QGraph->removeItem(ui->QGraph->itemCount()-1);
        //            }
        //        }


        auto planBoolean = m_aplan.AstarPlan();
        //draw part
        if (planBoolean)
        {
            m_aplan.qplot = ui->QGraph;
            m_aplan.drawPath();
            //m_aplan.slowDraw();
            m_aplan.drawS2T();
            //            m_aplan.drawCloseList();
            qDebug() << "plan success";
        }
        else
        {
            qDebug() << "plan  failed";
        }
    }
}

// test the search motion planning
void MainWindow::on_pushButton_2_clicked()
{
    if (false)
    {

    }
    else
    {

        //        vx.clear();
        //        vy.clear();
        //        vx.push_back(-50);
        //        vx.push_back(-47.4888);
        //        vx.push_back(-14.9327);
        //        vx.push_back(-20.0448);
        //        vx.push_back(-50);

        //        vy.push_back(-26.0538);
        //        vy.push_back(-54.3049);
        //        vy.push_back(-47.5785);
        //        vy.push_back(-20.0448);
        //        vy.push_back(-26.0538);
        this->m_splan.setPosition(-25.0936,-59.8127);
        //this->m_splan.GreedPlan();

        this->m_splan.setRange(vx,vy);
        clock_t start,finish;
        start = std::clock();
        this->m_splan.Plan();

        finish  = std::clock();

        auto duration = (double)(finish - start) / CLOCKS_PER_SEC;
        qDebug()<<"the total  duration  == "<<duration;

        this->m_splan.drawGraph(pl::GraphType::localGraph);
        //        this->m_splan.drawGraphEdges(pl::GraphType::localGraph);
        //        this->m_splan.drawvPath();
        //        this->m_splan.drawPath();
        //        this->m_splan.drawSpanningTree();

        this->m_splan.drawvPPath();
        vx.clear();
        vy.clear();
    }
}

//dp2p
void MainWindow::on_pushButton_4_clicked()
{

    qDebug()<<"wtf";

    long pmID[4] = {0,1,2,3};

    if((LclickTime ==4 )&&(RclickTime ==4))
        //excate the motion planning
    {
        LclickTime = 0;
        RclickTime = 0;
        if(true)
        {
            while (this->staPloNum<ui->QGraph->plottableCount())
            {
                ui->QGraph->removePlottable(ui->QGraph->plottableCount()-1);
            }
            while (this->staItemNum<ui->QGraph->itemCount())
            {
                ui->QGraph->removeItem(ui->QGraph->itemCount()-1);
            }
        }

        PathInfo *res = new PathInfo;


        res->p_path.pts = new PointGPS[200];

        AgentInfo currentlocation;
        currentlocation.agent.size = 4;
        currentlocation.agentID = 1;
        currentlocation.agent.ID  = pmID;

        currentlocation.agent.pts = MainData.currentLocationGps;

//        for(size_t j = 0; j<4;j++)
//        {
//            //            currentlocation.agent.pts[j].lat =39.96248664;
//            //            currentlocation.agent.pts[j].lon =116.3041191;
//            currentlocation.agent.pts[j].lat =39.96248664;
//            currentlocation.agent.pts[j].lon =116.3041191;
//        }


        for (size_t  j = 0 ;  j < 4 ; j++)
        {
            humanPathInfo humanpathUnit;

            humanpathUnit.avoid_Point.pts = MainData.humanAvdPointGps[j];
            humanpathUnit.avoid_Point.size = MainData.avdSize[j];

            humanpathUnit.way_Point.pts = MainData.humanWayPointGps[j];
            humanpathUnit.way_Point.size = MainData.waySize[j];


            this->MainData.Path.clear();
            currentlocation.agentID = j;

            PointTask targetLocation;
            targetLocation.target.lat = MainData.TargetGps[j].lat;
            targetLocation.target.lon = MainData.TargetGps[j].lon;

            //            targetLocation.target.lat = 39.96263820 ;
            //            targetLocation.target.lon = 116.30462592;

            //            targetLocation.target.lat = 39.96252905;
            //            targetLocation.target.lon = 116.3040638;


            //            targetLocation.target.lat = 39.96259209;
            //            targetLocation.target.lon = 116.3038707;

            // targetLocation.target.lat = 39.96257281;
            // targetLocation.target.lon = 116.3038457;


            auto disMark = distributedMotionPlanningHuman(res,targetLocation,currentlocation,this->m_mode,NULL,humanpathUnit);

            MainData.avdSize[j] = 0;
            MainData.waySize[j] = 0;

            if(disMark<0)
            {

            }
            else
            {
                for (size_t  i = 0 ; i< res->p_path.size; i++)
                {
                    double mx; double my;
                    GPS2Local(res->p_path.pts[i].lat,res->p_path.pts[i].lon,0,&mx,&my,NULL);
                    bgeo::DPoint pntUnit(mx,my);
                    this->MainData.Path.push_back(pntUnit);
                    //this->MainData.drawPath();
                }
//                if(j<3)
//                    continue;
                double len = bg::length(MainData.Path);
                qDebug()<<"the agent id is "<<j <<"the length is "<<len;
                MainData.Path = getDubinsPath(MainData.Path);
                this->MainData.drawPath(j);
            }
            std::cout<<" disMark = " <<disMark<< std::endl;
        }
    }

}


//distributed search motion planning
void MainWindow::on_pushButton_8_clicked()
{

    if(this->sRegionData)
    {
        long pmID[4] = {0,1,2,3};
        if(true)
        {
            while (this->staPloNum<ui->QGraph->plottableCount())
            {
                ui->QGraph->removePlottable(ui->QGraph->plottableCount()-1);
            }
            while (this->staItemNum<ui->QGraph->itemCount())
            {
                ui->QGraph->removeItem(ui->QGraph->itemCount()-1);
            }
        }

        PathInfo *res = new PathInfo;


        res->p_path.pts = new PointGPS[2000];

        AgentInfo currentlocation;
        currentlocation.agent.size = 4;
        currentlocation.agentID = 1;
        currentlocation.agent.ID  = pmID;

        currentlocation.agent.pts = MainData.currentLocationGps;
        humanPathInfo humanpathUnit;

        humanpathUnit.avoid_Point.pts = MainData.humanAvdPointGps[mID];
        humanpathUnit.avoid_Point.size = MainData.avdSize[mID];

        humanpathUnit.way_Point.pts = MainData.humanWayPointGps[mID];
        humanpathUnit.way_Point.size = MainData.waySize[mID];


        this->MainData.Path.clear();
        currentlocation.agentID = mID;
        PointArrary RegionData;
        RegionData.pts = new PointGPS[20];
        for(size_t i = 0; i <vx.size(); i++)
        {
            Local2GPS(vx.at(i),vy.at(i),0,&RegionData.pts[i].lat,&RegionData.pts[i].lon,nullptr);
        }
        RegionData.size = vx.size();

        auto disSearchMark  = distributedSearchMotionPlanningHuman(res,RegionData,currentlocation,0,nullptr,humanpathUnit);

        vx.clear();
        vy.clear();

        if(disSearchMark<0)
        {

        }
        else
        {
            for (size_t  i = 0 ; i< res->p_path.size; i++)
            {
                double mx; double my;
                GPS2Local(res->p_path.pts[i].lat,res->p_path.pts[i].lon,0,&mx,&my,NULL);
                bgeo::DPoint pntUnit(mx,my);
                this->MainData.Path.push_back(pntUnit);
            }
            double len = bg::length(MainData.Path);
            qDebug()<<"the agent id is "<<"the length is "<<len;
            //            this->MainData.drawPath(j);
//             MainData.Path = getDubinsPath(MainData.sPath);
            this->MainData.drawPath(0);
            this->MainData.Path.clear();
        }

        qDebug()<<"disSearchMark"<<disSearchMark;
    }
}


// redraw the plot
void MainWindow::on_pushButton_5_clicked()
{
    if(true)
    {
        while (this->staPloNum<ui->QGraph->plottableCount())
        {
            ui->QGraph->removePlottable(ui->QGraph->plottableCount()-1);
        }
        while (this->staItemNum<ui->QGraph->itemCount())
        {
            ui->QGraph->removeItem(ui->QGraph->itemCount()-1);
        }
        ui->QGraph->replot();
    }
}



//debug button
void MainWindow::on_pushButton_6_clicked()
{
    this->m_splan.drawGraph(pl::GraphType::localGraph);
    this->m_splan.drawvPath();
}

void MainWindow::on_pushButton_7_clicked()
{
    time_t tt=std::time(NULL);
    std::tm *t=std::localtime(&tt);

    auto sDay=std::to_string(t->tm_mday);
    auto sMonth = std::to_string(t->tm_mon+1);
    auto sHour = std::to_string(t->tm_hour);
    auto sMin = std::to_string(t->tm_min);
    std::string st_time = sMonth + "." + sDay +" hour:" +sHour + "." +sMin;

    st_time +="save";

    QString filename = QString::fromStdString(st_time);

    //QString file()
    ui->QGraph->savePdf(filename,0,0,QCP::epAllowCosmetic,filename,filename);
    ui->QGraph->saveJpg(filename);
}


// draw the grid for aggregation
void MainWindow::on_pushButton_9_clicked()
{
    //this->m_aplan.loadMap(MainMap);
    //    if (true)
    //    {
    //        if (false)
    //        {
    //            MainMap.drawgrid(ob::MapType::AggregationMap);

    //        }
    //        else
    //        {
    //            if(true)
    //            {
    //                MainMap.drawgrid(ob::MapType::GroupMap);
    //            }
    //            else{
    //                MainMap.drawgrid(ob::MapType::SearchMap);
    //            }
    //        }
    //    }
    //    this->m_aplan.m_map.drawgrid(ob::MapType::AggregationMap);
    this->m_aplan.m_map.drawgrid(ob::MapType::GroupMap);
}



void MainWindow::on_ComparisonSearch_clicked()
{
    if(this->sRegionData)
    {
        long pmID[4] = {0,1,2,3};
        if(true)
        {
            while (this->staPloNum<ui->QGraph->plottableCount())
            {
                ui->QGraph->removePlottable(ui->QGraph->plottableCount()-1);
            }
            while (this->staItemNum<ui->QGraph->itemCount())
            {
                ui->QGraph->removeItem(ui->QGraph->itemCount()-1);
            }
        }

        PathInfo *res = new PathInfo;


        res->p_path.pts = new PointGPS[2000];

        AgentInfo currentlocation;
        currentlocation.agent.size = 4;
        currentlocation.agentID = 1;
        currentlocation.agent.ID  = pmID;

        currentlocation.agent.pts = MainData.currentLocationGps;
        humanPathInfo humanpathUnit;

        humanpathUnit.avoid_Point.pts = MainData.humanAvdPointGps[mID];
        humanpathUnit.avoid_Point.size = MainData.avdSize[mID];

        humanpathUnit.way_Point.pts = MainData.humanWayPointGps[mID];
        humanpathUnit.way_Point.size = MainData.waySize[mID];


        this->MainData.Path.clear();
        currentlocation.agentID = mID;
        PointArrary RegionData;
        RegionData.pts = new PointGPS[20];
        for(size_t i = 0; i <vx.size(); i++)
        {
            Local2GPS(vx.at(i),vy.at(i),0,&RegionData.pts[i].lat,&RegionData.pts[i].lon,nullptr);
        }
        RegionData.size = vx.size();

        auto disSearchMark  = distributedSearchMotionPlanningHuman(res,RegionData,currentlocation,0,nullptr,humanpathUnit);

        vx.clear();
        vy.clear();

        if(disSearchMark<0)
        {

        }
        else
        {
            for (size_t  i = 0 ; i< res->p_path.size; i++)
            {
                double mx; double my;
                GPS2Local(res->p_path.pts[i].lat,res->p_path.pts[i].lon,0,&mx,&my,NULL);
                bgeo::DPoint pntUnit(mx,my);
                this->MainData.Path.push_back(pntUnit);
            }
            for (size_t targetID = 0; targetID < v_virtualTarget.size(); )
            for (size_t i = 0; i < MainData.Path.size();i++)
            {

            }
            double len = bg::length(MainData.Path);
            qDebug()<<"the agent id is "<<"the length is "<<len;
            //            this->MainData.drawPath(j);
            //             MainData.Path = getDubinsPath(MainData.sPath);
            this->MainData.drawPath(0);
            this->MainData.Path.clear();
        }
        qDebug()<<"disSearchMark"<<disSearchMark;
    }
}
