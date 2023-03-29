#include <QApplication>
#include <QSize>
#include <QScreen>
#include "mainWindow.h"
#include "telemetry.h"
#include "logs.h"
#include "powerLimit.h"
#include "spinLayout.h"
#include "camHolder.h"
//#include "udpHolder.h"
int main(int argc, char **argv)
{

	QApplication app(argc, argv);
	QScreen *scr = QGuiApplication::primaryScreen();
//	UdpHolder holder;
	MainWindow wndw;
	wndw.resize(scr->geometry().width()/2, scr->geometry().height());
	wndw.move(0, 0);
	wndw.show();
//	CamHolder cum;
//	cum.resize(640, 480);
//	cum.show();
//	SpinLayout wndw1("depth", 0, 5, 1);
//	wndw1.show();
//	PowerLimit wndw1;
//	wndw1.show();
//	LogsHolder::initLogFile();
//	LogsHolder wndw1;
//	wndw1.show();
//	Telemetry wndw1;
//	wndw1.show();
	return app.exec();
}
