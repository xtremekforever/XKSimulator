/* This source file is part of the XKSimulator project */

/***************************************************************************
   main.cpp
   The main file of the application containing the main() function.
   This loads the MainInterface class, which in turn loads the main
   window.

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
#include <QApplication>

#include "mainwindow.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName("Zamora Studios");
    QCoreApplication::setApplicationName("XKSimulator");

    MainWindow window;
    window.show();

    return app.exec();
}
