#include <QDebug>
#include <QCoreApplication>

#include <thread>
#include <chrono>
#include <future>

#include <cmath>

#include "usbHolder.h"

#define JOYSTICK_VENDOR_ID   0x44f
#define JOYSTICK_PRODUCT_ID  0xb108

USBHolder::USBHolder(QObject *parent) : QObject(parent)
{
//	startJoystickThread();		
	m_camCount = 2;
	for(uint8_t i = 0; i < m_camCount; ++i)
	{
		m_camPosValues_vctr.push_back(0);
		m_previousCamPosValues_vctr.push_back(0);
	}
	m_powerLimit = 50;
	m_device = 0;
	m_camStep = 10;
	m_timer = new QTimer(this);
	m_timerOpen = new QTimer(this);
//	m_thread = new std::thread(&USBHolder::readJoystickData, this);
	connect(m_timer, &QTimer::timeout, this, &USBHolder::readJoystickData);
	connect(m_timerOpen, &QTimer::timeout, this, &USBHolder::s_openDevice);
	m_timerOpen->start(13);
//	if(openDevice())
//	{
//	      	m_timer->start(10);
		
//		if(libusb_kernel_driver_active(m_device, 0) == 1)
//	      	{
//        		int result = libusb_detach_kernel_driver(m_device, 0);
//	  		if (result == LIBUSB_SUCCESS)
//	      		{
//        			qDebug() << "Kernel driver detached successfully";
//            		}
//        		else	qDebug() << "Error detaching kernel driver:" << libusb_error_name(result) << Qt::endl;
//		}
//        }
	
}
USBHolder::~USBHolder()
{
	m_stopThread = 1;
	m_timer->stop();
//	m_thread->join();
	closeDevice();
//	delete m_device;
//	delete m_thread;
	delete m_timer;
	delete m_timerOpen;


}
void USBHolder::startJoystickThread()
{
//	m_joystickThread = new QThread();
//	moveToThread(m_joystickThread);
//	connect(m_joystickThread, &QThread::started, this, &USBHolder::readJoystickData);
//	m_joystickThread->start();
}
void USBHolder::setPowerLimit(uint8_t vl)
{
	if(vl <= 100)	m_powerLimit = vl;
}
void USBHolder::printRawData()
{
	for(int i = 0; i < 22; ++i)	qDebug() << i << ": " << data_ch[i] << Qt::endl;
	std::system("clear");
}
void USBHolder::printControlData()
{
	for(int i = 0; i < m_data.size(); ++i)	qDebug() << i << ": " << int8_t(m_data[i]) << Qt::endl;
}
void USBHolder::readJoystickData()
{

	m_data.resize(4);//
	data_new.resize(4);
	int bytesTransferred = 0;
	int result = 0;
//	result = libusb_interrupt_transfer(m_device, 0x81, data_ch, sizeof(data_ch), &bytesTransferred, 0);
//	while(!m_stopThread)
//	{
//		auto future = std::async(std::launch::async, [&]()
//		{
			result = libusb_interrupt_transfer(m_device, 0x81, data_ch, sizeof(data_ch), &bytesTransferred, 10);
//		});
//		while(future.wait_for(std::chrono::milliseconds(10)) != std::future_status::ready)
//		{
			QCoreApplication::processEvents();
//		}
//		qDebug() << "libusb_interrupt_transter returns " << result << Qt::endl;	
//		qDebug() << "Size: " << bytesTransferred << Qt::endl;
	if (result == LIBUSB_SUCCESS && bytesTransferred == sizeof(data_ch))
	{
		qDebug() << "here" << Qt::endl;
//			auto fj = {[&](){return int16_t(256 - data_ch[5] + 256 * (3 - data_ch[6]));}};
	
		m_data[0] = uint8_t(-float(m_powerLimit) / 100 * float(int16_t(256 - data_ch[5] + 256 * (3 - data_ch[6])) - 512) / 512 * 100);//thrusters control values in range -100:100
		m_data[1] = uint8_t(float(m_powerLimit) / 100 * float(int16_t(256 - data_ch[3] + 256 * (3 - data_ch[4])) - 512) / 512 * 100);
		m_data[2] = uint8_t(-0.7 * float(m_powerLimit) / 100 * float(data_ch[8] - 128) / 128 * 100);
		m_data[3] = uint8_t(-float(m_powerLimit) / 100 * float((256 - data_ch[7]) - 128) / 128 * 100);

//			data_new[0] = int8_t(float(data_ch[9] - 128) / 128 * 100);//hardware debug statements
//			data_new[1] = int8_t(float(data_ch[9] - 128) / 128 * 100);

		data_new[3] = (data_ch[9] < 100) ? 100 : (((data_ch[9]) > 156) ? -100 : 0);//int8_t(float(data_ch[9] - 128) / 128 * 100);//nt8_t(int(int8_t(data_ch[9]) - 128) / 128 * 100); //uint8_t(int(int8_t(data_ch[9]) - 128) / 128 * 100);
		data_new[2] = int8_t((data_ch[16] == 255) ? -100 : ((data_ch[17] == 255) ? 100 : 0));
		for(uint8_t i = 0; i < m_camCount; ++i)//cams control values
		{
			if(int8_t(m_camPosValues_vctr[i] + m_camStep) < 101)
			{
				m_previousCamPosValues_vctr[i] = m_camPosValues_vctr[i];
				m_camPosValues_vctr[i] += m_camStep * (uint8_t(data_ch[2 * i + 10]) / 255);
			}
			if(int8_t(m_camPosValues_vctr[i] - m_camStep) > -101)
			{
				m_previousCamPosValues_vctr[i] = m_camPosValues_vctr[i];
				m_camPosValues_vctr[i] -= m_camStep * (uint8_t(data_ch[2 * i + 11]) / 255);
			}
			if(m_camPosValues_vctr[i] != m_previousCamPosValues_vctr[i])	m_camerasPositionsChanged_f = 1;
			data_new[i] = m_camPosValues_vctr[i];		
			if(m_camerasPositionsChanged_f)
			{
				std::array<int8_t, 2> camerasPositions;
				for(uint8_t i = 0; i < 2; ++i)	camerasPositions[i] = m_camPosValues_vctr[i];
				emit sig_camerasPositions(camerasPositions);
				m_camerasPositionsChanged_f = 0;
			}
//			printRawData();
//			qDebug() << int8_t(data_new[0]) << " " << int8_t(data_new[1]) << '\n';//int8_t(float(data_ch[9] - 128) / 128 * 100) << '\n';
//			printControlData();
			QCoreApplication::processEvents();
			emit joystickData(m_data, 2);
			emit joystickData(data_new, 29);
			QCoreApplication::processEvents();
		}
    }	
	else
	{
//			qDebug() << "Size: " << bytesTransferred << Qt::endl;
//			qDebug() << libusb_error_name(result) << Qt::endl;
//       		closeDevice();
//		      	openDevice();
	}
	QTimer timer;
	connect(&timer, &QTimer::timeout, this, &USBHolder::s_processEvents);
	timer.start(10);
//		QCoreApplication::exec();
//	}
}
void USBHolder::s_processEvents()
{
	QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
void USBHolder::readUSBData()
{
	while(m_timer->isActive())
	{
		readJoystickData();
		QCoreApplication::processEvents();
	}

}
void USBHolder::s_openDevice()
{
	QCoreApplication::processEvents();

	libusb_context* context;
	libusb_device** list;
	libusb_device* device = 0;
	libusb_device_handle *handle = 0;
    	if(libusb_init(&context) != LIBUSB_SUCCESS)
    	{
        	return;// false;
    	}
    	int count = libusb_get_device_list(context, &list);
    	for(int i = 0; i < count; i++)
    	{
        	libusb_device_descriptor descriptor;
        	libusb_get_device_descriptor(list[i], &descriptor);
//		qDebug() << Qt::hex << i << " product id: 0x" << descriptor.idProduct << " vendor id: 0x" << descriptor.idVendor << Qt::endl;
        	if(descriptor.idVendor == JOYSTICK_VENDOR_ID && descriptor.idProduct == JOYSTICK_PRODUCT_ID)
		{
//			qDebug() << "Joystick found:" << Qt::endl;
        		if(libusb_open(list[i], &m_device) == LIBUSB_SUCCESS)
			{
				m_timerOpen->stop();
				m_timer->start(11);
//				m_state_f = 1;
//				qDebug() <<"Correct product id: " << descriptor.idProduct << Qt::endl;
				if(libusb_kernel_driver_active(m_device, 0) == 1)
        		{
        			int result = libusb_detach_kernel_driver(m_device, 0);
        			if(result == LIBUSB_SUCCESS)
        			{
        				qDebug() << "Kernel driver detached successfully";
            		}
        			else	qDebug() << "Error detaching kernel driver:" << libusb_error_name(result) << Qt::endl;
				}
				break;
			}
			else	
			{
//				qDebug() << "libusb_open != LIBUSB_SUCCESS" << Qt::endl;
				break;
			}
		}
        
    	}
//	qDebug() << "open device?\n";
	libusb_free_device_list(list, 1);
    	if(m_device == 0)
	{
        	libusb_exit(context);
        	return;// false;
    	}
    	libusb_set_configuration(m_device, 1);
    	libusb_claim_interface(m_device, 0);
    	return;// true;
}
void USBHolder::closeDevice()
{
    if (m_device != 0)
    {
        libusb_release_interface(m_device, 0);
        libusb_close(m_device);
        m_device = 0;
    }
}
void USBHolder::s_setCamerasPositions(const std::array<int8_t, 2> &camerasPositions)
{
	for(uint8_t i = 0; i < 2; ++i)
	{
		m_camPosValues_vctr[i] = camerasPositions[i];
		m_previousCamPosValues_vctr[i] = camerasPositions[i];
		qDebug() << "value set in usbHolder: " << camerasPositions[i] << '\n';
	}
}

