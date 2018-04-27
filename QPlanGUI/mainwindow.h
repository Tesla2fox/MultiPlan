#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Aplan.h"
#include "splan.h"
#include "obmap.h"

#include "code/demodata.h"
#include "Obstacles.hpp"

namespace Ui {
class MainWindow;
}

enum  QPlanEvent
{
    noplan,
    Aggregation,
    search,
    human_avoid,
    human_way
};
enum GMode{
    AgentMode,
    GroupMode
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
protected:
    void resizeEvent(QResizeEvent *event);
private slots:
    void mousePress(QMouseEvent *);
    void boxChange(int);
    void idboxChange(int);
    void modeChange(int);

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_9_clicked();

private:
    Ui::MainWindow *ui;
    ob::Obmap MainMap;
    pl::Aplan m_aplan;
    pl::Splan m_splan;
    size_t planEvent;

    std::vector<double> vx;
    std::vector<double> vy;

    size_t lclickTimes =0;
    QVector<QCPCurveData> ScoutAreaData;

    int  staGraphNum = 0;
    int  staItemNum = 0;
    int  staPloNum = 0;

    int mID = 0;

    DemoData MainData;

    size_t  LclickTime = 0;
    size_t  RclickTime = 0;

    size_t  HLclickTime = 0;
    size_t  HRclickTime = 0;

    size_t  m_mode  = 0;

    bool sRegionData = false;
    impl::MapData _OutLine;
    impl::MapData _inLine;
    // input the region data or not;
};

#endif // MAINWINDOW_H
