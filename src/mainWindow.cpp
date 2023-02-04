#pragma once
#include <QWidget>
#include <QMenu>
#include <QMenuBar>
#include <QMainWindow>
#include <QAction>
#include <QString>
#include <QTextStream>

#include "mainWindow.h"
#include "telemetry.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
	setStyleSheet("QMenu::item::selected{background-color: black; }");
	auto central = new QWidget;
	grid = new QGridLayout(central);
	initWidgets(grid);
	createMenus();
	setCentralWidget(central);
	connect(telemetry_w, &QAction::triggered, this, &MainWindow::showTelemetryWindow);
}
void MainWindow::showTelemetryWindow()
{
	QTextStream out(stdout);
//	Telemetry wind;
	t_wndw = new Telemetry;
	t_wndw->setWindowTitle("Telemetry");
	t_wndw->show();
	out << "Telemetry window created;" << Qt::endl;
		
}
void MainWindow::initWidgets(QGridLayout *grid)
{
	t = new Telemetry(this);
	Telemetry *t1 = new Telemetry(this);
	Telemetry *t2 = new Telemetry(this);
	Telemetry *t3 = new Telemetry(this);
	Telemetry *t4 = new Telemetry(this);
	QWidget *empty = new QWidget(this);
//	t1->setStyleSheet("QWidget{background-color:yellow}");	
	grid->addWidget(t, 0, 0, 1, 1);
	grid->addWidget(empty, 1, 0, 5, 1);
//	grid->addWidget(t2, 1, 0);
//	grid->addWidget(t3, 1, 1);
//	grid->addWidget(t4, 0, 2);

}
void MainWindow::initActions()
{
	calibrateDrivers = new QAction("Calibrate Drivers", this);
	startRec = new QAction("Start recording", this);
	stopRec = new QAction("Stop recording", this);
	settings = new QAction("Settings", this);

	telemetry_w = new QAction("Telemetry", this);
	logs_w = new QAction("Logs", this);
	forceLimits_w = new QAction("Force Limit", this);
	algorithms_w = new QAction("Algorithms", this);
	data_w = new QAction("Data", this);

}
void MainWindow::initWidgetsMenu()
{
	telemetry = new QAction("Telemetry", this);
	logs = new QAction("Logs", this);
	forceLimits = new QAction("Force Limit", this);
	algorithms = new QAction("Algorithms", this);
	data = new QAction("Data", this);
	checkBoxes.push_back(telemetry);
	checkBoxes.push_back(logs);
	checkBoxes.push_back(forceLimits);
	checkBoxes.push_back(algorithms);
	checkBoxes.push_back(data);
	for(int i = 0; i < checkBoxes.size(); ++i)
	{
		checkBoxes[i]->setCheckable(1);
		checkBoxes[i]->setChecked(1);
	}
	
}
void MainWindow::createMenus()
{
	initWidgetsMenu();
	initActions();
	devices = menuBar()->addMenu("Devices");
	windows = menuBar()->addMenu("Windows");

	windows->addAction(telemetry_w);
	windows->addAction(logs_w);
	windows->addAction(forceLimits_w);
	windows->addAction(algorithms_w);
	windows->addAction(data_w);
	

	widgets = menuBar()->addMenu("Widgets");
	for(int i = 0; i < checkBoxes.size(); ++i)
	{
		widgets->addAction(checkBoxes[i]);
	}

	hardware = menuBar()->addMenu("Hardware");
	hardware->addAction(calibrateDrivers);

	cameras = devices->addMenu("Cameras");
	cameras->addAction(startRec);
	cameras->addAction(stopRec);

	controllers = devices->addMenu("Controllers");
	controllers->addAction(settings);
}
		
