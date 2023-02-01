#pragma once
#include <QWidget>
#include <QVBoxLayout>
#include <QString> 
#include <vector>
#include "telemetry.h"
Telemetry::Telemetry(QWidget *parent) : QWidget(parent)
{
	setStyleSheet("background-color:yellow;");
	std::vector<QLabel *> labels;
	QVBoxLayout *vbox = new QVBoxLayout(this);


	QString s_depth = "Depth: ";
	QString s_velocity = "Velocity: ";
	QString s_row = "Row: ";
	QString s_pitch = "Pitch: ";
	QString s_yaw = "Yaw: ";
	depth_l = new QLabel(s_depth, this);
	velocity_l = new QLabel(s_velocity, this);
	row_l = new QLabel(s_row, this);
	pitch_l = new QLabel(s_pitch, this);
	yaw_l = new QLabel(s_yaw);
	labels.push_back(depth_l);
	labels.push_back(velocity_l);
	labels.push_back(row_l);
	labels.push_back(pitch_l);
	labels.push_back(yaw_l);
	for(int i = 0; i < labels.size(); ++i)
	{
		if(i != labels.size())
		{
			labels[i]->setSizePolicy(QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed));
			labels[i]->setMaximumSize(400, 50);
		}
		labels[i]->setAlignment(Qt::AlignLeft | Qt::AlignTop);
		vbox->addWidget(labels[i]);

	}
//	velocity_l->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
//	velocity_l->setMaximumSize(400, 50);

//	velocity_l->setAlignment(Qt::AlignLeft | Qt::AlignTop);


//	vbox->addWidget(velocity_l);
//	vbox->setContentsMargins(1, 1, 1, 1);
	setLayout(vbox);

}

