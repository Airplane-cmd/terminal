#pragma once
#include <QTextEdit>
#include <QString>
#include <QDateTime>
#include <QFile>
#include <QDir>

//#include "mainWindow.h"
class LogsHolder : public QWidget
{
friend class MainWindow;
public:
	LogsHolder(QWidget *parent = 0);
	QString initLogFile();
	static void writeInLog(const QString &msg);
private:
	QTextEdit *qte;
	QString log_file;
};
	
