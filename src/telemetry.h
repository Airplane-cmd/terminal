#pragma once
#include <QWidget>
#include <QLabel>

class Telemetry : public QWidget
{
Q_OBJECT
public:
	Telemetry(QWidget *parent = 0);
	~Telemetry();
private: 
	QLabel *depth_l;
	QLabel *velocity_l;
	QLabel *row_l;
	QLabel *pitch_l;
	QLabel *yaw_l;

};
