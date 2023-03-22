#ifndef USBHOLDER_H
#define USBHOLDER_H

#include <QObject>
#include <QTimer>
#include <QByteArray>

#include <atomic>
#include <thread>

#include <libusb-1.0/libusb.h>
class USBHolder : public QObject
{
Q_OBJECT

public:
	explicit USBHolder(QObject *parent = 0);
	~USBHolder();

private:
	uint8_t m_powerLimit;
	libusb_device_handle* m_device;
	QTimer* m_timer;
	QByteArray m_data;
	unsigned char data_ch[22];

	void printRawData();
	void printControlData();

	bool openDevice();
	void closeDevice();
	void readUSBData();

	std::atomic<bool> m_stopThread{0};
	std::thread *m_thread;
private slots:
    	void readJoystickData();
public slots:
	void setPowerLimit(uint8_t);

signals:
    void joystickData(const QByteArray &, uint8_t);
};

#endif 
