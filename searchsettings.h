#ifndef SEARCHSETTINGS_H
#define SEARCHSETTINGS_H

#include <QMap>
#include <QList>

class searchSettings
{
public:
    searchSettings();

    QMap <QString, QList <quint16> > m_searchTuning;
    int m_timeout, m_baud;
};

#endif // SEARCHSETTINGS_H
