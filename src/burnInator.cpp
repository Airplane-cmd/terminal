#include <QDebug>
#include <iostream>
#include <algorithm>
#include <filesystem>
#include <sstream>
#include <fstream>
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
	std::pair<std::array<float, 3>, bool> emptyObjInstance;
	emptyObjInstance.second = 0;
	for(int i = 0; i < m_pidQcb_ptr->count(); ++i)	m_temporalValues_obj_vctr.push_back(emptyObjInstance);

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
	connect(m_pidQcb_ptr, &QComboBox::currentTextChanged, this, &BurnInator::s_setupQle);//?	
	
	m_setupQle();//?
}
void BurnInator::m_getConfigData()//and setup qle from ROM, should be called on qcbTextChanged if not marked also should create function for setup temporal object on apply
{
	if(std::filesystem::exists(m_dir))	qDebug() << "path " << QString(m_dir.c_str()) << " exists\n"; 
	else
	{
		qDebug() << "path "<< QString(m_dir.c_str()) << " is anawailable\n";
		std::filesystem::create_directory(m_dir);
		
	}
    std::string newString = "";  
	qDebug() << "Creating file\n";
	std::ifstream configFile(m_dir + m_filename);//txt?
	qDebug() << "File created\n";
    std::stringstream buff;
    std::string data;
    buff << configFile.rdbuf();
	qDebug() << "here\n";
    data = buff.str();
	qDebug() << data.c_str() << '\n';
    if(data == "")  for(uint8_t i = 0; i < m_pidQcb_ptr->count(); ++i)  data.append("0 0 0\n");
    std::vector<uint16_t> newlineIndexes;
	newlineIndexes.push_back(0);
    for(int i = 0; i < data.size(); ++i)
    {
        if(data[i] == '\n') newlineIndexes.push_back(i);
    }
    for(int i = newlineIndexes[m_pidIndex] + 1; i < newlineIndexes[m_pidIndex + 1]; ++i)
    {
        newString.push_back(data[i]);
    }
	qDebug() << "here\n";
//	int j = (m_pidIndex == 0 ) ? -2 : -1;
	int j = 0;
    std::string str = "";
	int t = (m_pidIndex == 0) ? 0 : 0;
    for(int i = t; i < newString.size(); ++i)
    {
//		if(m_pidIndex == 0)
        if(newString[i] != '|' && newString[i] != ' ')
        {
            str.push_back(newString[i]);
        }
        else if(newString[i] == '|')
        {
            m_qle_arr[j]->setText(str.c_str());
            str = "";
			++j;
        }
//        else 
//        {
//          str = "";
//			++j;
//        }
    }//hope you can debug it, asshole, you wrote it btw
	qDebug() << "trying to close config file\n";
    configFile.close();
	qDebug() << "config file closed\n";
}
void BurnInator::m_setConfigData()//should be called on burn
{
	qDebug() << "trying to save config\n";
	if(std::filesystem::exists(m_dir))	qDebug() << "path " << QString(m_dir.c_str()) << " exists\n"; 
	else
	{
		qDebug() << "path "<< QString(m_dir.c_str()) << " is anawailable\n";
		std::filesystem::create_directory(m_dir);
		
	}
    std::string newString = "";
 	for(QLineEdit *it : m_qle_arr)
	{
		newString.append((it->text()).toUtf8() + '|');
	}
	std::ifstream configFile(m_dir + m_filename);//txt?
	std::string data;
	if(std::filesystem::exists(m_dir + m_filename))
	{
		qDebug() << "path " << QString(m_dir.c_str()) << " exists\n"; 
    	std::stringstream buff;
    	buff << configFile.rdbuf();
    	data = buff.str();
	}
	else
	{
		qDebug() << "path "<< QString(m_dir.c_str()) << " is anawailable\n";
//		std::filesystem::create_directory(m_dir);
		for(uint8_t i = 0; i < m_pidQcb_ptr->count(); ++i)  data.append("0|0|0|\n");
	}
	configFile.close();
// if(data == "")  
	std::vector<uint16_t> newlineIndexes;
    newlineIndexes.push_back(0);
    for(int i = 0; i < data.size(); ++i)
    {
        if(data[i] == '\n') newlineIndexes.push_back(i);
    }
    data.replace(newlineIndexes[m_pidIndex] + 1, newlineIndexes[m_pidIndex + 1] - newlineIndexes[m_pidIndex] - 1, newString);//empty? should be fine +1
	std::ofstream configFileOut(m_dir + m_filename);
	configFileOut << data.c_str();//writes edited string in config file
	configFileOut.close();
	m_setTempObj();	
}

void BurnInator::m_setTempObj()
{
    for(int i = 0; i < 3; ++i)
    {
        m_temporalValues_obj_vctr[m_pidIndex].first[i] = m_qle_arr[i]->text().toFloat();
	m_temporalValues_obj_vctr[m_pidIndex].second = 1;
    }
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

void BurnInator::m_setupQle()//in constructor and on s_setupQle(QString)
{
	m_pidName = m_pidQcb_ptr->currentText().toStdString();//does currentText() give correct text?
	m_pidIndex = m_pidQcb_ptr->currentIndex();
	if(m_temporalValues_obj_vctr[m_pidIndex].second)//if object marked as actual, read from temporal object
	{
		for(uint8_t i = 0; i < 3; ++i)	m_qle_arr[i]->setText(QString::number(m_temporalValues_obj_vctr[m_pidQcb_ptr->currentIndex()].first[i]));//?
	}
	else//read from config string constaining 3 floats
	{
		m_getConfigData();
	}
	qDebug() << "m_setupQle\n";
}
void BurnInator::s_setupQle(QString)
{
	m_setupQle();
	qDebug() << "s_setupQle\n";
}
void BurnInator::s_processApply()
{
	m_setTempObj();
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
//	std::string pidName = m_pidQcb_ptr->currentText().toStdString();
	if(m_pidName == "Depth")		emit sig_setDepthPdi(numbers);
	else if(m_pidName == "Yaw")	emit sig_setYawPdi(numbers);
	//. . .
	//insert another PID controllers here

	std::cout << "Numbers for " << m_pidName << " were sent" << '\n';
}
void BurnInator::s_rebootBoard()
{
	emit sig_rebootBoard();
	std::cout << "reboot signal was emited" << '\n';
}
void BurnInator::s_burnNumbers()
{
	m_setConfigData();
	emit sig_burnNumbers();
	std::cout << "burn siganal was emited" << '\n';
}
