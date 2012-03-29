/* This header is part of the XKManager project */

/***************************************************************************
   serial.h
   An interface class to communicate with the serial port. Based off
   of Alexander Neundorf's CuteCom source code.

   Author: Alexander Neundorf <neundorf@kde.org>, modified for
           XKManager by Jesse L. Zamora <xtremekforever@gmail.com>

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
#ifndef SERIAL_H
#define SERIAL_H

#include <QObject>
#include <QDateTime>

class QTimer;
class QString;
class QSocketNotifier;
class QextSerialPort;

#define SERIAL_BUFSIZE 4096               // The size of one line just about

#define SERIAL_MODE_TEXT    1
#define SERIAL_MODE_BINARY  2

class SerialPort : public QObject
{
    Q_OBJECT
    
public:
    SerialPort();
    ~SerialPort();

    bool isOpen();
    void close();
    void open(QString & tty);

    QString currentPort() { return m_tty; }
                       
    void write(QByteArray data);
    void writeByte(unsigned char c);

    void setMode(int mode) { m_serial_mode = mode; }

    bool detectError();

    int currentError();

    void run();
public slots:
    void read();
    void dsrChanged(bool);

signals:
    void readLine(QString data);
    void readCmd(QByteArray& data);

    void serialError(QString errorStr);
    void serialStatus(QString status);

private:
    QString m_outputBuffer;
    QByteArray m_cmdBuffer;
    
    QString m_tty;
    char m_buf[SERIAL_BUFSIZE + 1];
    int m_cmd;

    int m_serial_mode;

    QextSerialPort * m_port;
};

#endif
