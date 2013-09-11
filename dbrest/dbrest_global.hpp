#ifndef DBREST_GLOBAL_HPP
#define DBREST_GLOBAL_HPP

#include <QtCore/qglobal.h>

#if defined(DBREST_LIBRARY)
#  define DBRESTSHARED_EXPORT Q_DECL_EXPORT
#else
#  define DBRESTSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // DBREST_GLOBAL_HPP
