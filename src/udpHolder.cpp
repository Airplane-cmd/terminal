#include <QTimer>
#include "udpHolder.h"

UdpHolder::UdpHolder(QObject *parent) : QObject{parent}
{
	socket = new QUdpSocket(this);
	socket->setSocketOption(QAbstractSocket::LowDelayOption, 1);
	socket->setReadBufferSize(0);
//	datagram = new QByteArray;
	datagram.resize(40);
	QTimer *timer = new QTimer(this);
	socket->bind(QHostAddress::Any, 2065);
	connect(socket, &QUdpSocket::readyRead, this, &UdpHolder::readPendingDatagrams);
//	connect(timer, &QTimer::timeout, this, &UdpHolder::writePendingDatagram);

	timer->start(100);
		
}
UdpHolder::~UdpHolder()
{
	delete socket;
}
uint16_t UdpHolder::calculateCRC(const QByteArray &dataR)
{
	QByteArray data;
	data.resize(dataR.size() - 2);
	for(int i = 0; i < data.size(); ++i)	data[i] = dataR[i];
	uint16_t crc = 0xFFFF;
	for (int i = 0; i < data.size(); ++i)
	{
//        	uint8_t byte = data[i];
        	crc ^= data[i];
        	for (int j = 0; j < 8; ++j)
		{
        		uint16_t mask = -(crc & 1);
            		crc = (crc >> 1) ^ (0xA001 & mask);
        	}
	}
    	return crc;
}

