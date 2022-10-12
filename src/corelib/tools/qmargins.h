/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtCore module of the Qt Toolkit.
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

#ifndef QMARGINS_H
#define QMARGINS_H

#include <QtCore/qnamespace.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

QT_MODULE(Core)

class QMargins
{
public:
    QMargins();
    QMargins(int left, int top, int right, int bottom);

    bool isNull() const;

    int left() const;
    int top() const;
    int right() const;
    int bottom() const;

    void setLeft(int left);
    void setTop(int top);
    void setRight(int right);
    void setBottom(int bottom);

private:
    int m_left;
    int m_top;
    int m_right;
    int m_bottom;

    friend inline bool operator==(const QMargins &, const QMargins &);
    friend inline bool operator!=(const QMargins &, const QMargins &);
};

Q_DECLARE_TYPEINFO(QMargins, Q_MOVABLE_TYPE);

/*****************************************************************************
  QMargins inline functions
 *****************************************************************************/

inline QMargins::QMargins()
{ m_top = m_bottom = m_left = m_right = 0; }

inline QMargins::QMargins(int aleft, int atop, int aright, int abottom)
    : m_left(aleft), m_top(atop), m_right(aright), m_bottom(abottom) {}

inline bool QMargins::isNull() const
{ return m_left==0 && m_top==0 && m_right==0 && m_bottom==0; }

inline int QMargins::left() const
{ return m_left; }

inline int QMargins::top() const
{ return m_top; }

inline int QMargins::right() const
{ return m_right; }

inline int QMargins::bottom() const
{ return m_bottom; }


inline void QMargins::setLeft(int aleft)
{ m_left = aleft; }

inline void QMargins::setTop(int atop)
{ m_top = atop; }

inline void QMargins::setRight(int aright)
{ m_right = aright; }

inline void QMargins::setBottom(int abottom)
{ m_bottom = abottom; }

inline bool operator==(const QMargins &m1, const QMargins &m2)
{
    return
            m1.m_left == m2.m_left &&
            m1.m_top == m2.m_top &&
            m1.m_right == m2.m_right &&
            m1.m_bottom == m2.m_bottom;
}

inline bool operator!=(const QMargins &m1, const QMargins &m2)
{
    return
            m1.m_left != m2.m_left ||
            m1.m_top != m2.m_top ||
            m1.m_right != m2.m_right ||
            m1.m_bottom != m2.m_bottom;
}

/*****************************************************************************
QMarginsF class
*****************************************************************************/

class QMarginsF
{
public:
	 QMarginsF() ;
	 QMarginsF(qreal left, qreal top, qreal right, qreal bottom) ;
	 QMarginsF(const QMargins &margins) ;

	 bool isNull() const ;

	 qreal left() const ;
	 qreal top() const ;
	 qreal right() const ;
	 qreal bottom() const ;

	 void setLeft(qreal left) ;
	 void setTop(qreal top) ;
	 void setRight(qreal right) ;
	 void setBottom(qreal bottom) ;

	 QMarginsF &operator+=(const QMarginsF &margins) ;
	 QMarginsF &operator-=(const QMarginsF &margins) ;
	 QMarginsF &operator+=(qreal addend) ;
	 QMarginsF &operator-=(qreal subtrahend) ;
	 QMarginsF &operator*=(qreal factor) ;
	 QMarginsF &operator/=(qreal divisor);

	 inline QMargins toMargins() const ;

private:
	qreal m_left;
	qreal m_top;
	qreal m_right;
	qreal m_bottom;
};

Q_DECLARE_TYPEINFO(QMarginsF, Q_MOVABLE_TYPE);

/*****************************************************************************
QMarginsF stream functions
*****************************************************************************/

