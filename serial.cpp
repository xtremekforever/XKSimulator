/* This source file is part of the XKSimulator project */

/***************************************************************************
   serial.cpp
   An interface class to communicate with the serial port. Based off
   of Alexander Neundorf's CuteCom source code.

   Author: Alexander Neundorf <neundorf@kde.org>, modified for
           XKSimulator by Jesse L. Zamora <xtremekforever@gmail.com>

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
#include <QSocketNotifier>
#include <QtDebug>
#include <QTimer>
#include <QFileInfo>
#include <QMessageBox>

#include <iostream>
#include <unistd.h>
#include <ctype.h>
#include <sys/time.h>
#include <sys/types.h>
#include <fcntl.h>

#include <qextserialport.h>

#include "serial.h"


bool exists(QString & tty)
{
  QFileInfo fi(tty);
  return fi.exists();
}

SerialPort::SerialPort()
{
  m_cmd = 0;
  m_serial_mode = SERIAL_MODE_TEXT;
  m_port = new QextSerialPort();
  connect(m_port, SIGNAL(readyRead()), this, SLOT(read()));
  connect(m_port, SIGNAL(dsrChanged(bool )), this, SLOT(dsrChanged(bool )));
}

SerialPort::~SerialPort()
{
  m_port->close();

  delete m_port;
}

bool SerialPort::isOpen()
{
  return m_port->isOpen();
}

void SerialPort::close()
{
  m_port->flush();
  m_port->close();
}

void SerialPort::open(QString & tty)
{
  m_tty = tty;

#ifdef Q_WS_X11
  if (!exists(tty)) {
    serialError(QString("%1 does not exist").arg(tty));
    return;
  }
#endif

  if (m_port->isOpen()) {
    qDebug() << "Port" << tty << "is already open!";
    return;
  }

  m_port->setPortName(tty);
  if (m_port->open(QIODevice::ReadWrite)) {
    m_port->setBaudRate(BAUD9600);
    m_port->setDataBits(DATA_8);
    m_port->setParity(PAR_NONE);
    m_port->setStopBits(STOP_1);
    m_port->setFlowControl(FLOW_OFF);
    m_port->setTimeout(10);
    m_port->setRts(true);
    m_port->setDtr(true);

    serialStatus(QString("%1 is open").arg(tty));
  } else {
    serialError(QString("Error opening %1").arg(tty));
    qDebug() << "Error opening " << tty;
  }

  detectError();
}

void SerialPort::write(QByteArray data)
{
  m_port->write(data, data.length());

  detectError();
}

void SerialPort::writeByte(unsigned char c)
{
  QByteArray byte;
  byte.append(c);

  m_port->write(byte, byte.length());

  detectError();
}

bool SerialPort::detectError()
{
  int error = m_port->lastError();

  if (error == E_NO_ERROR) {
    return false;
  } else {

    qDebug() << "Error" << error << "on port" << m_port->portName();

    switch (error) {
    case E_INVALID_FD:

      break;
    case E_NO_MEMORY:

      break;
    case E_CAUGHT_NON_BLOCKED_SIGNAL:

      break;
    case E_PORT_TIMEOUT:

      break;
    case E_INVALID_DEVICE:

      break;
    case E_BREAK_CONDITION:


      break;
    case E_FRAMING_ERROR:


      break;
    case E_IO_ERROR:


      break;
    case E_BUFFER_OVERRUN:



      break;
    case E_RECEIVE_OVERFLOW:


      break;
    case E_RECEIVE_PARITY_ERROR:


      break;
    case E_TRANSMIT_OVERFLOW:


      break;
    case E_READ_FAILED:


      break;
    case E_WRITE_FAILED:
      serialError(QString("Write failed to port %1").arg(m_port->portName()));
      break;

    case E_FILE_NOT_FOUND:


      break;
    }

    m_port->lastErrorClear();
  }

  return true;
}

int SerialPort::currentError()
{
  return m_port->lastError();
}

void SerialPort::dsrChanged(bool status)
{
#ifdef XKSIMULATOR_DEBUG
  QDateTime time = QDateTime::currentDateTime();
  qDebug() << time.toString("yyyy/MM/dd hh:mm:ss") << ":" << m_port->portName() << "DSR" << (int)status;
#endif
}

void SerialPort::read()
{
  QString error = QString("Error reading from %1").arg(m_tty);

  if (!m_port->isOpen()) {
    serialError(error);
    return;
  }

  int bytes_read = 0;
  bytes_read = m_port->read(m_buf, SERIAL_BUFSIZE);

  if (detectError()) return;

  if (bytes_read > 0) {
    m_buf[bytes_read] = '\0';
  } else if (bytes_read < 0) {
    serialError(error);
    return;
  } else {
    return;
  }

  char * c = m_buf;

  for (int i = 0; i < bytes_read; i++)
  {
    if (m_serial_mode == SERIAL_MODE_BINARY) {
      m_cmdBuffer.append(m_buf[i]);
      if (m_cmdBuffer.size() > 512) {
        readCmd(m_cmdBuffer);
        m_cmdBuffer.clear();
      }
    } else {
      if (*c == 0x28 && m_outputBuffer.length() == 0) {
        m_cmd = 1;
      }

      if (m_cmd == 1) {
        m_cmdBuffer.append(*c);
        if (*c == 0x29) {
          // Send read command signal
          readCmd(m_cmdBuffer);
          m_cmdBuffer.clear();
          m_cmd = 0;
        }
      } else {
        // also print a newline for \r, and print only one newline for \r\n
        if ((isprint(*c)) || (*c=='\n') || *c == '\t') {
          if (*c == '\n') {

            // Send a read line signal
            readLine(m_outputBuffer);
            m_outputBuffer.clear();
          } else if (*c != '\r') {
            m_outputBuffer += (*c);
          }
        }
      }

      c++;
    }
  }
}
