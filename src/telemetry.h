#pragma once
#include <QWidget>
#include <QLabel>

class Telemetry : public QWidget
{
Q_OBJECT
public:
	Telemetry(QWidget *parent = 0);
private: 
	QLabel *depth_l;
	QLabel *velocity_l;
private slots:

};
