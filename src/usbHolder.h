#ifndef USBHOLDER_H
#define USBHOLDER_H

#include <QObject>
#include <QTimer>
#include <QByteArray>
#include <QThread>

#include <atomic>
#include <thread>
#include <array>

#include <libusb-1.0/libusb.h>
class USBHolder : public QObject
{
Q_OBJECT

public:
	explicit USBHolder(QObject *parent = 0);
	~USBHolder();

private:
	uint8_t m_camCount;
	bool m_camerasPositionsChanged_f;
	std::vector<uint8_t> m_camPosValues_vctr;
	std::vector<uint8_t> m_previousCamPosValues_vctr;
	uint8_t m_camStep;
	uint8_t m_powerLimit;
	uint32_t m_openDelay;
	libusb_device_handle* m_device;
	QTimer* m_timer;
	QTimer* m_timerOpen;
	QByteArray m_data;
	QByteArray data_new;
	unsigned char data_ch[22];
	bool m_state_f = 0;
	QThread *m_joystickThread;

	void printRawData();
	void printControlData();

	bool openDevice();
	void closeDevice();
	void readUSBData();
	void startJoystickThread();

	std::atomic<bool> m_stopThread{0};
	std::thread *m_thread;
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
