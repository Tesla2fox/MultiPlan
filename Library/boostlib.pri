
#-------------------------------------------------
#
# Project created by QtCreator 2017-08-27T11:06:34
#
#-------------------------------------------------

win32{
BASE_QBOOSTLIB    = E:\boost\boost_1_65_0

}
unix
{
BASE_QBOOSTLIB= /usr/local/boost
}

message("BASE_QBOOSTLIB is " $${BASE_QSSCONFIG})
INCLUDEPATH    += $${BASE_QBOOSTLIB}
#HEADERS        += $${BASE_QSSCONFIG}/ssconfig.hpp

