#include "demodata.h"
#include "XMl/tinystr.h"
#include "XMl/tinyxml.h"
#include "time.h"
DemoData::DemoData()
{

}

void DemoData::drawPath()
{
    this->Maindraw.LineString(this->Path);
}

void DemoData::drawPath(const int &i)
{
    this->Maindraw.LineString(this->Path,i);
}

bool DemoData::saveData(const char *configflie)
{
    TiXmlDocument doc;
    TiXmlDeclaration* decl = new TiXmlDeclaration("2.0", "", "");
    doc.LinkEndChild(decl);

    //    time_t tt=std::time(NULL);
    //    std::tm *t=std::localtime(&tt);

    //    auto sDay=std::to_string(t->tm_mday);
    //    auto sMonth = std::to_string(t->tm_mon+1);
    //    auto sHour = std::to_string(t->tm_hour);
    //    std::string st_time = sMonth + "." + sDay +" hour:" +sHour;
    //    const char * c_time = st_time.c_str();

    //    TiXmlElement * ptime = new TiXmlElement("time");
    //    doc.LinkEndChild(ptime);


    //    TiXmlComment * commentTime = new TiXmlComment();
    //    commentTime->SetValue(c_time);
    //    time->LinkEndChild(commentTime);

    auto d2c = [](const double &d) {char buf[200]; sprintf(buf, "%.10f", d); return buf; };

    {
        TiXmlElement * Proot = new TiXmlElement("p2p");
        doc.LinkEndChild(Proot);

        TiXmlComment * comment = new TiXmlComment();
        comment->SetValue("target and currentlocation");
        Proot->LinkEndChild(comment);


        for(size_t i = 0 ; i < 4; i++)
        {

            TiXmlElement * tube = new TiXmlElement("tube");
            Proot->LinkEndChild(tube);

            char c_buffer_lat[20];
            gcvt(this->currentLocationGps[i].lat,15,c_buffer_lat);
            tube->SetAttribute("c_lat",c_buffer_lat);

            char c_buffer_lon[20];
            gcvt(this->currentLocationGps[i].lon,15,c_buffer_lon);
            tube->SetAttribute("c_lon",c_buffer_lon);

            char t_buffer_lat[20];
            gcvt(this->TargetGps[i].lat,15,t_buffer_lat);
            tube->SetAttribute("t_lat",c_buffer_lat);

            char t_buffer_lon[20];
            gcvt(this->TargetGps[i].lon,15,t_buffer_lon);
            tube->SetAttribute("t_lon",c_buffer_lon);
        }
    }
    ///avd point
    {
        TiXmlElement * Proot = new TiXmlElement("avd");
        doc.LinkEndChild(Proot);

        TiXmlComment * comment = new TiXmlComment();
        comment->SetValue("avd");
        Proot->LinkEndChild(comment);


        for (size_t j = 0 ; j< 4 ; j++)
        {
            for (size_t i = 0; i < this->avdSize[j] ; i++)
            {
                TiXmlElement * tube = new TiXmlElement("tube");
                Proot->LinkEndChild(tube);


                tube->SetAttribute("avd_id",j);
                char c_buffer_lat[20];
                gcvt(this->humanAvdPointGps[j][i].lat,15,c_buffer_lat);
                tube->SetAttribute("avd_lat",c_buffer_lat);

                char c_buffer_lon[20];
                gcvt(this->humanAvdPointGps[j][i].lon,15,c_buffer_lon);
                tube->SetAttribute("avd_lon",c_buffer_lon);
            }
        }
    }
    /// way point
    {
        TiXmlElement * Proot = new TiXmlElement("way");
        doc.LinkEndChild(Proot);

        TiXmlComment * comment = new TiXmlComment();
        comment->SetValue("way");
        Proot->LinkEndChild(comment);


        for (size_t j = 0 ; j< 4 ; j++)
        {
            for (size_t i = 0; i < this->waySize[j] ; i++)
            {
                TiXmlElement * tube = new TiXmlElement("tube");
                Proot->LinkEndChild(tube);

                char c_buffer_lat[20];
                gcvt(this->humanWayPointGps[j][i].lat,15,c_buffer_lat);
                tube->SetAttribute("avd_lat",c_buffer_lat);

                char c_buffer_lon[20];
                gcvt(this->humanWayPointGps[j][i].lon,15,c_buffer_lon);
                tube->SetAttribute("avd_lon",c_buffer_lon);
            }
        }
    }
    doc.SaveFile(configflie);
    return false;
}
