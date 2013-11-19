#include "qklog.hpp"
#include <signal.h>
#include <execinfo.h>
#include <QObject>
#include <cxxabi.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

QString QkLogBase::getBacktrace(quint8 pMaxSize)
{
    void* buf[pMaxSize];
    memset(buf, 0, pMaxSize * sizeof(void*));
    int sz = backtrace(buf, pMaxSize);
    char** bt = backtrace_symbols(buf, sz);
    QString mBacktrace;
    if (bt)
    {
        int status;
        for (int i = 2; i < sz; i++)
        {
            QString line(bt[i]);
            int p1 = line.indexOf("(");
            int p2 = line.indexOf("+");
            if (p1 >= 0 && p2 > p1)
            {
                QString mangled = line.mid(p1 + 1, p2 - p1 - 1);
                char* name = abi::__cxa_demangle(mangled.toUtf8().data(), 0, 0, &status);
                if (name)
                {
                    QString r = line.right(line.size() - p2);
                    line = line.left(p1) + "(" + name + ")" + r;
                }
                else line = "";
                free(name);
            }
            if (!line.isEmpty())
            {
                mBacktrace.append(line);
            }
            else
            {
                mBacktrace.append(QObject::tr("Unable to fetch function name, Status=%1. Original=%2").
                                  arg(status).arg(bt[i]));
            }
            mBacktrace.append("\n");
        }
        free(bt);
    }
    else
    {
        if (sz > 0)
        {
            for (int i = 0; i < sz; i++)
            {
                mBacktrace.append("0x%1\n").arg((intptr_t)buf[i], 16, 16, QChar('0'));
            }
        }
        else
        {
            mBacktrace = QObject::tr("No backtrace available");
        }
    }
    return mBacktrace;
}