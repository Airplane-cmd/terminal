#ifndef RECCONTROL_H
#define RECCONTROL_H

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>

class RecControl : public QWidget
{
Q_OBJECT
public:
	RecControl(QWidget* parent = 0);
	~RecControl();
private:
	QPushButton *m_startBtn_ptr;
	QPushButton *m_pauseBtn_ptr;
	QPushButton *m_stopBtn_ptr;
	QHBoxLayout *m_hbox_ptr;
private slots:
	void s_emitStart();
	void s_emitPause();
	void s_emitStop();
signals:
	void sig_startRec();
	void sig_pauseRec();
	void sig_stopRec();
};
#endif
