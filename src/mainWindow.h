#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QWidget>
//#include <QStringStream>
#include <QKeyEvent>
#include <QAction>
#include <QToolBar>
#include <QMenu>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QMainWindow>
#include <vector>
#include <QString>

#include "telemetry.h"
#include "logs.h"
#include "powerLimit.h"
#include "algorithms.h"
#include "udpHolder.h"
#include "usbHolder.h"
#include "camHolder.h"
#include "recControl.h"

class MainWindow : public QMainWindow
{
Q_OBJECT
friend class LogsHolder;
public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();
protected:
//	void timerEvent(QTimerEvent *);
//	void keyPressEvent(QKeyEvent *);
private:	
	QMenu *hardware;

	QMenu *devices;
		QMenu *cameras;
			QAction *startRec;
			QAction *stopRec;
		QMenu *controllers;
			QAction *settings;
	QMenu *drivers;
		QAction *calibrateDrivers;

	QMenu *widgets;
		QAction *telemetry;
		QAction *logs;
		QAction *forceLimits;
		QAction *algorithms;
		QAction *data;
	QMenu *windows;
		QAction *telemetry_w;
		QAction *logs_w;
		QAction *forceLimits_w;
		QAction *algorithms_w;
		QAction *data_w;

	std::vector<QAction *> checkBoxes;
	
	QGridLayout *grid;
	QHBoxLayout *hbox;
	std::vector<QVBoxLayout *> m_vbox_vctr;
	Telemetry *t;
	Telemetry *t_wndw;

	LogsHolder *lh;
	LogsHolder *lh_wndw;

	PowerLimit *pl;

	Algorithms *alg;

	bool gotLogInfo;
	static QString curLog;
	
	UdpHolder *udpHolder;
	USBHolder *usbHolder;
	CamHolder *m_player;
	RecControl *m_recControl_ptr;

	void createMenus();
	void initActions();
	void initWidgetsMenu();
	void initWidgets(QHBoxLayout *);
private slots:
	void showTelemetryWindow();
	void udpDataReceived(float roll);
};
#endif
