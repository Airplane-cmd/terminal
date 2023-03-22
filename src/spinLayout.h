#ifndef SPINLAYOUT_H_
#define SPINLAYOUT_H_


#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QString>
#include <QDoubleSpinBox>

class SpinLayout : public QWidget
{
Q_OBJECT
public:
	SpinLayout(QString, float beginRange = 0, float endRange = 100, float startValue = 0, QWidget *parent = 0);
	~SpinLayout();
private:
	bool m_state_b;
	bool m_state_d;
	float m_beginRange;
	float m_endRange;
	float m_startValue;
	QPushButton *m_button_ptr;
	QPushButton *m_getBttn_ptr;
	QDoubleSpinBox *m_dsb_ptr;
	void setValue(float);
	float getValue();
private slots:
	void emitSignalFromButton();
signals:
	void pressedButton(bool);
};

#endif
