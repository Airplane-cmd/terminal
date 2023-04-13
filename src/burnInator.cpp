#include "burnInator.h"
BurnInator::BurnInator(QWidget *parent) : QWidget(parent)
{
//	m_widget_ptr = new QWidget(this);
	m_pidQcb_ptr = new QComboBox;
	m_pidQcb_ptr->addItem("Depth");
	m_pidQcb_ptr->addItem("Yaw");
	//. . .
	//insert new items here
	m_qle_arr[0] = new QLineEdit("Proportional:", this);
	m_qle_arr[1] = new QLineEdit("    Integral:", this);
	m_qle_arr[2] = new QLineEdit("  Derivative:", this);

	m_setBttn_ptr = new QPushButton("Apply");
	m_burnBttn_ptr = new QPushButton("Burn");
	m_rebootBttn_ptr = new QPushButton("Reboot");
	
	m_vbox_ptr = new QVBoxLayout(this);
	m_vbox_ptr->addWidget(m_pidQcb_ptr);
	for(QLineEdit *it : m_qle_arr)	m_vbox_ptr->addWidget(it);
	m_hbox_ptr = new QHBoxLayout(this);
	m_hbox_ptr->addWidget(m_setBttn_ptr);
	m_hbox_ptr->addWidget(m_burnBttn_ptr);
	m_hbox_ptr->addWidget(m_rebootBttn_ptr);
	m_vbox_ptr->addLayout(m_hbox_ptr);
}
BurnInator::~BurnInator()
{
	delete m_pidQcb_ptr;
	for(QLineEdit *it : m_qle_arr)	delete it;
	delete m_setBttn_ptr;
	delete m_burnBttn_ptr;
	delete m_rebootBttn_ptr;
	delete m_vbox_ptr;
	delete m_hbox_ptr;
}
