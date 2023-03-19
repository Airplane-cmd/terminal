//#pragma once
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSlider>
#include <QLabel>
#include <QString>
#include <QTextStream>

#include <vector>
#include <functional>

#include "powerLimit.h"
PowerLimit::PowerLimit(QWidget *parent) : QWidget{parent}
{

	qs = new QSlider(Qt::Horizontal, this);
	dig = new QLabel("0", this);
	QHBoxLayout *hboxB = new QHBoxLayout();
	for(int i = 0; i < 4; ++i)
	{
		int pers;
		switch(i)
		{
			case(0):	pers = 10;
					break;
			case(1):	pers = 25;
					break;
			case(2):	pers = 50;
					break;
			case(3):	pers = 100;
					break;
		}
		bttns.push_back(new QPushButton(QString::number(pers), this));
//		bttns[i]->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
//		bttns[i]->setMaximumSize(100, 40);
		//bttns[i]->setAlignment(Qt::AlignLeft | Qt::AlignTop);
		hboxB->addWidget(bttns[i]);
//		connect(bttns[i], &QPushButton::clicked, this, func[i]);
	}
	QTextStream out{stdout};
//	out << "i want to fuck my girl, no to do stupid buttons, gui sucks" << Qt::endl;
	connect(bttns[0], &QPushButton::clicked, this, &PowerLimit::setSliderValue_A);
	connect(bttns[1], &QPushButton::clicked, this, &PowerLimit::setSliderValue_B);
	connect(bttns[2], &QPushButton::clicked, this, &PowerLimit::setSliderValue_C);
	connect(bttns[3], &QPushButton::clicked, this, &PowerLimit::setSliderValue_D);
//	hboxB->setContentsMargins(0, 0, 0, 0);
	QVBoxLayout *vbox = new QVBoxLayout(this);
	QHBoxLayout *hboxS = new QHBoxLayout();

	QWidget *shit = new QWidget(this);
	shit->setLayout(hboxB);
	shit->setMaximumWidth(410);
//	qs->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
//	qs->setMaximumSize(360, 40);
//	dig->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
//	dig->setMaximumSize(40, 40);
//	qs->setAlignment(Qt::AlignLeft | Qt::AlignTop);
//	dig->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	hboxS->addWidget(qs);
	hboxS->addWidget(dig);
	QWidget *cunt = new QWidget(this);
	cunt->setLayout(hboxS);
	cunt->setMaximumWidth(410);
	vbox->addWidget(shit);
	vbox->addWidget(cunt);
	setLayout(vbox);

//	connect(qs, &QSlider::valueChanged, dig, static_cast<void(QLabel::*)(int)>(&QLabel::setNum));
	connect(qs, &QSlider::valueChanged, this, &PowerLimit::setLabel);


}
PowerLimit::~PowerLimit()
{
	delete qs;
	delete dig;
}
void PowerLimit::setLabel()
{
	dig->setText(QString::number(qs->value()));
//	QByteArray force;
//	force.resize(1);
//	force[0] = uint8_t(qs->value());
	emit setForce(qs->value());
}
void PowerLimit::setSliderValue_A()
{
	qs->setValue(10);
}
void PowerLimit::setSliderValue_B()
{
	qs->setValue(25);
}
void PowerLimit::setSliderValue_C()
{
	qs->setValue(50);
}
void PowerLimit::setSliderValue_D()
{
	qs->setValue(100);
}
