#include "spinLayout.h"
SpinLayout::SpinLayout(QString name, float beginRange, float endRange, float startValue, QWidget *parent) : QWidget(parent), m_beginRange(beginRange), m_endRange(endRange), m_startValue(startValue) 
{
	m_getBttn_ptr = new QPushButton("Sensors data", this);
	m_getBttn_ptr->setCheckable(1);
	m_getBttn_ptr->setChecked(0);
	m_button_ptr = new QPushButton(name, this);
	m_button_ptr->setCheckable(1);
	m_button_ptr->setChecked(0);
	m_state_b = 0;
	m_state_d = 0;
	m_dsb_ptr = new QDoubleSpinBox(this);
	m_dsb_ptr->setRange(m_beginRange, endRange);
	m_dsb_ptr->setValue(m_startValue);
	m_dsb_ptr->setSingleStep((m_endRange - m_beginRange) / 100);
	QHBoxLayout *hbox_ptr = new QHBoxLayout(this);
	hbox_ptr->setSpacing(15);

	hbox_ptr->addWidget(m_dsb_ptr);
	hbox_ptr->addWidget(m_button_ptr);
	hbox_ptr->addWidget(m_getBttn_ptr);

	connect(m_button_ptr, &QPushButton::clicked, this, &SpinLayout::emitActivationSignal);
	connect(m_getBttn_ptr, &QPushButton::clicked, this, &SpinLayout::emitSettingSignal);
	QWidget *cntnr = new QWidget(this);
	cntnr->setLayout(hbox_ptr);
	cntnr->setMinimumWidth(400);
	cntnr->setMinimumHeight(40);

//	delete cntnr;
//	delete hbox_ptr;
}
SpinLayout::~SpinLayout()
{
	delete m_getBttn_ptr;
	delete m_button_ptr;
	delete m_dsb_ptr;
}
void SpinLayout::setValue(float data)
{
	m_dsb_ptr->setValue(data);
}
float SpinLayout::getValue()
{
	return m_dsb_ptr->value();
}
bool SpinLayout::getState()
{
	return m_state_d;
}
void SpinLayout::emitActivationSignal()
{
	m_state_b = !m_state_b;
	emit activationButtonPressed(m_state_b);
}
void SpinLayout::emitSettingSignal()
{
	m_state_d = !m_state_d;
	emit settingButtonPressed(m_state_d);
}
