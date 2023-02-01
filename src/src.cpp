#include <QApplication>
#include <QSize>
#include <QScreen>
#include "mainWindow.h"
#include "telemetry.h"
int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	QScreen *scr = QGuiApplication::primaryScreen();
	MainWindow wndw;
	wndw.resize(scr->geometry().width(), scr->geometry().height());
	wndw.show();
//	Telemetry wndw1;
//	wndw1.show();
	return app.exec();
}
