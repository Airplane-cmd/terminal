#ifndef ALGORITHMS_H
#define ALGORITHMS_H
#include <QWidget>
#include <QGridLayout>
#include <QPushButton>

#include <vector>

#include "spinLayout.h"
class Algorithms : public QWidget
{
Q_OBJECT
public:
	Algorithms(QWidget *parent = 0);
	~Algorithms();
private:
	std::vector<SpinLayout *> m_bttns_vctr; 	
	void initButtons();
	
public slots:
	void setDepth(float);
	void setYaw(float);
	void emitDepthControlSignal(bool);
	void emitYawControlSignal(bool);
signals:
	void depthControl(bool, float);
	void yawControl(bool, float);
};
#endif
