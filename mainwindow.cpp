/* This source file is part of the XKSimulator project */

/***************************************************************************
   mainwindow.cpp
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
#include <QMessageBox>
#include <QSettings>
#include <QTimer>

#include <QtDebug>

#include <qextserialenumerator.h>
#include <qextserialport.h>

#include <nmea/nmea.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "defines.h"
#include "serial.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  setWindowTitle(QString("XKSimulator %1").arg(getVersionString()));

  m_serial = new SerialPort();
  connect(m_serial, SIGNAL(serialError(QString)), this, SLOT(serialError(QString)));
  connect(m_serial, SIGNAL(serialStatus(QString)), this, SLOT(serialStatus(QString)));

  m_error = new QLabel("");
  statusBar()->addWidget(m_error);

  m_enum = new QextSerialEnumerator();
#ifdef Q_OS_WIN
  connect(m_enum, SIGNAL(deviceDiscovered(QextPortInfo)), this, SLOT(deviceDiscovered(QextPortInfo)));
  connect(m_enum, SIGNAL(deviceRemoved(QextPortInfo)), this, SLOT(deviceRemoved(QextPortInfo)));

  m_enum->setUpNotifications();
#endif

  m_gpsTimer = new QTimer(this);
  connect(m_gpsTimer, SIGNAL(timeout()), this, SLOT(gpsTimer()));

  // Load the application settings (not the command settings)
  loadAppSettings();

  // Load the list of available serial ports
  loadComPorts();

  // Load supported baudrates
  loadBaudRates();

  m_serial->open(m_tty);                                        // Open the selected serial port
}

MainWindow::~MainWindow()
{
  saveAppSettings();

  delete m_serial;

  delete ui;
}

void MainWindow::loadAppSettings()
{
  QSettings settings;

#ifdef Q_OS_WIN
  m_tty = settings.value(APPLICATION_SETTINGS_PREFIX + "SerialPort", "COM1").toString();
#else
  m_tty = settings.value(APPLICATION_SETTINGS_PREFIX + "SerialPort", "/dev/ttyS0").toString();
#endif

  // Load last settings
  double lat = settings.value(APPLICATION_SETTINGS_PREFIX + "Latitude", "0.0").toDouble();
  double lon = settings.value(APPLICATION_SETTINGS_PREFIX + "Longitude", "0.0").toDouble();
  int ns = settings.value(APPLICATION_SETTINGS_PREFIX + "NorthSouth", 0).toInt();
  int ew = settings.value(APPLICATION_SETTINGS_PREFIX + "EastWest", 0).toInt();
  int alt = settings.value(APPLICATION_SETTINGS_PREFIX + "Altitude", 0).toInt();

  // Setup the UI
  ui->latitudeSpinBox->setValue(lat);
  ui->longitudeSpinBox->setValue(lon);
  ui->northSouthComboBox->setCurrentIndex(ns);
  ui->eastWestComboBox->setCurrentIndex(ew);
  ui->altitudeSpinBox->setValue(alt);
}

void MainWindow::saveAppSettings()
{
  QSettings settings;

  settings.setValue(APPLICATION_SETTINGS_PREFIX + "SerialPort",
                    m_tty);

  settings.setValue(APPLICATION_SETTINGS_PREFIX + "Latitude",
                    ui->latitudeSpinBox->value());
  settings.setValue(APPLICATION_SETTINGS_PREFIX + "Longitude",
                    ui->longitudeSpinBox->value());
  settings.setValue(APPLICATION_SETTINGS_PREFIX + "NorthSouth",
                    ui->northSouthComboBox->currentIndex());
  settings.setValue(APPLICATION_SETTINGS_PREFIX + "EastWest",
                    ui->eastWestComboBox->currentIndex());
  settings.setValue(APPLICATION_SETTINGS_PREFIX + "Altitude",
                    ui->altitudeSpinBox->value());
}

void MainWindow::loadComPorts()
{
  ui->menuFile->clear();

  m_comPorts = new QActionGroup(this);
  m_comPorts->setExclusive(true);

  QList<QextPortInfo> ports = QextSerialEnumerator::getPorts();

  for (int i = 0; i < ports.size(); i++) {
    QString fullName;
    QString friendName = ports.at(i).friendName;
    QString name = ports.at(i).portName;
    if (name.contains("LPT")) {
      continue;
    }

#ifdef Q_OS_WIN
    name.remove("\\\\.\\");
    fullName = QString("%1").arg(friendName);
#else

#ifdef Q_OS_LINUX
    // Skip all ports ttyS10 - ttyS31
    if (name.size() == 6 && name.contains("ttyS")) {
      continue;
    }

    name.prepend("/dev/");
#endif

    if (friendName.size() > 0) {
      fullName = QString("%1 (%2)").arg(friendName)
                                   .arg(name);
    } else {
      QChar num = name.at(name.size() - 1);

      if (name.contains("ttyS")) {
        fullName = QString("Serial Port %1 (%2)").arg(num).arg(name);
      } else if (name.contains("ttyACM")) {
        fullName = QString("Serial ACM %1 (%2)").arg(num).arg(name);
      } else if (name.contains("ttyUSB")) {
        fullName = QString("Serial USB %1 (%2)").arg(num).arg(name);
      }
    }
#endif

    // Port must have a name to be added
    if (fullName.size() > 0) {
      QAction * com = new QAction(fullName, this);
      com->setToolTip(name);
      com->setCheckable(true);
  
      if (com->toolTip() == m_tty) {
        com->setChecked(true);
      }
  
      m_comPorts->addAction(com);
    }
  }

  connect(m_comPorts, SIGNAL(triggered(QAction*)),
          this, SLOT(comAction_triggered(QAction*)));

  ui->menuFile->addActions(m_comPorts->actions());
  ui->menuFile->addSeparator();
  ui->menuFile->addAction(ui->actionRefresh);
  ui->menuFile->addSeparator();
  ui->menuFile->addAction(ui->actionQuit);
}

void MainWindow::loadBaudRates()
{
  m_baudRates = new QActionGroup(this);
  m_baudRates->setExclusive(true);

  QAction * action;

  action = new QAction("1200", this);
  action->setCheckable(true);
  m_baudRates->addAction(action);

  action = new QAction("2400", this);
  action->setCheckable(true);
  m_baudRates->addAction(action);

  action = new QAction("4800", this);
  action->setCheckable(true);
  m_baudRates->addAction(action);

  action = new QAction("9600", this);
  action->setCheckable(true);
  action->setChecked(true);
  m_baudRates->addAction(action);

  action = new QAction("19200", this);
  action->setCheckable(true);
  m_baudRates->addAction(action);

  action = new QAction("38400", this);
  action->setCheckable(true);
  m_baudRates->addAction(action);

  action = new QAction("57600", this);
  action->setCheckable(true);
  m_baudRates->addAction(action);

  action = new QAction("115200", this);
  action->setCheckable(true);
  m_baudRates->addAction(action);

  connect(m_baudRates, SIGNAL(triggered(QAction*)),
          this, SLOT(baudRateAction_triggered(QAction*)));

  ui->menuBaud->addActions(m_baudRates->actions());
  ui->menuFile->addSeparator();
}

void MainWindow::comAction_triggered(QAction * action)
{
  m_tty = action->toolTip();

  if (m_serial->isOpen()) {
    m_serial->close();
  }

  m_serial->open(m_tty);
}

void MainWindow::on_actionRefresh_triggered()
{
  loadComPorts();
}

void MainWindow::baudRateAction_triggered(QAction * action)
{
  m_serial->setBaud(action->text().toInt());
}

void MainWindow::serialError(QString error)
{
#ifdef Q_OS_UNIX
  if (m_serial->currentError() != E_NO_ERROR) {
    if (m_serial->currentPort().contains("ACM") ||
        m_serial->currentPort().contains("USB")) {
      m_serial->close();
    }
  }
#endif

  m_error->setText(error);
  m_error->setStyleSheet("QLabel { color : red; }");
}

void MainWindow::serialStatus(QString status)
{
  m_error->setText(status);
  m_error->setStyleSheet("QLabel { color : black; }");
}

void MainWindow::on_actionAbout_triggered(bool )
{
  QMessageBox::about(this, "About XKSimulator", getAboutString());
}

void MainWindow::on_actionAboutQt_triggered(bool )
{
  QMessageBox::aboutQt(this, "About Qt");
}

void MainWindow::speedOrBearingUpdated()
{
  
}

void MainWindow::on_speedSpinBox_valueChanged(int i)
{
  // Update the speed
  m_info.speed = i;

  speedOrBearingUpdated();
}

void MainWindow::on_bearingSpinBox_valueChanged(int i)
{
  m_info.direction = i;

  speedOrBearingUpdated();
}

void MainWindow::on_startStopButton_pressed()
{
  if (ui->startStopButton->text() == "Start") {
    startSending();

    // Disable position controls
    ui->positionGroupBox->setEnabled(false);
    // Disable com port selection
    m_comPorts->setEnabled(false);

    ui->startStopButton->setText("Stop");
  } else {
    ui->startStopButton->setText("Start");
    m_gpsTimer->stop();

    // Enable position controls
    ui->positionGroupBox->setEnabled(true);
    // Enable com port selection
    m_comPorts->setEnabled(true);
  }
}

void MainWindow::sendGpsInfo()
{
  char buf[2048];
  int size = nmea_generate(&buf[0], sizeof(buf), &m_info,
                           GPGGA | GPGSA | GPGSV | GPRMC);

  buf[size] = 0;
  qDebug() << buf;

  m_serial->write(QByteArray(buf, size));
}

void MainWindow::gpsTimer()
{
  double lat = nmea_ndeg2degree(m_info.lat);
  double lon = nmea_ndeg2degree(m_info.lon);

  if (lat < 0.0) {
    lat = -lat;
    ui->northSouthComboBox->setCurrentIndex(1);
  } else {
    ui->northSouthComboBox->setCurrentIndex(0);
  }

  if (lon < 0.0) {
    lon = -lon;
    ui->eastWestComboBox->setCurrentIndex(1);
  } else {
    ui->eastWestComboBox->setCurrentIndex(0);
  }

  ui->latitudeSpinBox->setValue(lat);
  ui->longitudeSpinBox->setValue(lon);

  nmea_time_now(&m_info.utc);

  // Finally send the GPS info
  sendGpsInfo();
}

void MainWindow::startSending()
{
  nmea_zero_INFO(&m_info);

  double lat = ui->latitudeSpinBox->value();
  double lon = ui->longitudeSpinBox->value();
  if (ui->northSouthComboBox->currentText() == "S") {
    lat = -lat;
  }

  if (ui->eastWestComboBox->currentText() == "W") {
    lon = -lon;
  }

  // Get current time
  nmea_time_now(&m_info.utc);

  m_info.sig          = 3;
  m_info.fix          = ui->gpsFixComboBox->currentIndex() + 1;
  m_info.lat          = nmea_degree2ndeg(lat);
  m_info.lon          = nmea_degree2ndeg(lon);
  m_info.speed        = ui->speedSpinBox->value();
  m_info.direction    = ui->bearingSpinBox->value();
  m_info.elv          = ui->altitudeSpinBox->value();

  m_info.satinfo.inuse = 8;
  m_info.satinfo.inview = 12;

  // Start the GPS timer
  m_gpsTimer->start(1000);
}
