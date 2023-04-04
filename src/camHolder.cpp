#include <QCoreApplication>
#include <QDebug>
//#include <QString>
//#include <QDate>

#include "opencv2/opencv.hpp"
#include <iostream>
#include <chrono>
#include <filesystem>
#include <ctime>
#include <string>
#include <iomanip>
#include <sstream>
//#include <thread>
//#include <chrono>
//#include <future>

#include "camHolder.h"
CamHolder::CamHolder(QWidget *parent) : QWidget(parent)
{
	m_index = 0;
	m_write_f = 0;
	m_writeInit_f = 0;
	m_framerate_d = 1000 / 100;
	m_videoLabel_ptr = std::make_shared<QLabel>(this);
//	m_videoLabel_ptr->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	m_videoLabel_ptr->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	m_timer_ptr = new QTimer(this);
//	m_timer_ptr = std::make_shared<QTimer>(this);
//	m_streaming_b = 1;
    	connect(m_timer_ptr, &QTimer::timeout, this, &CamHolder::stream);
	m_timer_ptr->start(m_framerate_d);//works?
	m_width_d = m_capture.get(cv::CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
	m_height_d = m_capture.get(cv::CAP_PROP_FRAME_HEIGHT);
	std::cout << m_height_d << " " << m_width_d << std::endl;
//get the height of frames of the video
//    	std::cout << "camera width = " << dWidth << ", height = " << dHeight << std::endl;
//	if (!m_capture.isOpened()) 	std::cout << "cannot open camera" << std::endl;
	m_dir = "../data/video/";//argv[1];
	findPath();
}
void CamHolder::connect_(int cam)
{
	std::string camPath = "/dev/video" + std::to_string(cam);
	std::cout << "connecting..." << std::endl;
	m_capture.open(camPath);
	m_capture.set(cv::CAP_PROP_FRAME_WIDTH, 640);
	m_capture.set(cv::CAP_PROP_FRAME_HEIGHT, 480);//int(m_height_d));
}
void CamHolder::findPath()
{
//	std::string logsFolder = "../data/video";
	if(std::filesystem::exists(m_dir))		qDebug() << "path exists" << Qt::endl; 
	else
	{
		qDebug() << "path is anawailable" << Qt::endl;
		std::filesystem::create_directory(m_dir);
	}
		

}
void CamHolder::s_startRec()
{
	if(m_write_f)	return;
	if(!m_writeInit_f)	initRec();
	m_write_f = 1;
	m_writeInit_f = 1;
}
void CamHolder::s_stopRec()
{
	if(!(m_write_f || m_writeInit_f))	return;
	m_writeInit_f = 0;
	m_write_f = 0;
	m_videoWriter.release();
}
void CamHolder::s_pauseRec()
{
	if(m_write_f && m_writeInit_f)
	m_write_f = 0;
}
void CamHolder::s_setWriteFlag(bool flag)
{
	m_write_f = flag;
	if(!m_writeInit_f)
	{
		initRec();
		m_writeInit_f = 1;
	}
}
void CamHolder::initRec()
{
	std::cout << "function called" << std::endl;
	auto now = std::chrono::system_clock::now();
	std::time_t date = std::chrono::system_clock::to_time_t(now);
	std::stringstream ss;
	ss << std::put_time(std::localtime(&date), "%d_%m_%Y");
	std::string currentDate = ss.str();

	int indexMax = 0;	
	int index_l = 0;
	int filesCounter = 0;
	int index = 0;
	std::string filename;
	int i = 0;
	for(const auto &fileInfo : std::filesystem::directory_iterator(m_dir))
	{

		int indexS = 0;
 
		filename = fileInfo.path().filename().string();
		std::cout << filename << "  ";// << std::endl;//db
		std::cout << filename.substr(0, 10) << std::endl;
		if(filename.substr(0, 10) != currentDate)	continue;
		for(uint8_t i = filename.size() - 1; i > 0; --i)
		{
			if(filename[i] == '_')
			{
				indexS = i;
				break;
			}
		}
		std::cout << i << " string index: " <<  std::stoi(filename.substr(indexS + 1, filename.size() - 1)) << std::endl;
		index_l = std::stoi(filename.substr(indexS + 1, filename.size() - 1));
		std::cout << index_l << " | " << std::endl;
		if(index_l > indexMax)	indexMax = index_l;
		++filesCounter;
	}
	if(filesCounter == 0)	index = 0;
	else 			index = indexMax + 1;
	int fourcc = cv::VideoWriter::fourcc('m', 'p', '4', 'v');
	cv::Size frameRes{int(m_width_d), int(m_height_d)};
	m_videoWriter = cv::VideoWriter(std::string(m_dir + currentDate + '_' + std::to_string(index) + ".mp4"), fourcc, int(m_framerate_d), frameRes);//works?
	
}
void CamHolder::stream()
{
	std::cout << "Write state: " << m_write_f << std::endl; 
	bool readState = 0;
	m_width_d = m_capture.get(cv::CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
	m_height_d = m_capture.get(cv::CAP_PROP_FRAME_HEIGHT);

	readState = m_capture.read(m_frame_cv); // read a new frame from video 
	if(!readState) 
        {
//		std::cout << "Video camera is disconnected" << std::endl;
		if(std::filesystem::exists("/dev/video2"))
		{
			std::cout << "Video2 found" << std::endl;
			connect_(2);
		}
		else if(std::filesystem::exists("/dev/video3"))
		{
			std::cout << "Video3 found" << std::endl;
			connect_(3);
		}
		return;
	}
//	cv::imshow("video", m_frame_cv);
	std::string path = m_dir + std::to_string(m_index) + ".jpg";
//	cv::imwrite(path, m_frame_cv);
	if(m_write_f)	m_videoWriter.write(m_frame_cv);
        m_index++;
	QImage img(m_frame_cv.data, m_frame_cv.cols, m_frame_cv.rows, m_frame_cv.step, QImage::Format_BGR888);
	m_videoLabel_ptr->setPixmap(QPixmap::fromImage(img));
	m_videoLabel_ptr->adjustSize();
}
