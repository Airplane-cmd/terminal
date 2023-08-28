#ifndef USBHOLDER_H
#define USBHOLDER_H

#include <QObject>
#include <QTimer>
#include <QByteArray>
#include <QThread>
#include <QGamepad>

#include <memory>
#include <array>

#include <libusb-1.0/libusb.h>
class USBHolder : public QObject
{
Q_OBJECT

public:
	explicit USBHolder(QObject *parent = 0);
	~USBHolder();

private:
	std::shared_ptr<QGamepad> m_gamepad_ptr;
	uint8_t m_powerLimit = 100;
	std::array<int8_t, 2> m_camerasPositions_arr;
	QByteArray m_data;
	QByteArray m_perefData;
	void printControlData();
	
private slots:
    	void readJoystickData();
	void s_openDevice();
	void s_processEvents();
public slots:
	void setPowerLimit(uint8_t);
	void s_setCamerasPositions(const std::array<int8_t, 2> &);
signals:
	void joystickData(const QByteArray &, uint8_t);
	void sig_camerasPositions(const std::array<int8_t, 2> &);
	void sig_setPowerLimit(uint8_t);
};

#endif 
