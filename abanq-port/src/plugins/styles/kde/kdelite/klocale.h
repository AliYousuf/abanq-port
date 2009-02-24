#ifndef __KLOCALE_H__
#define __KLOCALE_H__

#include <qstring.h>
#include <qobject.h>


QString i18n(const char *text)
{
    return QObject::tr(text);
}

QString i18n(const char *index, const char *text)
{
    return QObject::tr(text);
}

#endif