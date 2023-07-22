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
	m_thrustersNC_qbarr.resize(4);
	m_camerasNC_qbarr.resize(4);
	initWidgets(hbox);
	
	createMenus();

	
	udpHolder = new UdpHolder(this);
	usbHolder = new USBHolder(this);
	connect(udpHolder, &UdpHolder::dataReceived, t, &Telemetry::updateTelemetry);
	connect(pl, &PowerLimit::setForce, usbHolder, &USBHolder::setPowerLimit);
	connect(usbHolder, &USBHolder::joystickData, udpHolder, &UdpHolder::setValueInDatagram);
	connect(usbHolder, &USBHolder::sig_setPowerLimit, pl, &PowerLimit::s_setSliderValue);
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

	connect(m_nativeControl_bttn.get(), &QPushButton::clicked, this, &MainWindow::s_nativeControl);
	connect(this, &MainWindow::sig_keyPressed, this, &MainWindow::s_nativeControlOnPressed);
	connect(this, &MainWindow::sig_keyReleased, this, &MainWindow::s_nativeControlOnReleased);
	connect(this, &MainWindow::sig_camerasPositionChanged, usbHolder, &USBHolder::s_setCamerasPositions);
	connect(usbHolder, &USBHolder::sig_camerasPositions, this, &MainWindow::s_setCamerasPositions);
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
void MainWindow::keyPressEvent(QKeyEvent *event)
{
	emit sig_keyPressed(event);
}
void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
	emit sig_keyReleased(event);
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
	m_nativeControl_bttn = std::make_shared<QPushButton>("Native control", this);
	m_nativeControl_bttn->setCheckable(1);
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
	m_vbox_vctr[0]->addWidget(m_nativeControl_bttn.get());
	m_vbox_vctr[0]->addWidget(empty);//, 5, 0, 2, 1);
	
//	CamHolder *cam(m_player);//cringe	
	QMainWindow *&window = m_camWindows_vctr[0];
	window->resize(m_player->width_d, m_player->height_d);
	window->setCentralWidget(m_player);
//	window->show();

	QMainWindow *&windowS = m_camWindows_vctr[1];
	windowS->resize(m_playerSecond->width_d, m_playerSecond->height_d);
	windowS->setCentralWidget(m_playerSecond);
