#include <QWidget>
#include <QMenu>
#include <QMenuBar>
#include <QMainWindow>
#include <QAction>
#include <QString>
#include <QTextStream>
#include <QDebug>

#include <QHBoxLayout>
#include <QVBoxLayout>

#include <memory>

#include "mainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
	for(uint8_t i = 0; i < 2; ++i)	m_camWindows_vctr.push_back(new QMainWindow);
	setStyleSheet("QMenu::item::selected{background-color: black; }");
//	LogsHolder::initLogFile();

	auto central = new QWidget;
	setCentralWidget(central);	
//	grid = new QGridLayout(central);
	hbox = new QHBoxLayout(central);
//	grid->setColumnMinimumWidth(0, 0);
	initWidgets(hbox);
	
	createMenus();

	
	udpHolder = new UdpHolder(this);
	usbHolder = new USBHolder(this);
//	connect(this, &MainWindow::updateTelemetry, t, &Telemetry::updateTelemetry);
//	connect(udpHolder, SIGNAL(dataReceived(const std::array<float, 13> , const std::vector<std::vector<bool>> )), t, SLOT(updateTelemetry(const std::array<float, 13> , const std::vector<std::vector<bool>> )));//crazy 
	connect(udpHolder, &UdpHolder::dataReceived, t, &Telemetry::updateTelemetry);
	connect(pl, &PowerLimit::setForce, usbHolder, &USBHolder::setPowerLimit);
	connect(usbHolder, &USBHolder::joystickData, udpHolder, &UdpHolder::setValueInDatagram);
	connect(telemetry_w, &QAction::triggered, this, &MainWindow::showTelemetryWindow);
	connect(m_showUtility_qact_ptr, &QAction::triggered, this, &MainWindow::s_showUtilitySettings);
	connect(t, &Telemetry::sendDepth, alg, &Algorithms::setDepth);
	connect(t, &Telemetry::sendYaw, alg, &Algorithms::setYaw);
	connect(alg, &Algorithms::depthControl, udpHolder, &UdpHolder::setDepthControl);
	connect(alg, &Algorithms::yawControl, udpHolder, &UdpHolder::setYawControl);

	connect(m_recControl_ptr, &RecControl::sig_startRec, m_player, &CamHolder::s_startRec);
	connect(m_recControl_ptr, &RecControl::sig_pauseRec, m_player, &CamHolder::s_pauseRec);
	connect(m_recControl_ptr, &RecControl::sig_stopRec, m_player, &CamHolder::s_stopRec);
	connect(m_player, &CamHolder::sig_gotNewDevice, m_recControl_ptr, &RecControl::s_gotNewDev);
	connect(m_player, &CamHolder::sig_removeItem, m_recControl_ptr, &RecControl::s_removeItem);
	connect(m_player, &CamHolder::sig_resize, this, &MainWindow::s_resize);
	connect(m_player, &CamHolder::sig_stream, this, &MainWindow::s_stream);

	connect(m_recControlSecond_ptr, &RecControl::sig_startRec, m_playerSecond, &CamHolder::s_startRec);//?
	connect(m_recControlSecond_ptr, &RecControl::sig_pauseRec, m_playerSecond, &CamHolder::s_pauseRec);
	connect(m_recControlSecond_ptr, &RecControl::sig_stopRec, m_playerSecond, &CamHolder::s_stopRec);
	connect(m_playerSecond, &CamHolder::sig_gotNewDevice, m_recControlSecond_ptr, &RecControl::s_gotNewDev);
	connect(m_playerSecond, &CamHolder::sig_removeItem, m_recControlSecond_ptr, &RecControl::s_removeItem);
	connect(m_playerSecond, &CamHolder::sig_resize, this, &MainWindow::s_resize);
	connect(m_playerSecond, &CamHolder::sig_stream, this, &MainWindow::s_streamS);

	connect(m_recControl_ptr, &RecControl::sig_connect, m_player, &CamHolder::s_setDev);
	connect(m_recControlSecond_ptr, &RecControl::sig_connect, m_playerSecond, &CamHolder::s_setDev);

	connect(m_burninator_ptr, &BurnInator::sig_setYawPdi, udpHolder, &UdpHolder::s_setYawPdi);
	connect(m_burninator_ptr, &BurnInator::sig_setDepthPdi, udpHolder, &UdpHolder::s_setDepthPdi);
	connect(m_burninator_ptr, &BurnInator::sig_rebootBoard, udpHolder, &UdpHolder::s_rebootBoard);
	connect(m_burninator_ptr, &BurnInator::sig_burnNumbers, udpHolder, &UdpHolder::s_burnNumbers);
	qDebug() << "connections made\n";
