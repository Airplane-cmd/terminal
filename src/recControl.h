#ifndef RECCONTROL_H
#define RECCONTROL_H

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QListWidget>

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
	QVBoxLayout *m_vbox_ptr;

	QListWidget *m_devList_ptr;
	int m_row = 0;

	std::vector<QListWidgetItem *> m_devices_vctr; 

private slots:
	void s_emitStart();
	void s_emitPause();
	void s_emitStop();
	
	void s_emitConnection(QListWidgetItem *);
public slots:
	void s_gotNewDev(const std::string &);
signals:
	void sig_startRec();
	void sig_pauseRec();
	void sig_stopRec();

	void sig_connect(const std::string &);
};
#endif
