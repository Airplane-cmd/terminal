#pragma once
#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QListWidget>
class Data : public QWisget
{
Q_OBJECT
public:
	Data(QWidget *parent = 0);
private:
	QGridLayout *grid;
	QPushButton *rec;
	QPushButton *save;
	QPushButoon *erase;
	QListWidget *camList;
	int num;
	void initBttns();
	
private slots:
	void recording();
	void saveRecord();
	void eraseRecords();
};
