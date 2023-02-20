#pragma once
#include <QTextStream>
#include <QDateTime>
#include <QTextEdit>
#include <QString>
#include <QDir>
#include <QFile>
//#include <QVBoxLayout>

#include "logs.h"
#include "mainWindow.h"
LogsHolder::LogsHolder(QWidget *parent) : QWidget(parent)
{
//	QHBoxLayout *hbox = new QHBoxLayout(this);
	qte = new QTextEdit(this);
	qte->setGeometry(10, 10, 400, 400);
	qte->setReadOnly(1);
	log_file = initLogFile();
//	MainWindow::curLog = initLogFile();
}
QString LogsHolder::initLogFile()
{
	QString path = "../data/logs/";
	QDir logs(path);
	logs.setFilter(QDir::Files);// | QDir::AllDirs);
	logs.setSorting(QDir::Time);
	QFileInfoList list = logs.entryInfoList();
	QTextStream out(stdout);
	QString today  =  QDate::currentDate().toString("dd_MM_yyyy");
	int index = 0;
	QString rowNum;
	QString filename;
	bool localed = 0;
	if(!logs.exists())	logs.mkpath(".");
	foreach(QFileInfo inf, list)
	{
		QString name = inf.fileName().section(".", 0, 0);//
		int i;	
		for(i = 0; i < today.size(); ++i)
		{
			if(name[i] != today[i])		break;
			if(i == today.size() - 1)	
			{
				if(name.size() > today.size())
				{
					for(int j = i + 1; j < name.size(); ++j)if(name[j] != "_")	rowNum += name[j];
					if(index < rowNum.toInt() + 1)	index = rowNum.toInt() + 1;
					localed = 1;
					rowNum.clear();
				}
				else if(name.size() == today.size())
				{
					localed = 1;
					if(index == 0)	index = -1;
				}
				else	out << "sucker" << Qt::endl;
			}
		}
	}
	if(!localed)
	{
		filename += today + ".txt";
	}
	else if(localed && index == -1)	filename += today + QString("_%1.txt").arg(1);
	else			filename += today + QString("_%1.txt").arg(index);
	filename = path + filename;
	QFile log(filename);
	if(log.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream fout(&log);
		QString data = "Terminal launched " +  QTime::currentTime().toString("hh:mm:ss:zzz");
		fout << data << '\n';
		qte->append(data);	
		qte->show();
		log.close();

	}
	log_file = filename; 
	return filename;
}
void LogsHolder::writeInLog(const QString &msg)
{

	QString data = msg + " " + QString(QTime::currentTime().toString("hh:mm:ss:zzz"));
	QFile log(log_file);
	if(log.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
	{
		QTextStream fstream(&log);
		fstream << data << Qt::endl;
		log.close();
	}
	qte->append(data);
	qte->show();


}