//connect(udpHolder, SIGNAL(dataReceived(float)), this, SLOT(udpDataReceived(float)));

//sig_setDepthPdi(std::array<float, 3> &);
//	void sig_setYawPdi(std::array<float, 3> &);
//	void sig_rebootBoard();
//	void sig_burnNumbers();

//	void s_setDepthPdi(std::array<float, 3> &);
//	void s_setYawPdi(std::array<float, 3> &);
//	void s_rebootBoard();
//	void s_burnNumbers();


}
MainWindow::~MainWindow()
{
	delete m_player;
	delete m_playerSecond;
	delete m_recControl_ptr;
	delete m_recControlSecond_ptr;
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
	for(QMainWindow *it : m_camWindows_vctr)	delete it;
}
void MainWindow::s_resize(double width, double height)
{
	if(m_camWindows_vctr.size() != 0)
	{
		for(uint8_t i = 0; i < 2; ++i)	m_camWindows_vctr[i]->resize(width, height);
	}
}
void MainWindow::s_showUtilitySettings()
{
	m_burninator_ptr->show();
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
void MainWindow::initWidgets(QHBoxLayout *grid)
{
	m_vbox_vctr.push_back(new QVBoxLayout);
	m_vbox_vctr.push_back(new QVBoxLayout);
	m_burninator_ptr = new BurnInator;
	t = new Telemetry(this);
	lh = new LogsHolder(this);
	pl = new PowerLimit(this);
	alg = new Algorithms(this);

	m_player = new CamHolder;
	m_recControl_ptr = new RecControl;
	m_playerSecond = new CamHolder;
	m_recControlSecond_ptr = new RecControl;

//	m_player->resize(1000, 1000);
	QWidget *empty = new QWidget(this);
	empty->setMaximumWidth(400);
//	qDebug() << grid->columnMinimumWidth(1) << Qt::endl;
//	t1->setStyleSheet("QWidget{background-color:yellow}");	
	m_vbox_vctr[0]->addWidget(t);//, 0, 0, 1, 1);
	m_vbox_vctr[0]->addWidget(lh);//, 1, 0, 2, 1);
	m_vbox_vctr[0]->addWidget(pl);//, 3, 0, 1, 1);
	m_vbox_vctr[0]->addWidget(alg);//, 4, 0);
	m_vbox_vctr[0]->addWidget(m_recControl_ptr);
	m_vbox_vctr[0]->addWidget(m_recControlSecond_ptr);
	m_vbox_vctr[0]->addWidget(empty);//, 5, 0, 2, 1);
//	CamHolder *cam(m_player);
	QMainWindow *&window = m_camWindows_vctr[0];
	window->resize(m_player->width_d, m_player->height_d);
	window->setCentralWidget(m_player);
	window->show();

	QMainWindow *&windowS = m_camWindows_vctr[1];
	windowS->resize(m_playerSecond->width_d, m_playerSecond->height_d);
	windowS->setCentralWidget(m_playerSecond);
	windowS->show();
//
//	m_vbox_vctr[1]->addWidget(m_player);

	for(int i = 0; i < m_vbox_vctr[0]->count(); ++i)
	{
		QLayoutItem *item = m_vbox_vctr[0]->itemAt(i);
		QWidget *widget;
	//	std::shared_ptr<QLayoutItem> item_ptr(item);
//		std::shared_ptr<QWidget> widget_ptr;
		if(!item)	continue;
		else
		{
			widget = item->widget();
//			widget_ptr(widget);
		}
		if(!widget)	continue;
		else
		{
			widget->setMaximumWidth(410);

			widget->setMinimumWidth(410);
//			widget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
		}
//		delete item;
//		delete widget;
	}
//	grid->addWidget(t2, 1, 0);
//	grid->addWidget(t3, 1, 1);
//	grid->addWidget(t4, 0, 2);
	grid->addLayout(m_vbox_vctr[0]);
	grid->addLayout(m_vbox_vctr[1]);
}
void MainWindow::s_stream()
{
	qDebug() << "called show\n";
	m_camWindows_vctr[0]->show();
}
void MainWindow::s_streamS()
{
	qDebug() << "called show\n";
	m_camWindows_vctr[1]->show();
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

	m_showUtility_qact_ptr = new QAction("Utility", this);

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
	m_utility_qm_ptr = menuBar()->addMenu("Utility");

	m_utility_qm_ptr->addAction(m_showUtility_qact_ptr);
	//add action "show burninator"
	//add m_burninator_ptr object;
	//make qlabes and add them to burninator class
	//add conncections to action and object
	//activate action and show burninator window with m_burninator_ptr->show();
	//test it somehow
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
		
