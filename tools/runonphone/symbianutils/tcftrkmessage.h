/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the tools applications of the Qt Toolkit.
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

#ifndef TRCFTRKMESSAGE_H
#define TRCFTRKMESSAGE_H

#include "symbianutils_global.h"

#include <QtCore/QStringList>
#include <QtCore/QVector>

QT_BEGIN_NAMESPACE
class QTextStream;
QT_END_NAMESPACE

namespace tcftrk {

class JsonValue;
class JsonInputStream;

enum Services {
    LocatorService,
    RunControlService,
    ProcessesService,
    MemoryService,
    SettingsService,  // non-standard, trk specific
    BreakpointsService,
    RegistersService,
    SimpleRegistersService, // non-standard, trk specific
    UnknownService
}; // Note: Check string array 'serviceNamesC' of same size when modifying this.

// Modes of RunControl/'Resume' (see EDF documentation).
// As of 24.6.2010, RM_RESUME, RM_STEP_OVER, RM_STEP_INTO,
// RM_STEP_OVER_RANGE, RM_STEP_INTO_RANGE are supported with
// RANG_START/RANGE_END parameters.
enum RunControlResumeMode {
    RM_RESUME = 0,
    RM_STEP_OVER = 1, RM_STEP_INTO = 2,
    RM_STEP_OVER_LINE = 3, RM_STEP_INTO_LINE = 4,
    RM_STEP_OUT = 5, RM_REVERSE_RESUME = 6,
    RM_REVERSE_STEP_OVER = 7, RM_REVERSE_STEP_INTO = 8,
    RM_REVERSE_STEP_OVER_LINE = 9, RM_REVERSE_STEP_INTO_LINE = 10,
    RM_REVERSE_STEP_OUT = 11, RM_STEP_OVER_RANGE = 12,
    RM_STEP_INTO_RANGE = 13, RM_REVERSE_STEP_OVER_RANGE = 14,
    RM_REVERSE_STEP_INTO_RANGE = 15
};

SYMBIANUTILS_EXPORT const char *serviceName(Services s);
SYMBIANUTILS_EXPORT Services serviceFromName(const char *);

// Debug helpers
SYMBIANUTILS_EXPORT QString formatData(const QByteArray &a);
SYMBIANUTILS_EXPORT QString joinByteArrays(const QVector<QByteArray> &a, char sep = ',');

// Context used in 'RunControl contextAdded' events and in reply
// to 'Processes start'. Could be thread or process.
struct SYMBIANUTILS_EXPORT RunControlContext {
    enum Flags {
        Container = 0x1, HasState = 0x2, CanSuspend = 0x4,
        CanTerminate = 0x8
    };
    enum Type { Process, Thread };

    RunControlContext();
    Type type() const;
    unsigned processId() const;
    unsigned threadId() const;

    void clear();
    bool parse(const JsonValue &v);
    void format(QTextStream &str) const;
    QString toString() const;

    // Helper for converting the TCF ids ("p12" or "p12.t34")
    static Type typeFromTcfId(const QByteArray &id);
    static unsigned processIdFromTcdfId(const QByteArray &id);
    static unsigned threadIdFromTcdfId(const QByteArray &id);
    static QByteArray tcfId(unsigned processId,  unsigned threadId = 0);

    unsigned flags;
    unsigned resumeFlags;
    QByteArray id;     // "p434.t699"
    QByteArray osid;   // Non-standard: Process or thread id
    QByteArray parentId; // Parent process id of a thread.
};

// Module load information occurring with 'RunControl contextSuspended' events
struct SYMBIANUTILS_EXPORT ModuleLoadEventInfo {
    ModuleLoadEventInfo();
    void clear();
    bool parse(const JsonValue &v);
    void format(QTextStream &str) const;

    QByteArray name;
    QByteArray file;
    bool loaded;
    quint64 codeAddress;
    quint64 dataAddress;
    bool requireResume;
};

// Breakpoint as supported by TcfTrk source June 2010
// TODO: Add watchpoints,etc once they are implemented
struct SYMBIANUTILS_EXPORT Breakpoint {
    enum Type { Software, Hardware, Auto };

    explicit Breakpoint(quint64 loc = 0);
    void setContextId(unsigned processId, unsigned threadId = 0);
    QString toString() const;

    static QByteArray idFromLocation(quint64 loc); // Automagically determine from location

