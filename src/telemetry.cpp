#pragma once
#include <QWidget>
#include <QVBoxLayout>
#include <QString> 

#include "telemetry.h"
Telemetry::Telemetry(QWidget *parent) : QWidget(parent)
{
	QString s_depth = "Depth: ";
	QString s_velocity = "Velocity: ";
	depth_l = new QLabel(s_depth, this);
	velocity_l = new QLabel(s_velocity, this);
	QVBoxLayout *vbox = new QVBoxLayout(this);
	vbox->addWidget(depth_l);
	vbox->addWidget(velocity_l);
	setLayout(vbox);

}