void UdpHolder::readPendingDatagrams()
{
	while(socket->hasPendingDatagrams())
        {
		QTextStream out(stdout);
		QByteArray buffer;
        	buffer.resize(socket->pendingDatagramSize());
		socket->readDatagram(buffer.data(), buffer.size(), &sender, &senderPort);

		std::array<float, 13> floats;
		floats[0] = getFloat(buffer, "r");
                floats[1] = getFloat(buffer, "pt");
                floats[2] = getFloat(buffer, "y");
        	floats[3] = getFloat(buffer, "g"); 
                floats[4] = getFloat(buffer, "a"); 
                floats[5] = getFloat(buffer, "d");  
                floats[6] = getFloat(buffer, "vl"); 
                floats[7] = getFloat(buffer, "c");   
                floats[8] = getFloat(buffer, "v_x"); 
                floats[9] = getFloat(buffer, "v_y"); 
                floats[10] = getFloat(buffer, "ps_x");
                floats[11] = getFloat(buffer, "ps_y");
                floats[12] = getFloat(buffer, "t");

 		std::vector<std::vector<bool>> cntnr;
		cntnr.push_back(getInt(buffer, "pid"));
		cntnr.push_back(getInt(buffer, "stat"));
		cntnr.push_back(getInt(buffer, "leak"));	
		cntnr.push_back(getInt(buffer, "err"));

//		printTelemetry(buffer);
		printDatagram(buffer);
		uint16_t crc = calculateCRC(buffer);
//		qDebug() << static_cast<uint16_t>(buffer.right(2).toUInt()) << " ? " << crc << Qt::endl;
		emit dataReceived(floats, cntnr);
        }
}
void UdpHolder::writePendingDatagram()
{
	datagram[0] = 0;
	datagram[1] = 230;
	uint16_t crcR = calculateCRC(datagram);
	uint16_t crc = ((crcR >> 8) & 0x00FF) | ((crcR << 8) & 0xFF00);
	std::memcpy(datagram.data() + 38, &crc, 2);
	socket->writeDatagram(datagram.data(), datagram.size(), sender, senderPort);
}
void UdpHolder::printTelemetry(const QByteArray &buffer)
{

	QTextStream out(stdout);
	std::system("clear");
	out << "Roll:        " << getFloat(buffer, "r") << Qt::endl; 
	out << "Pitch:       " << getFloat(buffer, "pt") << Qt::endl;
	out << "Yaw:         " << getFloat(buffer, "y") << Qt::endl;
	out << "Gyroscope:   " << getFloat(buffer, "g") << Qt::endl; 
	out << "Altitude:    " << getFloat(buffer, "a") << Qt::endl; 
	out << "Depth:       " << getFloat(buffer, "d") << Qt::endl;  
        out << "Voltage:     " << getFloat(buffer, "vl") << Qt::endl; 
        out << "Current:     " << getFloat(buffer, "c") << Qt::endl;   
        out << "Velocity_X:  " << getFloat(buffer, "v_x") << Qt::endl; 
        out << "Velocity_Y:  " << getFloat(buffer, "v_y") << Qt::endl; 
        out << "Pos_X:       " << getFloat(buffer, "ps_x") << Qt::endl;
        out << "Pos_Y:       " << getFloat(buffer, "ps_y") << Qt::endl;
        out << "Temperature: " << getFloat(buffer, "t") << Qt::endl;
	getPID(buffer);   
        getStat(buffer);  
	getLeaks(buffer); 
	getErrors(buffer);
}
void UdpHolder::printDatagram(const QByteArray &buffer)
{
	for(int i = 0; i < buffer.size(); ++i)
	{
//              	QString msg = QString::toHex(buffer[i]);
		qDebug() << i << ": 0x" << Qt::hex << static_cast<uint8_t>(buffer[i]) << Qt::dec << Qt::endl;
	}         
}
float UdpHolder::getFloat(const QByteArray &arr, const std::string &arg)
{
        QByteArray buf;
        int start = 0;
        if(arg == "r")          start = 2;
        else if(arg == "d")     start = 18;
        else if(arg == "pt")    start = 6;
        else if(arg == "y")     start = 10;
        else if(arg == "g")     start = 14;
        else if(arg == "a")     start = 22;
        else if(arg == "v_x")   start = 26;
        else if(arg == "v_y")   start = 30;
        else if(arg == "ps_x")  start = 34;
        else if(arg == "ps_y")  start = 38;
        else if(arg == "vl")    start = 42;
        else if(arg == "c")     start = 46;
        else if(arg == "t")     start = 54;
        else return -1;
        buf.resize(4);
        for(int i = start; i < start + 4; ++i)  buf[3 - i + start] = arr[i];
        char bytes[4];
        std::memcpy(bytes, buf.constData(), 4);
        float float_;
        std::memcpy(&float_, bytes, sizeof(float_));
        return float_;
}
std::vector<bool> UdpHolder::getInt(const QByteArray &arr, const std::string &arg)
{
        std::vector<bool> bits;
        uint8_t index = 0;
        uint8_t size = 0;
        if(arg == "pid")
        {
                index = 50;
                size = 7;
        }
        else if(arg == "stat")
        {
                index = 51;
                size = 2;
        }
        else if(arg == "leak")
        {
                index = 52;
                size = 2;
        }
        else if(arg == "err")
        {
                index = 53;
                size = 2;
        }
        std::byte byte_;
        std::memcpy(&byte_, arr.constData() + index, 1);
        for(int i = 0; i < size; ++i)
        {
                bits.push_back((byte_ & (std::byte(1) << i)) != std::byte(0));
        }
        return bits;
}
void UdpHolder::getPID(const QByteArray &arr)
{
        std::vector<bool> vctr = getInt(arr, "pid");
        QTextStream out(stdout);
        out << " PIDs:" << Qt::endl;
        out << "       Roll:      " << ((vctr[0]) ? "activated\n" : "not activated\n");
        out << "       Pitch:     " << ((vctr[1]) ? "activated\n" : "not activated\n");
        out << "       Depth:     " << ((vctr[2]) ? "activated\n" : "not activated\n");
        out << "       Altitude:  " << ((vctr[3]) ? "activated\n" : "not activated\n");
        out << "       Yaw:       " << ((vctr[4]) ? "activated\n" : "not activated\n");
        out << "       VelocityX: " << ((vctr[5]) ? "activated\n" : "not activated\n");
        out << "       VelocityY: " << ((vctr[6]) ? "activated\n" : "not activated\n");
}
void UdpHolder::getStat(const QByteArray &arr)
{
        std::vector<bool> vctr = getInt(arr, "stat");
        QTextStream out(stdout);
        out << " Stat: " << Qt::endl;
        out << "      Magnet1: " << ((vctr[0]) ? "activated\n" : "not activated\n");
        out << "      Magnet2: " << ((vctr[1]) ? "activated\n" : "not activated\n");
}
void UdpHolder::getLeaks(const QByteArray &arr)
{
        std::vector<bool> vctr = getInt(arr, "leak");
        QTextStream out(stdout);
        out << " Leaks: " << Qt::endl;
        out << "       Main Module:        " << ((vctr[0]) ? "detected\n" : "not detected\n");
        out << "       Orientation module: " << ((vctr[1]) ? "detected\n" : "not detected\n");
}
void UdpHolder::getErrors(const QByteArray &arr)
{
        std::vector<bool> vctr = getInt(arr, "err");
        QTextStream out(stdout);
        out << " Errors: " << Qt::endl;
        out << "       Pressure unit:    " << ((vctr[0]) ? "occured\n" : "not occured\n");
        out << "       Orientation unit: " << ((vctr[1]) ? "occured\n" : "not occured\n");
}


