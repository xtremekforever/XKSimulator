/* This header is part of the XKSimulator project */

/***************************************************************************
   mainwindow.h
   The main window class for the project.

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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <nmea/info.h>

namespace Ui {
  class MainWindow;
}

class QActionGroup;
class QLabel;
class QTimer;

class QextSerialEnumerator;

class SerialPort;

class TravelLine;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

protected:
  void loadComPorts();

  void loadAppSettings();
  void saveAppSettings();

  void startSending();

  void sendGpsInfo();
  void speedOrBearingUpdated();

private slots:
  void on_actionAbout_triggered(bool checked = false);
  void on_actionAboutQt_triggered(bool checked = false);

  void comAction_triggered(QAction * action);
  void on_actionRefresh_triggered();

  void on_speedSpinBox_valueChanged(int i);
  void on_bearingSpinBox_valueChanged(int i);

  void on_startStopButton_pressed();

  void serialError(QString error);
  void serialStatus(QString status);

  void gpsTimer();

private:
  Ui::MainWindow *ui;

  QLabel          *m_error;

  QString         m_tty;

  SerialPort      *m_serial;
  QActionGroup    *m_comPorts;

  QextSerialEnumerator * m_enum;

  // GPS Stuff
  QTimer * m_gpsTimer;
  nmeaINFO m_info;

  QList<TravelLine> m_lines;
};

#endif // MAINWINDOW_H
