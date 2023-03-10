#pragma once
#include <QWidget>
#include <QLabel>
#include <QDebug>
class Telemetry : public QWidget
{
Q_OBJECT
public:
	Telemetry(QWidget *parent = 0);
	~Telemetry();
public slots:
	void updateTelemetry(std::array<float, 13>, std::vector<std::vector<bool>>);
private: 
	QLabel *depth_l;
	QLabel *velocity_l;
	QLabel *row_l;
	QLabel *pitch_l;
	QLabel *yaw_l;

};
