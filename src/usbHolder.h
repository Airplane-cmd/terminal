#ifndef USBHOLDER_H
#define USBHOLDER_H

#include <QObject>
#include <QTimer>
#include <libusb-1.0/libusb.h>
#include <QByteArray>

class USBHolder : public QObject
{
Q_OBJECT

public:
	explicit USBHolder(QObject *parent = 0);
	~USBHolder();

private:
	libusb_device_handle* m_device;
	QTimer* m_timer;

	bool openDevice();
	void closeDevice();
private slots:
    	void readJoystickData();

signals:
    void joystickData(const QByteArray &, uint8_t);
};

#endif 
