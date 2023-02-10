#pragma once
#include <QTextStream>
#include <QDateTime>
#include <QTextEdit>
#include <QString>
#include <QDir>
#include <QFile>
//#include <QVBoxLayout>

#include "logs.h"
LogsHolder::LogsHolder(QWidget *parent) : QWidget(parent)
{
//	QHBoxLayout *hbox = new QHBoxLayout(this);
	te = new QTextEdit(this);
	te->setGeometry(10, 10, 400, 400);
	te->setReadOnly(1);
}
QString LogsHolder::initLogFile()
{
	QString path = "../data/logs/";
	QDir logs(path);
	logs.setFilter(QDir::Files);// | QDir::AllDirs);
	logs.setSorting(QDir::Time);
	QFileInfoList list = logs.entryInfoList();
	QTextStream out(stdout);
	QString today  =  QDate::currentDate().toString("dd:MM:yyyy");
	int index = 0;
	QString rowNum;
	QString filename;
	bool localed = 0;
	foreach(QFileInfo inf, list)
	{
		QString name = inf.fileName().section(".", 0, 0);//
		int i;
		
		for(i = 0; i < today.size(); ++i)
		{
			if(name[i] != today[i])		break;
			if(i == name.size() - 1)	
			{
				if(name.size() > today.size())
				{
					for(int j = i; j < name.size(); ++j)
					{
						if(name[j] != "(" || name[j] != ")")	rowNum += name[j];	
					}
					if(index < rowNum.toInt() + 1)	index = rowNum.toInt() + 1;
					localed = 1;
					rowNum.clear();
//					filename += today + QString("(%1).txt").arg(index);

				}
				else if(name.size() == today.size())
				{
					localed = 1;
					if(index == 0)	index = -1;
				}
			}
		}
	}
	if(!localed)
	{
		filename += today + ".txt";
	}
	else if(index == -1)	filename += today + QString("(%1).txt").arg(1);
	else			filename += today + QString("(%1).txt").arg(index);
	QFile log(path + filename);
	if(log.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream fout(&log);
		fout << "Terminal launched" << '\n';
		log.close();

	}
	return filename;
}

