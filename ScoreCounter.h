#pragma once
#include <QObject>

class ScoreCounter : public QObject
{
    Q_OBJECT
public:
    inline ScoreCounter(QObject *parent = 0)
        : QObject(parent)
    {
        m_mark[0] = 0;
        m_mark[1] = 0;
    }
    
    inline void increase(int owner)
    {
        if (owner > -1 && owner < 2)
        {
            ++m_mark[owner];
            emit statusUpdated(QString("First: %1 - Second: %2").arg(QString::number(m_mark[0]), QString::number(m_mark[1])));
        }
    }

private:
    int m_mark[2];
    
signals:
    void statusUpdated(const QString&);
};