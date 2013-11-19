#pragma once

#include "core.export.hpp"

#include <QMutex>
#include <QMutexLocker>
#include <QMap>

#define LockFunc() static QMutex* mutex##__LINE__ = new QMutex(); QMutexLocker locker##__LINE__(mutex##__LINE__)
