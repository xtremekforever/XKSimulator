#ifndef TRAVELLINE_H
#define TRAVELLINE_H

class TravelLine
{
public:
    TravelLine(int speed, int bearing);

    int speed() { return m_speed; }
    int bearing() { return m_bearing; }

    void start();
    void end();

    int time();
    double distance();
private:
    int m_speed;
    int m_bearing;

    quint64 m_startTime;
    quint64 m_endTime;
};

#endif // SPEEDFORTIME_H
