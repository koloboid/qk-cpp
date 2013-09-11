#pragma once

#include <QtCore/qglobal.h>

#if defined(QKDB_LIBRARY)
#  define QKDB_EXPORT Q_DECL_EXPORT
#else
#  define QKDB_EXPORT Q_DECL_IMPORT
#endif
