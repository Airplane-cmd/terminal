#include <QString>
#include <QDebug>
#include <filesystem>
#include <fstream>
#include <iostream>//?
#include <sstream>
#include "movementCalibrator.h"
MCalibrator::MCalibrator(QWidget *parent)
{
	m_configMaxIndex = 0;
	m_axisNames_arr[0] = 'X';
	m_axisNames_arr[1] = 'Y';
	m_axisNames_arr[2] = 'W';
	m_axisNames_arr[3] = 'Z';

	m_dir = "../settings/";
	m_dirDest = "../settings/configs/";

	m_axisQcb_ptr = new QComboBox;
	m_axisQcb_ptr->setCurrentIndex(0);
	m_fileQcb_ptr = new QComboBox;
//	m_fileQcb_ptr->setCurrentIndex(0);
	m_qcb_ptr_arr[0] = m_axisQcb_ptr;
	m_qcb_ptr_arr[1] = m_fileQcb_ptr;

	m_captureBttn_ptr = new QPushButton("Capture");
	m_setBttn_ptr = new QPushButton("Set");
	m_resetBttn_ptr = new QPushButton("Reset");
	m_newConfigBttn_ptr = new QPushButton("New");
	m_burnValueBttn_ptr = new QPushButton("Burn");

	m_bttns_ptr_arr[0] = m_resetBttn_ptr;
	m_bttns_ptr_arr[1] = m_newConfigBttn_ptr;
	m_bttns_ptr_arr[2] = m_setBttn_ptr;
	m_bttns_ptr_arr[3] = m_captureBttn_ptr;
	m_bttns_ptr_arr[4] = m_burnValueBttn_ptr;

	m_setConfigs();

	m_hbox_ptr = new QHBoxLayout(this);
	m_viewer_ptr = new QTextEdit(this);
	for(uint8_t i = 0; i < 6; ++i)
	{
		if(i < 2)
		{
			m_vbox_ptr_arr[i] = new QVBoxLayout;
			
		}
		if(i < 4)
		{
			m_axisQcb_ptr->addItem(QString(m_axisNames_arr[i]));//?
			m_nums_ptr_arr[i] = new QSpinBox();
			m_nums_ptr_arr[i]->setRange(-100, 100);
			m_sliders_ptr_arr[i] = new QSlider(Qt::Horizontal);
			m_sliders_ptr_arr[i]->setRange(-100, 100);
			m_labels_ptr_arr[i] = new QLabel(QString(m_axisNames_arr[i]), this);
			m_labels_ptr_arr[i]->setFixedWidth(20);
		}
		m_hbox_ptr_arr[i] = new QHBoxLayout;
	}

//	m_qli_ptr = new QWidgetItem(m_viewer_ptr);
	m_vbox_ptr_arr[1]->addWidget(m_viewer_ptr);
	for(uint8_t i = 0; i < 6; ++i)
	{
		if(i < 2)
		{
			m_hbox_ptr->addLayout(m_vbox_ptr_arr[i]);
			m_hbox_ptr_arr[0]->addWidget(m_qcb_ptr_arr[i]);
		}
		if((i < 5) && (i > 0))
		{
			
			m_hbox_ptr_arr[i]->addWidget(m_labels_ptr_arr[i - 1]);
			m_hbox_ptr_arr[i]->addWidget(m_sliders_ptr_arr[i - 1]);
			m_hbox_ptr_arr[i]->addWidget(m_nums_ptr_arr[i - 1]);
		}
		if(i < 5)
		{
			m_hbox_ptr_arr[5]->addWidget(m_bttns_ptr_arr[i]);
		}
		m_vbox_ptr_arr[0]->addLayout(m_hbox_ptr_arr[i]);
	}
	for(uint8_t i = 0; i < 4; ++i)
	{
		connect(m_sliders_ptr_arr[i], &QSlider::valueChanged, m_nums_ptr_arr[i], &QSpinBox::setValue);
		connect(m_nums_ptr_arr[i], static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), m_sliders_ptr_arr[i], &QSlider::setValue);
	}
	connect(m_newConfigBttn_ptr, &QPushButton::clicked, this, &MCalibrator::s_processNew);
	connect(this, &MCalibrator::sig_openConfig, this, &MCalibrator::s_openConfig);
	
	for(QComboBox *it : m_qcb_ptr_arr)	connect(it, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &MCalibrator::s_processQcbChange);
