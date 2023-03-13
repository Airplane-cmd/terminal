#ifndef UDPHOLDER_H
#define UDPHOLDER_H

#include <QObject>
#include <QUdpSocket>
#include <QByteArray>

#include <vector>
#include <string>
#include <cstring>
#include <cstddef>

class UdpHolder : public QObject
{
Q_OBJECT
public:
	UdpHolder(QObject *parent = 0);
	virtual ~UdpHolder();

signals:
	void dataReceived(std::array<float, 13>, std::vector<std::vector<bool>>);
private slots:
	void readPendingDatagrams();
	void writePendingDatagram();
public slots:
	void setValueInDatagram(const QByteArray &, uint8_t);
	 
private:
	QUdpSocket *socket;
	QByteArray datagram;

	QUdpSocket *socket_T;
	
	QHostAddress sender;
	quint16 senderPort;
	
//	QHostAddress receiver;
	quint16 receiverPort;

	uint16_t calculateCRC(const QByteArray &);

	void getPID(const QByteArray &arr);
	void getStat(const QByteArray &arr);
	void getLeaks(const QByteArray &arr);
	void getErrors(const QByteArray &arr);
	float getFloat(const QByteArray &arr, const std::string &arg);
	std::vector<bool> getInt(const QByteArray &arr, const std::string &arg);

	void printTelemetry(const QByteArray &buffer);
	void printDatagram(const QByteArray &);
};
#endif
