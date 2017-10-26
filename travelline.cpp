/* This source file is part of the XKSimulator project */

/***************************************************************************
   travelline.cpp
   A simple class that stores data to keep track of distance traveled
   between two points in time and space. In other words, this is used
   to encapsulate a line (including the speed, direction, and time)
   between two points.

   Author: Jesse L. Zamora <xtremekforever@gmail.com>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the
   Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
 ***************************************************************************/
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
