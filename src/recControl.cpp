#include <QDebug>
#include <QString>

#include "recControl.h"

RecControl::RecControl(QWidget *parent) : QWidget(parent)
{
	m_startBtn_ptr = new QPushButton("Start recording", this);
	m_pauseBtn_ptr = new QPushButton("Pause recording", this);
	m_stopBtn_ptr = new QPushButton("Stop recording", this);
	m_hbox_ptr = new QHBoxLayout(this);
	m_vbox_ptr = new QVBoxLayout(this);

	m_devList_ptr = new QListWidget(this);
	m_devList_ptr->setMinimumWidth(410);

	m_hbox_ptr->addWidget(m_startBtn_ptr);
	m_hbox_ptr->addWidget(m_stopBtn_ptr);
	m_hbox_ptr->addWidget(m_pauseBtn_ptr);

	m_vbox_ptr->addWidget(m_devList_ptr);
	m_vbox_ptr->addLayout(m_hbox_ptr);
	
	connect(m_stopBtn_ptr, &QPushButton::clicked, this, &RecControl::s_emitStop);
	connect(m_pauseBtn_ptr, &QPushButton::clicked, this, &RecControl::s_emitPause);
	connect(m_startBtn_ptr, &QPushButton::clicked, this, &RecControl::s_emitStart);
	connect(m_devList_ptr, &QListWidget::itemActivated, this, &RecControl::s_emitConnection);

}
RecControl::~RecControl()
{
	delete m_startBtn_ptr;
	delete m_pauseBtn_ptr;
	delete m_stopBtn_ptr;
	delete m_hbox_ptr;
	delete m_vbox_ptr;
	delete m_devList_ptr;
	for(QListWidgetItem *it : m_devices_vctr)
	{
		delete it;
	}

}
void RecControl::s_emitStart()
{
	qDebug() << "Start recording signal emited" << Qt::endl;
	emit sig_startRec();
}
void RecControl::s_emitPause()
{
	qDebug() << "Pause recording signal emited" << Qt::endl;
	emit sig_pauseRec();
}
void RecControl::s_emitStop()
{
	qDebug() << "Stop recording signal emited" << Qt::endl;
	emit sig_stopRec();
}
void RecControl::s_gotNewDev(const std::string &devName)
{
	
	QListWidgetItem *item = new QListWidgetItem;
	item->setText(QString::fromStdString(devName));
	m_devices_vctr.push_back(item);
	m_devList_ptr->insertItem(m_row++, item);
	
}
void RecControl::s_removeItem(const std::string &item)
{
	for(QListWidgetItem *it : m_devices_vctr)
	{
		if(it->text() == QString::fromStdString(item))
		{
			qDebug() << "Removed item " << it->text() << '\n';
			m_devList_ptr->takeItem(m_devList_ptr->row(it));
			m_devList_ptr->update();
			
		}
	}
}
void RecControl::s_emitConnection(QListWidgetItem *dev)
{
	emit sig_connect(std::string(dev->text().toStdString()));
}
