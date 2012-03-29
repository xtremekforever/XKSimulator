
#include <QMessageBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "defines.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle(QString("XKSimulator %1").arg(getVersionString()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionAbout_triggered(bool )
{
    QMessageBox::about(this, "About XKSimulator", getAboutString());
}

void MainWindow::on_actionAboutQt_triggered(bool )
{
    QMessageBox::aboutQt(this, "About Qt");
}
