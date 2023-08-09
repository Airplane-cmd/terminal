#include <QWidget>
#include <QVBoxLayout>
#include <QAction>
#include <QTextStream>
#include <QDebug>

#include <QAction>

#include "algorithms.h"

Algorithms::Algorithms(QWidget *parent) : QWidget(parent)
{
	QWidget *cntnr = new QWidget(this);
	QVBoxLayout *vbox = new QVBoxLayout();
	initButtons();
	for(int i = 0; i < m_bttns_vctr.size(); ++i)	vbox->addWidget(m_bttns_vctr[i]);

	connect(m_bttns_vctr[1], &SpinLayout::activationButtonPressed, this, &Algorithms::emitYawControlSignal);

	connect(m_bttns_vctr[0], &SpinLayout::activationButtonPressed, this, &Algorithms::emitDepthControlSignal);
	connect(m_bttns_vctr[2], &SpinLayout::activationButtonPressed, this, &Algorithms::emitRollControlSignal);
	connect(m_bttns_vctr[3], &SpinLayout::activationButtonPressed, this, &Algorithms::emitPitchControlSignal);

	cntnr->setLayout(vbox);
	setLayout(vbox);
	cntnr->setMaximumWidth(4100);
//	cntnr->setMinimumHeight(1500);

//	qDebug() << "constructor worked" << Qt::endl;
}
Algorithms::~Algorithms()
{
	for(SpinLayout *obj : m_bttns_vctr)
	{
		delete obj;
	}
}
void Algorithms::initButtons()
{
	m_bttns_vctr.push_back(new SpinLayout("Depth", -1000, 1000, 0, 0, this));
	m_bttns_vctr.push_back(new SpinLayout("Yaw", 0, 360, 0, 0.1, this));
	m_bttns_vctr.push_back(new SpinLayout("Roll", 0, 360, 0, 0.1, this));
	m_bttns_vctr.push_back(new SpinLayout("Pitch", 0, 360, 0, 0.1, this));

//	m_bttns_vctr.push_back(new SpinLayout("Altitude", 0, 5, 1, this));
//	qDebug() << m_bttns_vctr.size() << Qt::endl;
}
void Algorithms::setDepth(float data)
{
	if(m_bttns_vctr[0]->getState())	m_bttns_vctr[0]->setValue(data);
}
void Algorithms::setYaw(float data)
{
	if(m_bttns_vctr[1]->getState())	m_bttns_vctr[1]->setValue(data);
}
void Algorithms::emitDepthControlSignal(bool state)
{
	float depth = (m_bttns_vctr[0])->getValue();
	emit depthControl(state, depth);
}
void Algorithms::emitRollControlSignal(bool state)
{
	float depth = (m_bttns_vctr[0])->getValue();
	emit rollControl(state, depth);
}
void Algorithms::emitPitchControlSignal(bool state)
{
	float depth = (m_bttns_vctr[0])->getValue();
	emit pitchControl(state, depth);
}

void Algorithms::emitYawControlSignal(bool state)
{
	float yaw = m_bttns_vctr[1]->getValue();
	qDebug() << " yaw: " << yaw << Qt::endl;
	emit yawControl(state, yaw);
}


