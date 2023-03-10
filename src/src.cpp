#include <QApplication>
#include <QSize>
#include <QScreen>
#include "mainWindow.h"
#include "telemetry.h"
#include "logs.h"
#include "powerLimit.h"
//#include "udpHolder.h"
int main(int argc, char **argv)
{

	QApplication app(argc, argv);
	QScreen *scr = QGuiApplication::primaryScreen();
//	UdpHolder holder;
	MainWindow wndw;
	wndw.resize(scr->geometry().width(), scr->geometry().height());
	wndw.show();
//	PowerLimit wndw1;
//	wndw1.show();
//	LogsHolder::initLogFile();
//	LogsHolder wndw1;
//	wndw1.show();
//	Telemetry wndw1;
//	wndw1.show();
	return app.exec();
}
