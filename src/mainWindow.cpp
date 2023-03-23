#include <QWidget>
#include <QMenu>
#include <QMenuBar>
#include <QMainWindow>
#include <QAction>
#include <QString>
#include <QTextStream>

#include "mainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
	setStyleSheet("QMenu::item::selected{background-color: black; }");
//	LogsHolder::initLogFile();

	auto central = new QWidget;
	grid = new QGridLayout(central);
	initWidgets(grid);
	
	createMenus();
	setCentralWidget(central);
	
	udpHolder = new UdpHolder(this);
	usbHolder = new USBHolder(this);
//	connect(this, &MainWindow::updateTelemetry, t, &Telemetry::updateTelemetry);
//	connect(udpHolder, SIGNAL(dataReceived(const std::array<float, 13> , const std::vector<std::vector<bool>> )), t, SLOT(updateTelemetry(const std::array<float, 13> , const std::vector<std::vector<bool>> )));//crazy 
	connect(udpHolder, &UdpHolder::dataReceived, t, &Telemetry::updateTelemetry);
	connect(pl, &PowerLimit::setForce, usbHolder, &USBHolder::setPowerLimit);
	connect(usbHolder, &USBHolder::joystickData, udpHolder, &UdpHolder::setValueInDatagram);
	connect(telemetry_w, &QAction::triggered, this, &MainWindow::showTelemetryWindow);
	connect(t, &Telemetry::sendDepth, alg, &Algorithms::setDepth);
	connect(t, &Telemetry::sendYaw, alg, &Algorithms::setYaw);
	connect(alg, &Algorithms::depthControl, udpHolder, &UdpHolder::setDepthControl);
	connect(alg, &Algorithms::yawControl, udpHolder, &UdpHolder::setYawControl);
//	connect(udpHolder, SIGNAL(dataReceived(float)), this, SLOT(udpDataReceived(float)));


}
MainWindow::~MainWindow()
{
	delete hardware;
	delete devices;
	delete cameras;
	delete startRec;
	delete stopRec;
	delete controllers;
	delete settings;
	delete drivers;
	delete calibrateDrivers;
	delete widgets;
	delete telemetry;
	delete logs;
	delete forceLimits;
	delete algorithms;
	delete data;
	delete windows;
	delete telemetry_w;
	delete logs_w;
	delete forceLimits_w;
	delete algorithms_w;
	delete data_w;	
	delete grid;
	delete t;
	delete t_wndw;
	delete lh;
	delete lh_wndw;
	delete pl;
	delete alg;
	delete usbHolder;
	delete udpHolder;
}
void MainWindow::udpDataReceived(float roll)
{
	qDebug() << roll << Qt::endl;
}
void MainWindow::showTelemetryWindow()
{
	QTextStream out(stdout);
//	Telemetry wind;
	t_wndw = new Telemetry(this);
	t_wndw->setWindowTitle("Telemetry");
	t_wndw->resize(200, 200); 
	t_wndw->show();
	out << "Telemetry window created;" << Qt::endl;
	lh->writeInLog("Telemetry window shown");
		
}
void MainWindow::initWidgets(QGridLayout *grid)
{
	t = new Telemetry(this);
	lh = new LogsHolder(this);
	pl = new PowerLimit(this);
	alg = new Algorithms(this);	

	QWidget *empty = new QWidget(this);
//	t1->setStyleSheet("QWidget{background-color:yellow}");	
	grid->addWidget(t, 0, 0, 1, 1);
	grid->addWidget(lh, 1, 0, 2, 1);
	grid->addWidget(pl, 3, 0, 1, 1);
	grid->addWidget(alg, 4, 0);
	grid->addWidget(empty, 5, 0, 2, 1);
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
	lh->writeInLog("Some actions initiated");

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
	lh->writeInLog("widgets menu initiated");
	
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
	lh->writeInLog("Menus created");
}
		
