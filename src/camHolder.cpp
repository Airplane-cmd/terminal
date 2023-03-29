#include <QCoreApplication>


#include "opencv2/opencv.hpp"
#include <iostream>
#include <chrono>
#include <filesystem>
#include <ctime>

//#include <thread>
//#include <chrono>
//#include <future>

#include "camHolder.h"
CamHolder::CamHolder(QWidget *parent) : QWidget(parent)
{
	m_index = 0;
	m_videoLabel_ptr = std::make_shared<QLabel>(this);
//	m_videoLabel_ptr->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	m_videoLabel_ptr->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	m_timer_ptr = new QTimer(this);
//	m_timer_ptr = std::make_shared<QTimer>(this);
//	m_streaming_b = 1;
    	connect(m_timer_ptr, &QTimer::timeout, this, &CamHolder::stream);
	m_timer_ptr->start(1000/100);
//	double dWidth = m_capture.get(cv::CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
//	double dHeight = m_capture.get(cv::CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video
//    	std::cout << "camera width = " << dWidth << ", height = " << dHeight << std::endl;
//	if (!m_capture.isOpened()) 	std::cout << "cannot open camera" << std::endl;
	m_dir = "~/WS/qt/USBCamera/logs";//argv[1];
}
void CamHolder::connect_(int cam)
{
	std::string camPath = "/dev/video" + std::to_string(cam);
	std::cout << "connecting..." << std::endl;
	m_capture.open(camPath);
	m_capture.set(cv::CAP_PROP_FRAME_WIDTH, 1920);
	m_capture.set(cv::CAP_PROP_FRAME_HEIGHT, 1080);
}
void CamHolder::stream()
{
//	int m_index = 0;
	bool readState = 0;

//	cv::VideoCapture capture;
//    	while(m_streaming_b)
//   	{
//		auto future = std::async(std::launch::async, [&]()
//		{
	readState = m_capture.read(m_frame_cv); // read a new frame from video 
//		});
//		while(future.wait_for(std::chrono::milliseconds(10)) != std::future_status::ready)
//		{
//			QCoreApplication::processEvents();
//		}
//		std::cout << "State: " << readState << std::endl;
	if(!readState) 
        {
		std::cout << "Video camera is disconnected" << std::endl;
		if(std::filesystem::exists("/dev/video2"))
		{
			std::cout << "Video2 found" << std::endl;
			connect_(0);
		}
		else if(std::filesystem::exists("/dev/video3"))
		{
			std::cout << "Video3 found" << std::endl;
			connect_(1);
		}
		return;
	}
//	cv::imshow("video", m_frame_cv);
	std::string path = m_dir + std::to_string(m_index) + ".jpg";
//	cv::imwrite(path, m_frame_cv);
        m_index++;
	QImage img(m_frame_cv.data, m_frame_cv.cols, m_frame_cv.rows, m_frame_cv.step, QImage::Format_BGR888);
	m_videoLabel_ptr->setPixmap(QPixmap::fromImage(img));
	m_videoLabel_ptr->adjustSize();
}
 
//	        if (cv::waitKey(10) == 27)
//	      	{
//			std::cout << "Esc key is pressed by user. Stoppig the video" << std::endl;
//			
//	 		break;
//        	}

void CamHolder::stop()
{
	m_streaming_b = 0;
}

