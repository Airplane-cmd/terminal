
#include <QWidget>
#include <QVBoxLayout>
#include <QString> 
#include <vector>
#include <QTextStream>
#include <QDebug>

#include <cmath>

#include "telemetry.h"
Telemetry::Telemetry(QWidget *parent) : QWidget(parent)
{
	QTextStream out{stdout};
	out << "Telemetry widget created;" << Qt::endl;

	setStyleSheet("background-color:yellow;");
	std::vector<QLabel *> labels;
	QVBoxLayout *vbox = new QVBoxLayout(this);


	QString s_depth    = "Depth   : N/A";
	QString s_velocity = "Velocity: N/A";
	QString s_row      = "Roll    : N/A";
	QString s_pitch    = "Pitch   : N/A";
	QString s_yaw      = "Yaw     : N/A";
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
//	setLayout(vbox);

}
Telemetry::~Telemetry()
{
	qDebug() << "Telemetry widget destucted" << Qt::endl;
	delete depth_l;
	delete velocity_l;
	delete row_l;
	delete pitch_l;
	delete yaw_l;


}
void Telemetry::updateTelemetry(std::array<float, 13> floats, std::vector<std::vector<bool>> cntnr)
{
//	qDebug() << "called" << Qt::endl;
//	QTextStream out{stdout};
//	out << "cytvguhbijnoij9h89g78yvuhl kjpijhugyvuh jklnoubiyvu" << Qt::endl;
	depth_l->setText(QString("Depth:   %1").arg(floats[5]));
	row_l->setText(QString("Roll:     %1").arg(floats[0]));
	pitch_l->setText(QString("Pitch:   %1").arg(floats[1]));
	yaw_l->setText(QString     ("Yaw:     %1").arg(floats[2]));
	velocity_l->setText(QString("Velocity:%1").arg(sqrt(pow(floats[8], 2) + pow(floats[9], 2))));
	update();
	emit sendDepth(floats[5]);
	emit sendYaw(floats[2]);
}

