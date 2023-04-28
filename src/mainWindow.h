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
#include "burnInator.h"

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
	QMenu *m_utility_qm_ptr;
		QAction *m_showUtility_qact_ptr;
		BurnInator *m_burninator_ptr;
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
	CamHolder *m_playerSecond;
	RecControl *m_recControlSecond_ptr;

	std::vector<QMainWindow *> m_camWindows_vctr;
	
	void createMenus();
	void initActions();
	void initWidgetsMenu();
	void initWidgets(QHBoxLayout *);
private slots:
	void showTelemetryWindow();
	void udpDataReceived(float roll);
	void s_showUtilitySettings();
	void s_resize(double, double);
	void s_stream();
	void s_streamS();
};
#endif
