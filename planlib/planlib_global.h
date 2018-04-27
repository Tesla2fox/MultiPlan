#ifndef PLANLIB_GLOBAL_H
#define PLANLIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PLANLIB_LIBRARY)
#  define PLANLIBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define PLANLIBSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // PLANLIB_GLOBAL_H
