#pragma once
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSlider>
#include <QLabel>

#include <vector>

class PowerLimit : public QWidget
{
Q_OBJECT
private:
	QSlider *qs;
	QLabel *dig;
	std::vector<QPushButton *> bttns;
private slots:
	void setSliderValue_A();
	void setSliderValue_B();
	void setSliderValue_C();
	void setSliderValue_D();
	void setLabel();
public slots:
	void s_setSliderValue(uint8_t);
public:
	PowerLimit(QWidget *parent = 0);
	~PowerLimit();
	int getValue();
signals:
	void setForce(uint8_t);
};
