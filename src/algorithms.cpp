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

//	connect(bttns[0], &QPushButton::clicked, this, &Algorithms::depthControl);
	
	cntnr->setLayout(vbox);
	setLayout(vbox);
	cntnr->setMaximumWidth(410);

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
	m_bttns_vctr.push_back(new SpinLayout("Depth", 0, 5, 0, this));
	m_bttns_vctr.push_back(new SpinLayout("Yaw", -180, 180, 1, this));
	m_bttns_vctr.push_back(new SpinLayout("Altitude", 0, 5, 1, this));
	qDebug() << m_bttns_vctr.size() << Qt::endl;
}
void Algorithms::depthControl()
{
	QTextStream out{stdout};
//	if(bttns[0]->isChecked())	out << "Depth control activated" << Qt::endl;
//	else				out << "Depth control disactivated" << Qt::endl;
}