//	connect(m_fileQcb_ptr, &QComboBox::currentIndexChanged, this, &MCalibrator::s_processConfigChange);

}
MCalibrator::~MCalibrator()
{
	delete m_viewer_ptr;
//	delete m_qli_ptr;
	for(uint8_t i = 0; i < 6; ++i)
	{
		if(i < 2)
		{
			delete m_vbox_ptr_arr[i];
			delete m_qcb_ptr_arr[i];
			
		}
		if(i < 4)
		{
			delete m_nums_ptr_arr[i];
			delete m_sliders_ptr_arr[i];
			delete m_labels_ptr_arr[i];
		}
		delete m_hbox_ptr_arr[i];
	}
}
void MCalibrator::m_setConfigs()
{

	if(std::filesystem::exists(m_dir))		qDebug() << "path " << QString(m_dir.c_str()) << " exists\n"; 
	else
	{
		qDebug() << "path "<< QString(m_dir.c_str()) << " is anawailable\n";
		std::filesystem::create_directory(m_dir);
		
	}
	if(std::filesystem::exists(m_dirDest))		qDebug() << "path " << QString(m_dirDest.c_str()) << " exists\n"; 
	else
	{
		qDebug() << "path " << QString(m_dirDest.c_str()) << " is anawailable\n";
		std::filesystem::create_directory(m_dirDest);
		return;
	}
	std::string filename;//config
	uint l_index;
	for(const auto &fileInfo : std::filesystem::directory_iterator(m_dirDest))
	{
		filename = fileInfo.path().filename().string();
		qDebug() << filename.substr(0, 6).c_str() << "\n";
		if(filename.substr(0, 6) != "config")	continue;
		qDebug() << filename.substr(6, filename.size()).c_str() << '\n';	
		l_index = uint(std::stoi(filename.substr(6, filename.size())));
		if(l_index > m_configMaxIndex)		m_configMaxIndex = l_index;
		m_fileQcb_ptr->addItem(QString(filename.c_str()));
	}
}
std::string MCalibrator::m_getFileData(const std::string &configDir, const char axis)
{
	std::ifstream inFile(configDir + axis + ".txt");
	std::string data;
	std::stringstream buff;
	buff << inFile.rdbuf();
	data = buff.str();
//	QString data_qstr = data.c_str();
//	qDebug() << "file " << configDir + axis + ".txt content: "<< data.c_str() << '\n';
	inFile.close();
	return data;
}
std::string MCalibrator::m_getCurrentConfigDir()
{
	return m_dirDest + m_fileQcb_ptr->currentText().toStdString() + '/';
}
char MCalibrator::m_getCurrentAxis()
{
	return m_axisQcb_ptr->currentText().at(0).toLatin1();
}
void MCalibrator::m_initStructure()
{
	std::string fileData;
	std::string line;
	for(uint8_t i = 0; i < 4; ++i)
	{
		fileData = m_getFileData(m_getCurrentConfigDir(), m_axisNames_arr[i]);
		for(uint8_t j = 0; j < 202; ++j)
		{
			for(uint8_t k = 0; k < 4; ++k)
			{

			}
		}
	}
}
void MCalibrator::s_processNew()
{
	if(std::filesystem::exists(m_dirDest))
	{
		std::string currentConfigDir = m_dirDest + "config" + std::to_string(++m_configMaxIndex) + '/';
		std::string filePath;
		std::string dir = "config" + std::to_string(m_configMaxIndex);
		std::filesystem::create_directory(currentConfigDir);
		for(uint8_t i = 0; i < 4; ++i)
		{
			filePath = currentConfigDir + m_axisNames_arr[i] + ".txt";
			qDebug() << QString(filePath.c_str()) << '\n';
			std::ofstream configFile(filePath);//txt
			for(int8_t j = -100; j < 101; ++j)
			{
				std::string str = "  " + std::to_string(j) + " 0 0 0\n";
				configFile << str.c_str();
			}
			configFile.close();
		}
		m_fileQcb_ptr->addItem(QString(dir.c_str()));
		m_fileQcb_ptr->setCurrentIndex(m_fileQcb_ptr->count() - 1);//?
		qDebug() << "dir to open: " << currentConfigDir.c_str() << '\n';
		emit sig_openConfig(currentConfigDir.c_str(), m_axisQcb_ptr->currentText().at(0).toLatin1());
		
	}
}
void MCalibrator::s_openConfig(const std::string &configDir, const char axis)
{
//	m_currentConfigDir = configDir;
	qDebug() << configDir.c_str() << '\n';
/*	std::ifstream inFile(configDir + axis + ".txt");
	std::string data;
	std::stringstream buff;
	buff << inFile.rdbuf();
	data = buff.str();
	QString data_qstr = data.c_str();
	qDebug() << "nothing: "<< data.c_str() << '\n';
	m_viewer_ptr->setPlainText(data_qstr);
	m_labels_ptr_arr[0]->setText(QString(axis));
	uint8_t j = 1;
	inFile.close();
*/
	m_viewer_ptr->setPlainText(m_getFileData(configDir, axis).c_str());
	m_labels_ptr_arr[0]->setText(QString(axis));
	uint8_t j = 1;

	for(uint8_t i = 0; i < 4; ++i)
	{   

		qDebug() << i << " " << j << '\n';
		if(j > 3) continue;
                m_labels_ptr_arr[j++]->setText(QString(m_axisNames_arr[i]));
                if(m_axisNames_arr[i] == axis)  --j;
	}
	
}
void MCalibrator::s_processQcbChange(int index)
{
	
	std::string dir = m_dirDest + m_fileQcb_ptr->currentText().toStdString() + '/';
	qDebug() << dir.c_str() << '\n';
	emit sig_openConfig(dir.c_str(), m_axisQcb_ptr->currentText().at(0).toLatin1());
}