    Type type;
    bool enabled;
    int ignoreCount;
    QVector<QByteArray> contextIds;   // Process or thread ids.
    QByteArray id;                    // Id of the breakpoint;
    quint64 location;
    unsigned size;
    bool thumb;
};

SYMBIANUTILS_EXPORT JsonInputStream &operator<<(JsonInputStream &str, const Breakpoint &b);

// Event hierarchy
class SYMBIANUTILS_EXPORT TcfTrkEvent {
    Q_DISABLE_COPY(TcfTrkEvent)
public:
    enum Type { None,
                LocatorHello,
                RunControlContextAdded,
                RunControlContextRemoved,
                RunControlSuspended,
                RunControlBreakpointSuspended,
                RunControlModuleLoadSuspended,
                RunControlResumed
              };

    virtual ~TcfTrkEvent();

    Type type() const;
    virtual QString toString() const;

    static TcfTrkEvent *parseEvent(Services s, const QByteArray &name, const QVector<JsonValue> &val);

protected:
    explicit TcfTrkEvent(Type type = None);

private:
    const Type m_type;
};

// ServiceHello
class SYMBIANUTILS_EXPORT TcfTrkLocatorHelloEvent : public TcfTrkEvent {
public:
    explicit TcfTrkLocatorHelloEvent(const QStringList &);

    const QStringList &services() { return m_services; }
    virtual QString toString() const;

private:
    QStringList m_services;
};

// Base for events that just have one id as parameter
// (simple suspend)
class SYMBIANUTILS_EXPORT TcfTrkIdEvent : public TcfTrkEvent {
protected:
    explicit TcfTrkIdEvent(Type t, const QByteArray &id);
public:
    QByteArray id() const { return m_id; }
    QString idString() const { return QString::fromUtf8(m_id); }

private:
    const QByteArray m_id;
};

// Base for events that just have some ids as parameter
// (context removed)
class SYMBIANUTILS_EXPORT TcfTrkIdsEvent : public TcfTrkEvent {
protected:
    explicit TcfTrkIdsEvent(Type t, const QVector<QByteArray> &ids);

public:
    QVector<QByteArray> ids() const { return m_ids; }
    QString joinedIdString(const char sep = ',') const;

private:
    const QVector<QByteArray> m_ids;
};

// RunControlContextAdded
class SYMBIANUTILS_EXPORT TcfTrkRunControlContextAddedEvent : public TcfTrkEvent {
public:
    typedef QVector<RunControlContext> RunControlContexts;

    explicit TcfTrkRunControlContextAddedEvent(const RunControlContexts &c);

    const RunControlContexts &contexts() const { return m_contexts; }
    virtual QString toString() const;

    static TcfTrkRunControlContextAddedEvent *parseEvent(const QVector<JsonValue> &val);

private:
    const RunControlContexts m_contexts;
};

// RunControlContextRemoved
class SYMBIANUTILS_EXPORT TcfTrkRunControlContextRemovedEvent : public TcfTrkIdsEvent {
public:
    explicit TcfTrkRunControlContextRemovedEvent(const QVector<QByteArray> &id);
    virtual QString toString() const;
};

// Simple RunControlContextSuspended (process/thread)
class SYMBIANUTILS_EXPORT TcfTrkRunControlContextSuspendedEvent : public TcfTrkIdEvent {
public:
    enum Reason  { BreakPoint, ModuleLoad, Crash, Other } ;

    explicit TcfTrkRunControlContextSuspendedEvent(const QByteArray &id,
                                                   const QByteArray &reason,
                                                   quint64 pc = 0);
    virtual QString toString() const;

    quint64 pc() const { return m_pc; }
    QByteArray reasonID() const { return m_reason; }
    Reason reason() const;

protected:
    explicit TcfTrkRunControlContextSuspendedEvent(Type t,
                                                   const QByteArray &id,
                                                   const QByteArray &reason,
                                                   quint64 pc = 0);
    void format(QTextStream &str) const;

private:
    const quint64 m_pc;
    const QByteArray m_reason;
};

// RunControlContextSuspended due to module load
class SYMBIANUTILS_EXPORT TcfTrkRunControlModuleLoadContextSuspendedEvent : public TcfTrkRunControlContextSuspendedEvent {
public:
    explicit TcfTrkRunControlModuleLoadContextSuspendedEvent(const QByteArray &id,
                                                             const QByteArray &reason,
                                                             quint64 pc,
                                                             const ModuleLoadEventInfo &mi);

    virtual QString toString() const;
    const ModuleLoadEventInfo &info() const { return m_mi; }

private:
    const ModuleLoadEventInfo m_mi;
};

} // namespace tcftrk
#endif // TRCFTRKMESSAGE_H
