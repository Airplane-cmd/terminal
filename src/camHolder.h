#ifndef CAMHOLDER_H
#define CAMHOLDER_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QTimer>

#include <memory>
#include <string>
#include <map>

#include "opencv2/opencv.hpp"
class CamHolder : public QWidget
{
Q_OBJECT
public:
	explicit CamHolder(QWidget *parent = 0);
//	~CamHolder();
//	void stream();

private:
	std::shared_ptr<QLabel> m_videoLabel_ptr;
	QTimer *m_timer_ptr;//std::shared_ptr<QTimer> m_timer_ptr;
	int m_index = 0;

	bool m_write_f = 0;
	bool m_writeInit_f = 0;
	bool m_readState_f = 0;
	cv::VideoCapture m_capture;
	cv::Mat m_frame_cv;
	std::string m_dir;
	void findPath();
	void initRec();
	std::string getDevInfo(const std::string &path);
	double m_height_d;
	double m_width_d;
	cv::VideoWriter m_videoWriter;
	double m_framerate_d;

	std::string m_activeDev_str;
	
	std::multimap<std::string, int> m_dev_map;
public:
	bool connect_(int cam = 1);
public slots:
	void stream();
	void s_stopRec();
	void s_pauseRec();
	void s_startRec();
	void s_setWriteFlag(bool);
	
	void s_setDev(const std::string &);

signals:
	void sig_gotNewDevice(const std::string &);



};
#endif
