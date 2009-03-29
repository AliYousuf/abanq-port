/**********************************************************************
**
** Copyright (C) 2005 Trolltech AS.  All rights reserved.
**
** This file is part of Qt Designer.
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
** See http://www.trolltech.com/gpl/ for GPL licensing information.
** See http://www.trolltech.com/pricing.html or email sales@trolltech.com for
**   information about Qt Commercial License Agreements.
**
** Contact info@trolltech.com if any conditions of this licensing are
** not clear to you.
**
**********************************************************************/

#include "editorinterfaceimpl.h"
#include "languageinterfaceimpl.h"
#include "preferenceinterfaceimpl.h"
#include "projectsettingsinterfaceimpl.h"
#include "sourcetemplateinterfaceimpl.h"

class CommonInterface : public QComponentInformationInterface
{
public:
    CommonInterface();
    virtual ~CommonInterface();

    QRESULT queryInterface( const QUuid&, QUnknownInterface** );
    Q_REFCOUNT;

    QString name() const { return "C++"; }
    QString description() const { return "C++ Integration"; }
    QString version() const { return "0.1"; }
    QString author() const { return "Trolltech AS"; }

private:
    LanguageInterfaceImpl *langIface;
    PreferenceInterfaceImpl *prefIface;
    ProjectSettingsInterfaceImpl *proIface;
    SourceTemplateInterfaceImpl *srcIface;

};

CommonInterface::CommonInterface()
{
    langIface = new LanguageInterfaceImpl( this );
    langIface->addRef();
    prefIface = new PreferenceInterfaceImpl( this );
    prefIface->addRef();
    proIface = new ProjectSettingsInterfaceImpl( this );
    proIface->addRef();
    srcIface = new SourceTemplateInterfaceImpl;
    srcIface->addRef();
}

CommonInterface::~CommonInterface()
{
    langIface->release();
    prefIface->release();
    proIface->release();
    srcIface->release();
}

QRESULT CommonInterface::queryInterface( const QUuid &uuid, QUnknownInterface** iface )
{
    *iface = 0;
    if ( uuid == IID_QUnknown )
	*iface = (QUnknownInterface*)this;
    else if ( uuid == IID_QComponentInformation )
	*iface = (QComponentInformationInterface*)this;
    else if ( uuid == IID_Editor )
	*iface = new EditorInterfaceImpl;
    else if ( uuid == IID_Language )
	*iface = langIface;
    else if ( uuid == IID_Preference )
	*iface = prefIface;
    else if ( uuid == IID_ProjectSettings )
	*iface = proIface;
    else if ( uuid == IID_SourceTemplate )
	*iface = srcIface;
    else
	return QE_NOINTERFACE;

    (*iface)->addRef();
    return QS_OK;
}


Q_EXPORT_COMPONENT()
{
     Q_CREATE_INSTANCE( CommonInterface )
}
