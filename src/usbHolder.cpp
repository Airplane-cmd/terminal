#include <QDebug>

#include "usbHolder.h"

#define JOYSTICK_VENDOR_ID   0x44f
#define JOYSTICK_PRODUCT_ID  0xb108

USBHolder::USBHolder(QObject *parent) : QObject(parent)
{
    m_device = 0;
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &USBHolder::readJoystickData);

    if(openDevice())
    {
        m_timer->start(100);
	if(libusb_kernel_driver_active(m_device, 0) == 1)
        {
            int result = libusb_detach_kernel_driver(m_device, 0);
            if (result == LIBUSB_SUCCESS)
            {
                qDebug() << "Kernel driver detached successfully";
            }
            else
            {
                qDebug() << "Error detaching kernel driver:" << libusb_error_name(result) << Qt::endl;
            }
        }
	else	qDebug() << "Kerner driver isn't active;" << Qt::endl;
    }
}
USBHolder::~USBHolder()
{
    closeDevice();
}

void USBHolder::readJoystickData()
{
	QByteArray data_o;
	data_o.resize(1);
//	data[0] = 
	unsigned char data[22];
	int bytesTransferred = 0;
	int result = libusb_interrupt_transfer(m_device, 0x81, data, sizeof(data), &bytesTransferred, 0);
//	qDebug() << "libusb_interrupt_transter returns " << result << Qt::endl;	
//	qDebug() << "Size: " << bytesTransferred << Qt::endl;
	if (result == LIBUSB_SUCCESS && bytesTransferred == sizeof(data))
	{
		// Parse the joystick data from the USB packet
//		qDebug() << "here" << Qt::endl;
		for(int i = 0; i < 22; ++i)	qDebug() << i << ": " << data[i] << Qt::endl; 
        	data_o[0] = uint8_t(data[7] - 128);
	
//		qDebug() << x << " " << y << Qt::endl; 
        	emit joystickData(data_o, 5);
	}
	else
	{
		qDebug() << "Size: " << bytesTransferred << Qt::endl;
		qDebug() << libusb_error_name(result) << Qt::endl;
        	closeDevice();
	      	openDevice();
    	}
}
bool USBHolder::openDevice()
{
	libusb_context* context;
	libusb_device** list;
	libusb_device* device = 0;
	libusb_device_handle *handle = 0;
    	if(libusb_init(&context) != LIBUSB_SUCCESS)
    	{
        	return false;
    	}
    	int count = libusb_get_device_list(context, &list);
    	for(int i = 0; i < count; i++)
    	{
        	libusb_device_descriptor descriptor;
        	libusb_get_device_descriptor(list[i], &descriptor);
//		qDebug() << Qt::hex << i << " product id: 0x" << descriptor.idProduct << " vendor id: 0x" << descriptor.idVendor << Qt::endl;
        	if(descriptor.idVendor == JOYSTICK_VENDOR_ID && descriptor.idProduct == JOYSTICK_PRODUCT_ID)
		{
			qDebug() << "Joystick found:" << Qt::endl;
        		if(libusb_open(list[i], &m_device) == LIBUSB_SUCCESS)
			{
				qDebug() <<"Correct product id: " << descriptor.idProduct << Qt::endl;
				break;
			}
			else	qDebug() << "libusb_open != LIBUSB_SUCCESS" << Qt::endl;
		}
        
    	}
	libusb_free_device_list(list, 1);
    	if(m_device == 0)
	{
        	libusb_exit(context);
        	return false;
    	}
    	libusb_set_configuration(m_device, 1);
    	libusb_claim_interface(m_device, 0);
    	return true;
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


