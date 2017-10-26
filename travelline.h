/* This header is part of the XKSimulator project */

/***************************************************************************
   travelline.h
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
