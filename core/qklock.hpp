#pragma once

#include "qkcore.export.hpp"

#include <QMutex>
#include <QMutexLocker>
#include <QMap>

#define QkLockFunc() static QMutex* mutex##__LINE__ = new QMutex(); QMutexLocker locker##__LINE__(mutex##__LINE__)
