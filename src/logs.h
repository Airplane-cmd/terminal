#pragma once
#include <QTextEdit>
#include <QString>
#include <QDateTime>
#include <QFile>
#include <QDir>
class LogsHolder : public QWidget
{
public:
	LogsHolder(QWidget *parent = 0);
	static QString initLogFile();
	static void writeInLog(const QString &msg);
private:
	QTextEdit *te;
};
	
