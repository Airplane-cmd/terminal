#ifndef BURNINATOR_H
#define BURNINATOR_H
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include <array>
class BurnInator : public QWidget
{
Q_OBJECT
public:
	BurnInator(QWidget *parent = 0);
	~BurnInator();
private:
	QWidget *m_widget_ptr;
	QPushButton *m_burnBttn_ptr;
	QPushButton *m_setBttn_ptr;
	QPushButton *m_rebootBttn_ptr;
	QComboBox *m_pidQcb_ptr;
	QVBoxLayout *m_vbox_ptr;
	QHBoxLayout *m_hbox_ptr;
	std::array<QLineEdit *, 3> m_qle_arr;


	
};
#endif
//mainWindow widget should remember PID even if widget was closed
//main layout is vertical
//
