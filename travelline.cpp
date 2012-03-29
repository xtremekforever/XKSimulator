#include <QDateTime>

#include "travelline.h"

TravelLine::TravelLine(int speed, int bearing)
    : m_speed(speed),
      m_bearing(bearing),
      m_startTime(0),
      m_endTime(0)
{
}

void TravelLine::start()
{
    m_startTime = QDateTime::currentMSecsSinceEpoch();
}

void TravelLine::end()
{
    m_endTime = QDateTime::currentMSecsSinceEpoch();
}

int TravelLine::time()
{
    if (m_endTime == 0) {
        return QDateTime::currentMSecsSinceEpoch() - m_startTime;
    } else {
        return m_endTime - m_startTime;
    }
}

double TravelLine::distance()
{
    double meters_second = (m_speed * 1000.0) / 3600;
    double time_seconds = time() / 1000.0;
    return (meters_second * time_seconds);
}
