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
		if(i > 0)	connect(m_sliders_ptr_arr[i], &QSlider::valueChanged, this, &MCalibrator::s_onSliderValueChanged);
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

	if(std::filesystem::exists(m_dir))		qDebug() << "path " << QString(m_dir.c_str()) << " exists\n";	else
	{
		qDebug() << "path "<< QString(m_dir.c_str()) << " is anawailable\n";
		std::filesystem::create_directory(m_dir);
		
	}
	if(std::filesystem::exists(m_dirDest))	qDebug() << "path " << QString(m_dirDest.c_str()) << " exists\n";
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
	std::string path = configDir + axis + ".txt";
//	QString data_qstr = data.c_str();
//	qDebug() << "m_getFileData(): " << path.c_str() << "content: \n" << data.c_str() << '\n';//db
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
void MCalibrator::m_initStructure(bool db)
{
	std::string fileData;
	std::string line;
	for(uint8_t i = 0; i < 4; ++i)
	{
		std::vector<uint16_t> newLineIndexes_vctr;
		newLineIndexes_vctr.emplace_back(0);
		fileData = m_getFileData(m_getCurrentConfigDir(), m_axisNames_arr[i]);
		for(uint16_t ii = 0; ii < fileData.size(); ++ii)
		{
			if(fileData[ii] == '\n')	newLineIndexes_vctr.emplace_back(ii);
		}
//		qDebug() << "Number of lines in saved file: " << newLineIndexes_vctr.size() << '\n';//201
//		qDebug() << fileData.c_str() << '\n';//works fine
//		std::istringstream iss(fileData);
		for(uint8_t j = 0; j < newLineIndexes_vctr.size() - 1; ++j)
		{
			line = fileData.substr(newLineIndexes_vctr[j], newLineIndexes_vctr[j + 1] - newLineIndexes_vctr[j]);
			std::vector<int> whiteSpacesIndexes_vctr;
			for(uint8_t jj = (j == 0)?1:2; jj < line.size(); ++jj)
			{
				if(line[jj] == ' ')
				{
//if(j == 0)					qDebug()  << " - " << jj;//db
					whiteSpacesIndexes_vctr.emplace_back(jj);
				}
//				else
//if(j == 0)			 qDebug() << line[jj] << " ";
			}
			whiteSpacesIndexes_vctr.emplace_back(line.size());

//			qDebug() << "count of white spaces in string: " << whiteSpacesIndexes_vctr.size() << '\n';
//			if(j == 0)	qDebug() << line.c_str() << '\n';//db
			std::string number;
			for(uint8_t k = 0; k < 4; ++k)
			{
				number = line.substr(whiteSpacesIndexes_vctr[k], whiteSpacesIndexes_vctr[k + 1] - whiteSpacesIndexes_vctr[k]);

//				if(!(iss >> m_movementDataRAM_arr[i][j][k]))	qDebug() << "Error occured\n"; 
//				std::getline(iss, line, ' ');
//				qDebug() << std::stoi("");
				if(j == 0)	qDebug() << number.c_str();
				try
				{
					if(number != "") std::stoi(number.c_str());
				}
				catch(std::runtime_error &err)
				{

					qDebug() << err.what();
				}


				m_movementDataRAM_arr[i][j][k] = std::stoi(number);
			}
		}
//		iss.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
//	std::stringstream ss;	
	if(db)
	{	
		for(uint8_t i = 0; i < 4; ++i)
		{
			for(uint8_t j = 0; j < 201; ++j)
			{
				for(uint8_t k = 0; k < 4; ++k)
				{
					std::cout << std::setw(3) << m_movementDataRAM_arr[i][j][k] << " ";//db
				}
				std::cout << '\n';
			}
		}
		std::cout << '\n';
	}
//	std::cout << ss.str();
}

std::string MCalibrator::m_structToString(const char axis, bool db)
{
	std::stringstream ss;
	int8_t axisIndex = -1;
	for(uint8_t i = 0; i < m_axisNames_arr.size(); ++i)	axisIndex = (m_axisNames_arr[i] == axis) ? i : 0;
	if(axisIndex == -1)
	{
		qDebug() << "Axis not specified\n";
		return "";
	}
//	if(db)	
//	{
		for(uint8_t j = 0; j < 201; ++j)
		{
			for(uint8_t k = 0; k < 4; ++k)
			{
				ss << m_movementDataRAM_arr[axisIndex][j][k] << ' ';//2d array, fix
//				std::cout << std::setw(3) << m_movementDataRAM_arr[i][j][k] << " ";//db
			}
			ss << '\n';
		}
//		std::cout << '\n';
//	}
	std::string stringStruct = ss.str();
//	qDebug() << stringStruct.c_str() << "fuck you cunt" << '\n';//db
	return stringStruct;
}
void MCalibrator::m_setViewerText()
{
	m_viewer_ptr->setPlainText(m_structToString(m_getCurrentAxis()).c_str());
}
void MCalibrator::s_processNew()
{
	m_initStructure();
	if(std::filesystem::exists(m_dirDest))
	{
		std::string currentConfigDir = m_dirDest + "config" + std::to_string(++m_configMaxIndex) + '/';
		std::string filePath;
		std::string dir = "config" + std::to_string(m_configMaxIndex);
		std::filesystem::create_directory(currentConfigDir);
		for(uint8_t i = 0; i < 4; ++i)
		{
			filePath = currentConfigDir + m_axisNames_arr[i] + ".txt";
			qDebug() << "Path in s_processnew" << QString(filePath.c_str()) << '\n';
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
	std::string path { configDir + axis + ".txt"};
	std::ifstream inFile(path.c_str());
	std::string data;
	std::stringstream buff;
	buff << inFile.rdbuf();
	data = buff.str();
	QString data_qstr = data.c_str();
//	qDebug() << "data.c_str(): \n\n"<< data.c_str() << '\n';//db
//	qDebug() << "data_qstr: \n\n" << data_qstr << '\n';//db
//	m_viewer_ptr->setPlainText(data_qstr);
	m_labels_ptr_arr[0]->setText(QString(axis));
//	uint8_t j = 1;
	inFile.close();
//m_getFileData
	m_viewer_ptr->setPlainText(m_getFileData(configDir, axis).c_str());
//	m_setViewerText();
//	m_initStructure();
	m_setViewerText();
	m_labels_ptr_arr[0]->setText(QString(axis));
	uint8_t j = 1;

	for(uint8_t i = 0; i < 4; ++i)
	{   
		qDebug() << i << " " << j << '\n';
		if(j > 3) continue;
                m_labels_ptr_arr[j++]->setText(QString(m_axisNames_arr[i]));
                if(m_axisNames_arr[i] == axis)  --j;
	}
	qDebug() << "Axis: " << axis << '\n';//db
	qDebug() << "Path: " << path.c_str() << '\n';

//	m_initStructure();	
}
void MCalibrator::s_processQcbChange(int index)
{
	QComboBox *cbox = qobject_cast<QComboBox *>(sender());
//	m_initStructure();	
	std::string dir = m_dirDest + m_fileQcb_ptr->currentText().toStdString() + '/';
	qDebug() << dir.c_str() << '\n';//db
	emit sig_openConfig(dir.c_str(), m_axisQcb_ptr->currentText().at(0).toLatin1());

}

void MCalibrator::s_onSliderValueChanged(int vl)
{
	QSlider *slider = qobject_cast<QSlider *>(sender());
	if(slider == 0)
	{
		qDebug() << "slider is corrupted\n";
		return;
	}
	int sliderIndex = -1;
	int8_t axisIndex = -1;
	bool sliderFound = 0;
	bool axisFound = 0;
	char axis = m_getCurrentAxis();
	for(uint8_t i = 0; i < m_sliders_ptr_arr.size(); ++i)
	{
		if(sliderFound && axisFound)	break;
		if(m_sliders_ptr_arr[i] == slider && !sliderFound)
		{
			sliderIndex = i;
		}
		if(m_axisNames_arr[i] == m_getCurrentAxis() && !axisFound)
		{
			axisIndex = i;
		}
	}
	m_movementDataRAM_arr[axisIndex][m_sliders_ptr_arr[0]->value() + 100][sliderIndex] = vl;
	m_setViewerText();
	qDebug() << m_getCurrentAxis() << ' ' << axisIndex << ' ' << sliderIndex << ' ' << vl << '\n';//works fine /
}