/*****************************************************************************
QMarginsF inline functions
*****************************************************************************/

 inline QMarginsF::QMarginsF() 
	: m_left(0), m_top(0), m_right(0), m_bottom(0) {}

 inline QMarginsF::QMarginsF(qreal aleft, qreal atop, qreal aright, qreal abottom) 
	: m_left(aleft), m_top(atop), m_right(aright), m_bottom(abottom) {}

 inline QMarginsF::QMarginsF(const QMargins &margins) 
	: m_left(margins.left()), m_top(margins.top()), m_right(margins.right()), m_bottom(margins.bottom()) {}

 inline bool QMarginsF::isNull() const 
{
	return qFuzzyIsNull(m_left) && qFuzzyIsNull(m_top) && qFuzzyIsNull(m_right) && qFuzzyIsNull(m_bottom);
}

 inline qreal QMarginsF::left() const 
{
	return m_left;
}

 inline qreal QMarginsF::top() const 
{
	return m_top;
}

 inline qreal QMarginsF::right() const 
{
	return m_right;
}

 inline qreal QMarginsF::bottom() const 
{
	return m_bottom;
}


 inline void QMarginsF::setLeft(qreal aleft) 
{
	m_left = aleft;
}

 inline void QMarginsF::setTop(qreal atop) 
{
	m_top = atop;
}

 inline void QMarginsF::setRight(qreal aright) 
{
	m_right = aright;
}

 inline void QMarginsF::setBottom(qreal abottom) 
{
	m_bottom = abottom;
}

 inline bool operator==(const QMarginsF &lhs, const QMarginsF &rhs) 
{
	return qFuzzyCompare(lhs.left(), rhs.left())
		&& qFuzzyCompare(lhs.top(), rhs.top())
		&& qFuzzyCompare(lhs.right(), rhs.right())
		&& qFuzzyCompare(lhs.bottom(), rhs.bottom());
}

 inline bool operator!=(const QMarginsF &lhs, const QMarginsF &rhs) 
{
	return !operator==(lhs, rhs);
}

 inline QMarginsF operator+(const QMarginsF &lhs, const QMarginsF &rhs) 
{
	return QMarginsF(lhs.left() + rhs.left(), lhs.top() + rhs.top(),
		lhs.right() + rhs.right(), lhs.bottom() + rhs.bottom());
}

 inline QMarginsF operator-(const QMarginsF &lhs, const QMarginsF &rhs) 
{
	return QMarginsF(lhs.left() - rhs.left(), lhs.top() - rhs.top(),
		lhs.right() - rhs.right(), lhs.bottom() - rhs.bottom());
}

 inline QMarginsF operator+(const QMarginsF &lhs, qreal rhs) 
{
	return QMarginsF(lhs.left() + rhs, lhs.top() + rhs,
		lhs.right() + rhs, lhs.bottom() + rhs);
}

 inline QMarginsF operator+(qreal lhs, const QMarginsF &rhs) 
{
	return QMarginsF(rhs.left() + lhs, rhs.top() + lhs,
		rhs.right() + lhs, rhs.bottom() + lhs);
}

 inline QMarginsF operator-(const QMarginsF &lhs, qreal rhs) 
{
	return QMarginsF(lhs.left() - rhs, lhs.top() - rhs,
		lhs.right() - rhs, lhs.bottom() - rhs);
}

 inline QMarginsF operator*(const QMarginsF &lhs, qreal rhs) 
{
	return QMarginsF(lhs.left() * rhs, lhs.top() * rhs,
		lhs.right() * rhs, lhs.bottom() * rhs);
}

 inline QMarginsF operator*(qreal lhs, const QMarginsF &rhs) 
{
	return QMarginsF(rhs.left() * lhs, rhs.top() * lhs,
		rhs.right() * lhs, rhs.bottom() * lhs);
}

 inline QMarginsF operator/(const QMarginsF &lhs, qreal divisor)
{
	return QMarginsF(lhs.left() / divisor, lhs.top() / divisor,
		lhs.right() / divisor, lhs.bottom() / divisor);
}

 inline QMarginsF &QMarginsF::operator+=(const QMarginsF &margins) 
{
	return *this = *this + margins;
}

 inline QMarginsF &QMarginsF::operator-=(const QMarginsF &margins) 
{
	return *this = *this - margins;
}

 inline QMarginsF &QMarginsF::operator+=(qreal addend) 
{
	m_left += addend;
	m_top += addend;
	m_right += addend;
	m_bottom += addend;
	return *this;
}

 inline QMarginsF &QMarginsF::operator-=(qreal subtrahend) 
{
	m_left -= subtrahend;
	m_top -= subtrahend;
	m_right -= subtrahend;
	m_bottom -= subtrahend;
	return *this;
}

 inline QMarginsF &QMarginsF::operator*=(qreal factor) 
{
	return *this = *this * factor;
}

 inline QMarginsF &QMarginsF::operator/=(qreal divisor)
{
	return *this = *this / divisor;
}

 inline QMarginsF operator+(const QMarginsF &margins) 
{
	return margins;
}

 inline QMarginsF operator-(const QMarginsF &margins) 
{
	return QMarginsF(-margins.left(), -margins.top(), -margins.right(), -margins.bottom());
}

 inline QMargins QMarginsF::toMargins() const 
{
	return QMargins(qRound(m_left), qRound(m_top), qRound(m_right), qRound(m_bottom));
}


#ifndef QT_NO_DEBUG_STREAM
Q_CORE_EXPORT QDebug operator<<(QDebug, const QMargins &);
#endif

QT_END_NAMESPACE

QT_END_HEADER

#endif // QMARGINS_H
