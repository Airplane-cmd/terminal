#ifndef CAMHOLDER_H
#define CAMHOLDER_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QTimer>

#include <memory>
#include <string>

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
	bool m_streaming_b = 0;
	cv::VideoCapture m_capture;
	cv::Mat m_frame_cv;
	std::string m_dir;
public:
	void connect_(int cam = 3);
private slots:
	void stream();
	void stop();


};
#endif
