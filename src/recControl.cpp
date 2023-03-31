#include "recControl.h"

RecControl::RecControl(QWidget *parent) : QWidget(parent)
{
	m_startBtn_ptr = new QPushButton("Start recording", this);
	m_pauseBtn_ptr = new QPushButton("Pause recording", this);
	m_stopBtn_ptr = new QPushButton("Stop recording", this);
	m_hbox_ptr = new QHBoxLayout(this);

	m_hbox_ptr->addWidget(m_startBtn_ptr);
	m_hbox_ptr->addWidget(m_stopBtn_ptr);
	m_hbox_ptr->addWidget(m_pauseBtn_ptr);

	connect(m_stopBtn_ptr, &QPushButton::clicked, this, &RecControl::s_emitStop);
	connect(m_pauseBtn_ptr, &QPushButton::clicked, this, &RecControl::s_emitPause);
	connect(m_startBtn_ptr, &QPushButton::clicked, this, &RecControl::s_emitStart);

}
RecControl::~RecControl()
{
	delete m_startBtn_ptr;
	delete m_pauseBtn_ptr;
	delete m_stopBtn_ptr;
	delete m_hbox_ptr;
}
void RecControl::s_emitStart()
{
	emit sig_startRec();
}
void RecControl::s_emitPause()
{
	emit sig_pauseRec();
}
void RecControl::s_emitStop()
{
	emit sig_stopRec();
}


