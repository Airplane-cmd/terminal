#pragma once
#include <QWidget>
//#include <QStringStream>
#include <QKeyEvent>
#include <QAction>
#include <QToolBar>
#include <QMenu>
#include <QGridLayout>
#include <QMainWindow>
#include <vector>
#include <QString>

#include "telemetry.h"
#include "logs.h"
class MainWindow : public QMainWindow
{
Q_OBJECT
friend class LogsHolder;
public:
	MainWindow(QWidget *parent = 0);
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
	Telemetry *t;
	Telemetry *t_wndw;

	LogsHolder *lh;
	LogsHolder *lh_wndw;

	bool gotLogInfo;
	static QString curLog;

	void createMenus();
	void initActions();
	void initWidgetsMenu();
	void initWidgets(QGridLayout *);
private slots:
	void showTelemetryWindow();
};
