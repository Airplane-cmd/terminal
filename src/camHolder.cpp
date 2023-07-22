#include <QCoreApplication>
#include <QDebug>
#include <QResizeEvent>
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
#include <regex>
#include <cmath>

#include <stdio.h>

#include "camHolder.h"
CamHolder::CamHolder(QWidget *parent) : QWidget(parent)
{
	m_index = 0;
	m_write_f = 0;
	m_writeInit_f = 0;
	m_framerate_d = 1000 / 30;
	m_activeDev_str = "";
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
void CamHolder::resizeEvent(QResizeEvent* eve)
{
	QWidget::resizeEvent(eve);
	m_widgetWidth_d = eve->size().width();
	m_widgetHeight_d = eve->size().height();
}
std::string CamHolder::getDevInfo(const std::string &path)
{
	std::string cmd = "v4l2-ctl --all -d " + path;
	FILE *pipe = popen(cmd.c_str(), "r");
	if(!pipe)
	{
		return "something went wrong";
	}
	char buf[256];
	std::string res{""};
	std::string name{""};
	while(!feof(pipe))
	{
		if(fgets(buf, 256, pipe) != 0)	res += buf;
	}
	pclose(pipe);
	bool data = 0;
	std::regex name_re("Card type\\s+:\\s+(.*)\\n");
	std::smatch match;
	if(std::regex_search(res, match, name_re))
	{
//db		std::cout << match[1] << '\n';
//db		std::cout << "////////////////////////////////////////////" << '\n';
		return match[1];
	}

	else return res;
}
bool CamHolder::connect_(int cam)
{
	std::string camPath = "/dev/video" + std::to_string(cam);
	std::cout << "connecting..." << std::endl;
	if(m_capture.open(camPath))
	{
		m_capture.set(cv::CAP_PROP_FRAME_WIDTH, 640);
		m_capture.set(cv::CAP_PROP_FRAME_HEIGHT, 480);//int(m_height_d));
		return 1;
	}
	return 0;
//	std::cout << getDevInfo(camPath) << '\n';
//	std::cout << "///////////////////////////////////////////////////////////" << std::endl;
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
void CamHolder::s_setDev(const std::string &name)
{
//	std::cout << "called\n";
	m_activeDev_str = name;
	m_readState_f = 1;
}
void CamHolder::initRec()
{
	
//db	std::cout << "function called" << std::endl;
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
//db		std::cout << filename << "  ";// << std::endl;//db
//db		std::cout << filename.substr(0, 10) << std::endl;
		if(filename.substr(0, 10) != currentDate)	continue;
		for(uint8_t i = filename.size() - 1; i > 0; --i)
		{
			if(filename[i] == '_')
			{
				indexS = i;
				break;
			}
		}
//db		std::cout << i << " string index: " <<  std::stoi(filename.substr(indexS + 1, filename.size() - 1)) << std::endl;
		index_l = std::stoi(filename.substr(indexS + 1, filename.size() - 1));
//db		std::cout << index_l << " | " << std::endl;
		if(index_l > indexMax)	indexMax = index_l;
		++filesCounter;
	}
	if(filesCounter == 0)	index = 0;
	else 			index = indexMax + 1;
	int fourcc = cv::VideoWriter::fourcc('m', 'p', '4', 'v');
	cv::Size frameRes{int(m_width_d), int(m_height_d)};
	width_d = m_width_d;
	height_d = m_height_d;
	m_videoWriter = cv::VideoWriter(std::string(m_dir + currentDate + '_' + std::to_string(index) + ".mp4"), fourcc, int(m_framerate_d), frameRes);
	
}
void CamHolder::stream()
{
//	std::cout << "Write state: " << m_write_f << std::endl; 
	bool readState = 0;
	m_width_d = m_capture.get(cv::CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
	m_height_d = m_capture.get(cv::CAP_PROP_FRAME_HEIGHT);
	width_d = m_width_d;
	height_d = m_height_d;
	readState = m_capture.read(m_frame_cv); // read a new frame from video 
//	if(!readState)
//	{
		std::string filename{""};
		std::string dir{"/dev/"};
		std::vector<std::string> frameOfDevices_vctr;
		
		for(const auto &fileInfo : std::filesystem::directory_iterator(dir))
		{
			filename = fileInfo.path().filename().string();
			if(filename.find("video") == -1) continue;
//			std::cout << filename << ' ';
			std::string devName{getDevInfo(dir + filename)};


			int index = 0;
			for(std::size_t i = filename.size() - 1; i > 4; --i)	index += (filename[i] - 48) * pow(10, filename.size() - 1 - i);
//db			std::cout << devName <</* " " << index << */'\n';

			auto range = m_dev_map.equal_range(devName);
			bool found_f = 0;
			bool foundDuplicate_f = 0;
			for(auto it = range.first; it != range.second; ++it)
			{
				if(it->second == index)		found_f = 1;
				if(it->first == devName)	foundDuplicate_f = 1;
			}
			if(devName != "")	frameOfDevices_vctr.push_back(devName);
			if(!found_f)
			{

				m_dev_map.insert(std::make_pair(devName, index));
		
				if(!foundDuplicate_f)	
				{
					if(devName != "")
					{
		
						m_dev_vctr.push_back(devName);
//						std::cout << "Added " << devName << " in frameOfDevices_vctr, vector size: "<< frameOfDevices_vctr.size() << '\n';
						emit sig_gotNewDevice(devName);
					}
				}
			}
			
			//search for equal fields in m_dev_map
			//store if there's something new
			//if new - emit signal to add device in widget
			//if initiated recording from one of provided devices, connect_(m_dev_map->second for the whole range
			//make a new vector of device names, initialize it
			//compare it with multimap m_dev_map

		}
//		std::cout << "Iteration started, vector size: " << frameOfDevices_vctr.size() << " m_vector size: " << m_dev_vctr.size() << '\n';

		for(std::size_t it = 0; it < m_dev_vctr.size(); ++it)
		{
			bool deadItem_f = 1;
			for(std::size_t i = 0; i < frameOfDevices_vctr.size(); ++i)
			{
//				std::cout << m_dev_vctr[it] << " ?? " << frameOfDevices_vctr[i];
				if(m_dev_vctr[it] == frameOfDevices_vctr[i])
				{
					deadItem_f = 0;
//					std::cout << " found" << '\n';
					break;
					
				}
				std::cout << '\n';
			}
			if(deadItem_f)
			{
				m_dev_map.erase(m_dev_vctr[it]);
				emit sig_removeItem(m_dev_vctr[it]);
				m_dev_vctr.erase(m_dev_vctr.begin() + it);

			}

		}
		
//db		std::cout << "connection initiation: " << '\n';
//		std::string dev{"Conexant VIDEO GRABBER"};
//		std::string dev{"USB2.0 PC CAMERA: USB2.0 PC CAM"};
//		std::string dev{"HD WebCam: HD WebCam"};
		std::string dev = m_activeDev_str;
		int cntr = 0;
	if(!readState || m_readState_f)
	{
		m_readState_f = 0;
		auto range = m_dev_map.equal_range(dev);
		for(auto it = range.first; it != range.second; ++it)
		{
			if(connect_(it->second))
			{
				std::cout << "connected to " << it->second << '\n';
				break;
			}
//			else	std::cout << "Not connected to " << it->second << '\n';
			++cntr;
		}
	}
//db	if(cntr == 0)	std::cout << "there is no matches" << '\n';
//	}
/*	if(!readState) 
        {
//		std::cout << "Video camera is disconnected" << std::endl;
		if(std::filesystem::exists("/dev/video0"))
		{
			std::cout << "Video2 found" << std::endl;
			connect_(2);
		}
		else if(std::filesystem::exists("/dev/video1"))
		{
			std::cout << "Video3 found" << std::endl;
			connect_(3);
		}
		return;
	}*/
//	cv::imshow("video", m_frame_cv);
	std::string path = m_dir + std::to_string(m_index) + ".jpg";
//	cv::imwrite(path, m_frame_cv);
	if(m_write_f)	m_videoWriter.write(m_frame_cv);
        m_index++;
	QImage img(m_frame_cv.data, m_frame_cv.cols, m_frame_cv.rows, m_frame_cv.step, QImage::Format_BGR888);
	QImage resizedImg;
	if(!img.isNull())	resizedImg = img.scaled(m_widgetWidth_d, m_widgetHeight_d, Qt::KeepAspectRatio);//?
	m_videoLabel_ptr->setPixmap(QPixmap::fromImage(resizedImg));
	m_videoLabel_ptr->adjustSize();
	if(m_width_d != m_widthP_d || m_height_d != m_heightP_d)
	{
		emit sig_resize(m_width_d, m_height_d);
		m_heightP_d = m_height_d;
		m_widthP_d = m_width_d;
	}
	QCoreApplication::processEvents();//
	emit sig_stream();
}
