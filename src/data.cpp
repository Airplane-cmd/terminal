#include "data.h"
Data::Data(QWidget *parent) : QWidget{parent}
{
	initBttns();
	rec->setCheckable(1);
	rec->setChecked(0);
	connect(rec, &QPushButton::clicked, this, &Data::recording);
	connect(save, &QPushButton::clicked, this, &Data::saveRecord);
	connect(erese, &QPushButton::clicked, this, &Data::eraseRecord);
}
void Data::initBttns()
{
	grid = new QGridLayout();
	rec = new QPushButton("Start recording", this);
	save = new QPushButton("Save", this);
	erase = new QPushButton("Erase", this);

}
void Data::recording()
{
	if(rec->isChecked())
	{
		rec.setText("Stop recording");

	}
	else
	{
		rec.setText("Start recording");

	}
}
void Data::saveRecord()
{

}
void Data::eraseRecord()
{

}

