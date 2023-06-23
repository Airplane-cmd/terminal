//#include <QString>
#include <iostream>
#include <algorithm>
#include "burnInator.h"
BurnInator::BurnInator(QWidget *parent) : QWidget(parent)
{
//	m_widget_ptr = new QWidget(this);
	m_pidQcb_ptr = new QComboBox;
	m_pidQcb_ptr->addItem("Depth");
	m_pidQcb_ptr->addItem("Yaw");
	m_pidQcb_ptr->setCurrentIndex(0);
	//. . .
	//insert new items here
	m_qle_arr[0] = new QLineEdit(this);
	m_qle_arr[1] = new QLineEdit(this);
	m_qle_arr[2] = new QLineEdit(this);

	m_labels_arr[0] = new QLabel(" Proportional", this);
	m_labels_arr[1] = new QLabel(" Integral", this);
	m_labels_arr[2] = new QLabel(" Derivative", this);

	m_setBttn_ptr = new QPushButton("Apply");
	m_burnBttn_ptr = new QPushButton("Burn");
	m_rebootBttn_ptr = new QPushButton("Reboot");
	
	m_hbox_ptr_vctr.push_back(new QHBoxLayout);

	m_vbox_ptr = new QVBoxLayout(this);
	m_vbox_ptr->addWidget(m_pidQcb_ptr);
	m_grid_ptr = new QGridLayout;	
	for(uint8_t i = 0; i < 3; ++i)
	{
		m_grid_ptr->addWidget(m_qle_arr[i], 0, i);
		m_grid_ptr->addWidget(m_labels_arr[i], 1, i);
	}
	m_vbox_ptr->addLayout(m_grid_ptr);
	m_hbox_ptr_vctr[0]->addWidget(m_setBttn_ptr);
	m_hbox_ptr_vctr[0]->addWidget(m_burnBttn_ptr);
	m_hbox_ptr_vctr[0]->addWidget(m_rebootBttn_ptr);
	m_vbox_ptr->addLayout(m_hbox_ptr_vctr[0]);

	connect(m_setBttn_ptr, &QPushButton::clicked, this, &BurnInator::s_processApply);
	connect(m_burnBttn_ptr, &QPushButton::clicked, this, &BurnInator::s_burnNumbers);
	connect(m_rebootBttn_ptr, &QPushButton::clicked, this, &BurnInator::s_rebootBoard);
	


}
BurnInator::~BurnInator()
{
	delete m_grid_ptr;
	delete m_pidQcb_ptr;
	for(QLineEdit *it : m_qle_arr)	delete it;
	delete m_setBttn_ptr;
	delete m_burnBttn_ptr;
	delete m_rebootBttn_ptr;
	delete m_hbox_ptr_vctr[0];
//	delete m_hbox_ptr;
}
void BurnInator::s_processApply()
{
	std::array<float, 3> numbers;
	for(uint8_t i = 0; i < 3; ++i)	numbers[i] = m_qle_arr[i]->text().toFloat();
	for(float it : numbers)		std::cout << it << " ";
	std::cout << '\n';
//	numbers[1] ^= numbers[2];
//	numbers[2] ^= numbers[1];
//	numbers[1] ^= numbers[2];
	std::swap(numbers[1], numbers[2]);
	for(float it : numbers)		std::cout << it << " ";
	std::cout << '\n';
	std::string pidName = m_pidQcb_ptr->currentText().toStdString();
	if(pidName == "Depth")		emit sig_setDepthPdi(numbers);
	else if(pidName == "Yaw")	emit sig_setYawPdi(numbers);
	//. . .
	//insert another PID controllers here

	std::cout << "Numbers for " << pidName << " were sent" << '\n';
}
void BurnInator::s_rebootBoard()
{
	emit sig_rebootBoard();
	std::cout << "reboot signal was emited" << '\n';
}
void BurnInator::s_burnNumbers()
{
	emit sig_burnNumbers();
	std::cout << "burn siganal was emited" << '\n';
}
