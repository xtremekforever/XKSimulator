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

#define QEXTSERIALPORT_URL      QString("http://code.google.com/p/qextserialport/")
#define OXYGEN_PROJECT_URL      QString("http://www.oxygen-icons.org/")

#include "version.h"

static QString getVersionString()
{
    return QString("%1").arg(VERSION_STR);
}

static QString getAboutString()
{
    QString about = QString("<b>XKManager %1</b><br/>").arg(getVersionString());


    about += "<br/>XKSimulator is a Qt4 application to simulate<br/>";
    about += "a GPS device by sending NMEA 0183 strings to a USB<br/>";
    about += "serial emulator or serial port.<br/>";

    about += "<br/>Special thanks to the <a href=\"" + QEXTSERIALPORT_URL +
             "\">QextSerialPort</a> project<br/>";
    about += "for their cross-platform serial port libraries.<br/>";

    about += "<br/>Also, kudos to the <a href=\"" + OXYGEN_PROJECT_URL +
             "\">Oxygen</a> project for their<br/>";
    about += "beautiful icons!<br/>";

    about += "<br/>Author:<br/>";
    about += "&nbsp;&nbsp;Jesse L. Zamora<br/>";
    about += "&nbsp;&nbsp;xtremekforever@gmail.com<br/>";

    about += "<br/>Copyright (C) 2012 Zamora Studios<br/>";

    return about;
}

#endif