//	windowS->show();
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
//	qDebug() << "called show\n";
	m_camWindows_vctr[0]->show();
}
void MainWindow::s_streamS()
{
//	qDebug() << "called show\n";
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
void MainWindow::s_nativeControl()
{
	if(m_nativeControl_bttn->isChecked())
	{
		connect(this, &MainWindow::sig_thrustersQba, udpHolder, &UdpHolder::setValueInDatagram); 
		connect(this, &MainWindow::sig_camerasQba, udpHolder, &UdpHolder::setValueInDatagram); 
		qDebug() << "connections of native control system created\n";
	}
	else
	{
		disconnect(this, &MainWindow::sig_thrustersQba, udpHolder, &UdpHolder::setValueInDatagram); 
		disconnect(this, &MainWindow::sig_camerasQba, udpHolder, &UdpHolder::setValueInDatagram); 
		qDebug() << "connection of native control system destroyed\n";
	}
}
void MainWindow::s_nativeControlOnPressed(QKeyEvent *event)
{
	int8_t value;
	uint8_t index;
	bool emitThrustersSignal_f = 1;
	bool emitCamerasSignal_f = 1;
	bool autorepeat_f = 0;
	std::array<int, 4> autorepeatingKeys = {75, 76, 59, 39};
	if(event->isAutoRepeat())
	{
		for(uint8_t i = 0; i < 4; ++i)	if(event->key() == autorepeatingKeys[i])	autorepeat_f = 1;
		if(!autorepeat_f)	return;
	}
	switch(event->key())
	{
		case(87):
			index = 0;
			value = -1;
			break;
		case(65):
			index = 1;
			value = 1;
			break;
		case(83):
			index = 0;
			value = 1;
			break;
		case(68):
			index = 1;
			value = -1;
			break;
		case(81):
			index = 2;
			value = 1;
			break;
		case(69):
			index = 2;
			value = -1;
			break;
		case(16777248):
			index = 3;
			value = 1;
			break;
		case(16777249):
			index = 3;
			value = -1;
			break;
		default:
			emitThrustersSignal_f = 0;
	}
	qDebug() << "key pressed" << event->key() << '\n';//db
	if(emitThrustersSignal_f)
	{
		m_thrustersNC_qbarr[index] = value * pl->getValue();	
		qDebug() << "key pressed" << event->key() << ", value is:" << value * pl->getValue() << "\n";
		emit sig_thrustersQba(m_thrustersNC_qbarr, 2);
		if(m_nativeControl_bttn->isChecked())	qDebug() << "thrusters signal was emited\n";//db
	}
//	value = 0;
	switch(event->key())
	{
		case(73):
			index = 2;
			value = 100;
			break;
		case(79):
			index = 2;
			value = -100;
			break;
		case(80):
			index = 3;
			value = 100;
			break;
		case(91):
			index = 3;
			value = -100;
			break;
		case(75):
			index = 0;
			if(m_camerasPositions_arr[index] > -100)
			{
				if(m_nativeControl_bttn->isChecked())	m_camerasPositions_arr[index] -= 1;
				value = m_camerasPositions_arr[index];
//				emit sig_camerasPositionChanged(m_camerasPositions_arr);
			}
			break;
		case(76):
			index = 0;
			if(m_camerasPositions_arr[index] < 100)
			{
				if(m_nativeControl_bttn->isChecked())	m_camerasPositions_arr[index] += 1;
				value = m_camerasPositions_arr[index];
//				emit sig_camerasPositionChanged(m_camerasPositions_arr);

			}
			break;
		case(59):
			index = 1;
			if(m_camerasPositions_arr[index] > -100)
			{
				if(m_nativeControl_bttn->isChecked())	m_camerasPositions_arr[index] -= 1;
				value = m_camerasPositions_arr[index];
//				emit sig_camerasPositionChanged(m_camerasPositions_arr);
			}
			break;
		case(39):
			index = 1;
			if(m_camerasPositions_arr[index] < 100)
			{
				if(m_nativeControl_bttn->isChecked())	m_camerasPositions_arr[index] += 1;
				value = m_camerasPositions_arr[index];
//				emit sig_camerasPositionChanged(m_camerasPositions_arr);
			}
			break;
		default:
			emitCamerasSignal_f = 0;
	}
	if(emitCamerasSignal_f)
	{
//		m_camerasNC_qbarr[index] = (autorepeat_f) ? value : value * pl->getValue();
		m_camerasNC_qbarr[index] = value;	
		emit sig_camerasPositionChanged(m_camerasPositions_arr);
		qDebug() << "value is:" << value << "\n";
		emit sig_camerasQba(m_camerasNC_qbarr, 29);
		if(m_nativeControl_bttn->isChecked())	qDebug() << "cameras signal was emited\n";//db
//		value = 0;

	}


}
void MainWindow::s_nativeControlOnReleased(QKeyEvent *event)
{
	uint8_t index;
	int8_t value;
	bool emitThrustersSignal_f = 1;
	bool emitCamerasSignal_f = 1;
	bool autorepeat_f = 0;
	std::array<int, 4> autorepeatingKeys = {75, 76, 59, 39};
	if(event->isAutoRepeat())//?
	{
		for(uint8_t i = 0; i < 4; ++i)	if(event->key() == autorepeatingKeys[i])	autorepeat_f = 1;
		if(!autorepeat_f)	return;
	}
	qDebug() << "key relesaed" << event->key() << '\n';
	switch(event->key())
	{
		case(87):
			index = 0;
			break;
		case(65):
			index = 1;
			break;
		case(83):
			index = 0;
			break;
		case(68):
			index = 1;
			break;
		case(81):
			index = 2;
			break;
		case(69):
			index = 2;
			break;
		case(16777248):
			index = 3;
			break;
		case(16777249):
			index = 3;
			break;
		default:
			emitThrustersSignal_f = 0;
			
	}
	if(emitThrustersSignal_f)
	{
		m_thrustersNC_qbarr[index] = 0;
		emit sig_thrustersQba(m_thrustersNC_qbarr, 2);
		if(m_nativeControl_bttn->isChecked()) qDebug() << "thrusters release signal was emited\n";//db
	}
	switch(event->key())
	{
		case(73):
			index = 2;
			value = 0;
			break;
		case(79):
			index = 2;
			value = 0;
			break;
		case(80):
			index = 3;
			value = 0;
			break;
		case(91):
			index = 3;
			value = 0;
			break;
		case(75):
			index = 0;
//			if(m_camerasPositions_arr[index] > -100)
//			{
				value = m_camerasPositions_arr[index];
//			}
			break;
		case(76):
			index = 0;
//			if(m_camerasPositions_arr[index] < 100)
//			{
				value = m_camerasPositions_arr[index];
//			}
			break;
		case(59):
			index = 1;
//			if(m_camerasPositions_arr[index] > -100)
//			{
				value = m_camerasPositions_arr[index];
//			}
			break;
		case(39):
			index = 1;
//			if(m_camerasPositions_arr[index] < 100)
//			{
				value = m_camerasPositions_arr[index];
//			}
			
			break;
		default:
			emitCamerasSignal_f = 0;
			
	}
	if(emitCamerasSignal_f)
	{
		m_camerasNC_qbarr[index] = value;
		if(autorepeat_f)	qDebug() << "camera position: " << value;
		emit sig_camerasQba(m_camerasNC_qbarr, 29);
		if(m_nativeControl_bttn->isChecked())	qDebug() << "cameras release signal was emmited\n";//db
	}
}
void MainWindow::s_setCamerasPositions(const std::array<int8_t, 2> &camerasPositions)
{
	for(uint8_t i = 0; i < 2; ++i) m_camerasPositions_arr[i] = camerasPositions[i];
}
