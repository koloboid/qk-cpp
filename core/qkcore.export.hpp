#pragma once

#include <QtCore/qglobal.h>

#if defined(QKCORE_LIBRARY)
#  define QKCORE_EXPORT Q_DECL_EXPORT
#else
#  define QKCORE_EXPORT Q_DECL_IMPORT
#endif
