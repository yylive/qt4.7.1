/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtDeclarative module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qperformancetimer_p.h"

#if defined(Q_OS_MAC)
#include <sys/time.h>
#include <unistd.h>
#include <mach/mach_time.h>
#elif defined(Q_OS_SYMBIAN)
#include <e32std.h>
#include <sys/time.h>
#include <hal.h>
#include <hal_data.h>
#elif defined(Q_OS_UNIX)
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#elif defined(Q_OS_WIN)
#include <windows.h>
#endif

// mac/unix code heavily copied from QElapsedTimer

QT_BEGIN_NAMESPACE

////////////////////////////// Mac //////////////////////////////
#if defined(Q_OS_MAC)

static mach_timebase_info_data_t info = {0,0};
static qint64 absoluteToNSecs(qint64 cpuTime)
{
    if (info.denom == 0)
        mach_timebase_info(&info);
    qint64 nsecs = cpuTime * info.numer / info.denom;
    return nsecs;
}

void QPerformanceTimer::start()
{
    t1 = mach_absolute_time();
}

qint64 QPerformanceTimer::elapsed() const
{
    uint64_t cpu_time = mach_absolute_time();
    return absoluteToNSecs(cpu_time - t1);
}

////////////////////////////// Symbian //////////////////////////////
#elif defined(Q_OS_SYMBIAN)

static qint64 getTimeFromTick(quint64 elapsed)
{
    static TInt freq = 0;
    if (!freq)
        HAL::Get(HALData::EFastCounterFrequency, freq);

    return (elapsed * 1000000000) / freq;
}

void QPerformanceTimer::start()
{
    t1 = User::FastCounter();
}

qint64 QPerformanceTimer::elapsed() const
{
    return getTimeFromTick(User::FastCounter() - t1);
}


////////////////////////////// Unix //////////////////////////////
#elif defined(Q_OS_UNIX)

#if defined(QT_NO_CLOCK_MONOTONIC) || defined(QT_BOOTSTRAPPED)
// turn off the monotonic clock
# ifdef _POSIX_MONOTONIC_CLOCK
#  undef _POSIX_MONOTONIC_CLOCK
# endif
# define _POSIX_MONOTONIC_CLOCK -1
#endif

#if (_POSIX_MONOTONIC_CLOCK-0 != 0)
static const bool monotonicClockChecked = true;
static const bool monotonicClockAvailable = _POSIX_MONOTONIC_CLOCK > 0;
#else
static int monotonicClockChecked = false;
static int monotonicClockAvailable = false;
#endif

#ifdef Q_CC_GNU
# define is_likely(x) __builtin_expect((x), 1)
#else
# define is_likely(x) (x)
#endif
#define load_acquire(x) ((volatile const int&)(x))
#define store_release(x,v) ((volatile int&)(x) = (v))

static void unixCheckClockType()
{
#if (_POSIX_MONOTONIC_CLOCK-0 == 0)
    if (is_likely(load_acquire(monotonicClockChecked)))
        return;

# if defined(_SC_MONOTONIC_CLOCK)
    // detect if the system support monotonic timers
    long x = sysconf(_SC_MONOTONIC_CLOCK);
    store_release(monotonicClockAvailable, x >= 200112L);
# endif

    store_release(monotonicClockChecked, true);
#endif
}

static inline void do_gettime(qint64 *sec, qint64 *frac)
{
#if (_POSIX_MONOTONIC_CLOCK-0 >= 0)
    unixCheckClockType();
    if (is_likely(monotonicClockAvailable)) {
        timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        *sec = ts.tv_sec;
        *frac = ts.tv_nsec;
        return;
    }
#endif
    *sec = 0;
    *frac = 0;
}

void QPerformanceTimer::start()
{
    do_gettime(&t1, &t2);
}

qint64 QPerformanceTimer::elapsed() const
{
    qint64 sec, frac;
    do_gettime(&sec, &frac);
    sec = sec - t1;
    frac = frac - t2;

    return sec * Q_INT64_C(1000000000) + frac;
}

////////////////////////////// Windows //////////////////////////////
#elif defined(Q_OS_WIN)

static qint64 getTimeFromTick(quint64 elapsed)
{
    static LARGE_INTEGER freq;
    if (!freq.QuadPart)
        QueryPerformanceFrequency(&freq);
    return 1000000000 * elapsed / freq.QuadPart;
}

void QPerformanceTimer::start()
{
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    t1 = li.QuadPart;
}

qint64 QPerformanceTimer::elapsed() const
{
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    return getTimeFromTick(li.QuadPart - t1);
}

////////////////////////////// Default //////////////////////////////
#else

// default implementation (no hi-perf timer) does nothing
void QPerformanceTimer::start()
{
}

qint64 QPerformanceTimer::elapsed() const
{
    return 0;
}

#endif

QT_END_NAMESPACE


