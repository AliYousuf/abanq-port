/****************************************************************************
** $Id: qt/qspinbox.h   3.3.6   edited Aug 31 2005 $
**
** Definition of QSpinBox widget class
**
** Created : 970101
**
** Copyright (C) 1992-2005 Trolltech AS.  All rights reserved.
**
** This file is part of the widgets module of the Qt GUI Toolkit.
**
** This file may be distributed under the terms of the Q Public License
** as defined by Trolltech AS of Norway and appearing in the file
** LICENSE.QPL included in the packaging of this file.
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** Licensees holding valid Qt Enterprise Edition or Qt Professional Edition
** licenses may use this file in accordance with the Qt Commercial License
** Agreement provided with the Software.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.trolltech.com/pricing.html or email sales@trolltech.com for
**   information about Qt Commercial License Agreements.
** See http://www.trolltech.com/qpl/ for QPL licensing information.
** See http://www.trolltech.com/gpl/ for GPL licensing information.
**
** Contact info@trolltech.com if any conditions of this licensing are
** not clear to you.
**
**********************************************************************/

#ifndef QSPINBOX_H
#define QSPINBOX_H

#ifndef QT_H
#include "qwidget.h"
#include "qrangecontrol.h"
#endif // QT_H

#ifndef QT_NO_SPINBOX

class QLineEdit;
class QValidator;
class QSpinBoxPrivate;

class Q_EXPORT QSpinBox: public QWidget, public QRangeControl
{
    Q_OBJECT
    Q_ENUMS( ButtonSymbols )
    Q_PROPERTY( QString text READ text )
    Q_PROPERTY( QString prefix READ prefix WRITE setPrefix )
    Q_PROPERTY( QString suffix READ suffix WRITE setSuffix )
    Q_PROPERTY( QString cleanText READ cleanText )
    Q_PROPERTY( QString specialValueText READ specialValueText WRITE setSpecialValueText )
    Q_PROPERTY( bool wrapping READ wrapping WRITE setWrapping )
    Q_PROPERTY( ButtonSymbols buttonSymbols READ buttonSymbols WRITE setButtonSymbols )
    Q_PROPERTY( int maxValue READ maxValue WRITE setMaxValue )
    Q_PROPERTY( int minValue READ minValue WRITE setMinValue )
    Q_PROPERTY( int lineStep READ lineStep WRITE setLineStep )
    Q_PROPERTY( int value READ value WRITE setValue )

public:
    QSpinBox( QWidget* parent=0, const char* name=0 );
    QSpinBox( int minValue, int maxValue, int step = 1,
	      QWidget* parent=0, const char* name=0 );
    ~QSpinBox();

    QString		text() const;

    virtual QString	prefix() const;
    virtual QString	suffix() const;
    virtual QString	cleanText() const;

    virtual void	setSpecialValueText( const QString &text );
    QString		specialValueText() const;

    virtual void	setWrapping( bool on );
    bool		wrapping() const;

    enum ButtonSymbols { UpDownArrows, PlusMinus };
    virtual void	setButtonSymbols( ButtonSymbols );
    ButtonSymbols	buttonSymbols() const;

    virtual void	setValidator( const QValidator* v );
    const QValidator * validator() const;

    QSize		sizeHint() const;
    QSize		minimumSizeHint() const;

    int	 minValue() const;
    int	 maxValue() const;
    void setMinValue( int );
    void setMaxValue( int );
    int	 lineStep() const;
    void setLineStep( int );
    int  value() const;

    QRect		upRect() const;
    QRect		downRect() const;

public slots:
    virtual void	setValue( int value );
    virtual void	setPrefix( const QString &text );
    virtual void	setSuffix( const QString &text );
    virtual void	stepUp();
    virtual void	stepDown();
    virtual void 	setEnabled( bool enabled );
    virtual void 	selectAll();

signals:
    void		valueChanged( int value );
    void		valueChanged( const QString &valueText );

protected:
    virtual QString	mapValueToText( int value );
    virtual int		mapTextToValue( bool* ok );
    QString		currentValueText();

    virtual void	updateDisplay();
    virtual void	interpretText();

    QLineEdit*		editor() const;

    virtual void	valueChange();
    virtual void	rangeChange();

    bool		eventFilter( QObject* obj, QEvent* ev );
    void		resizeEvent( QResizeEvent* ev );
#ifndef QT_NO_WHEELEVENT
    void		wheelEvent( QWheelEvent * );
#endif
    void		leaveEvent( QEvent* );

    void		styleChange( QStyle& );

protected slots:
    void		textChanged();

private:
    void initSpinBox();
    QSpinBoxPrivate* d;
    QLineEdit* vi;
    QValidator* validate;
    QString pfix;
    QString sfix;
    QString specText;

    uint wrap		: 1;
    uint edited		: 1;

    void arrangeWidgets();

private:	// Disabled copy constructor and operator=
#if defined(Q_DISABLE_COPY)
    QSpinBox( const QSpinBox& );
    QSpinBox& operator=( const QSpinBox& );
#endif

};

#endif // QT_NO_SPINBOX

#endif // QSPINBOX_H
