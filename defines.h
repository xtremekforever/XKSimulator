/* This header is part of the XKSimulator project */

/***************************************************************************
   defines.h
   All the default project definitions.

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
#ifndef DEFINES_H
#define DEFINES_H

#define APPLICATION_SETTINGS_PREFIX     QString("/Application/")

#define QEXTSERIALPORT_URL      QString("http://code.google.com/p/qextserialport/")
#define OXYGEN_PROJECT_URL      QString("http://www.oxygen-icons.org/")
#define NMEALIB_PROJECT_URL     QString("http://nmea.sourceforge.net/")

#include "version.h"

static QString getVersionString()
{
  return QString("%1").arg(VERSION_STR);
}

static QString getAboutString()
{
  QString about = QString("<b>XKSimulator %1</b><br/>").arg(getVersionString());


  about += "<br/>XKSimulator is a simple GPS simulator written in Qt4.<br/>";
  about += "It simulates a GPS device by sending NMEA 0183 strings to a<br/>";
  about += "USB serial emulator or a serial port.<br/>";

  about += "<br/>Special thanks to the <a href=\"" + QEXTSERIALPORT_URL +
            "\">QextSerialPort</a> project for their cross-<br/>";
  about += "platform serial port libraries.<br/>";

  about += "<br/>Also, kudos to the <a href=\"" + OXYGEN_PROJECT_URL +
            "\">Oxygen</a> project for their beautiful icons!<br/>";

  about += "<br/>And, last but not least, many thanks to Tim (xtimor@gmail.com)<br/>";
  about += "for his <a href=\"" + NMEALIB_PROJECT_URL + "\">NMEA Library</a> ";
  about += "for generating NMEA 0183 strings.<br/>";

  about += "<br/>Author:<br/>";
  about += "&nbsp;&nbsp;Jesse L. Zamora<br/>";
  about += "&nbsp;&nbsp;xtremekforever@gmail.com<br/>";

  about += "<br/>Copyright (C) 2017 XKSimulator<br/>";

  return about;
}

#endif
