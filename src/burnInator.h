#ifndef BURNINATOR_H
#define BURNINATOR_H
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>

#include <array>
#include <vector>
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
	std::vector<QHBoxLayout *> m_hbox_ptr_vctr;
	QGridLayout *m_grid_ptr;
	QVBoxLayout *m_vbox_ptr;
	std::array<QLineEdit *, 3> m_qle_arr;
	std::array<QLabel *, 3> m_labels_arr;
private slots:
//	s_setDepthPdi();
//	s_setYawPdi();
	void s_processApply();
	void s_burnNumbers();
	void s_rebootBoard();
signals:
	void sig_setDepthPdi(std::array<float, 3> &);
	void sig_setYawPdi(std::array<float, 3> &);
	void sig_rebootBoard();
	void sig_burnNumbers();


	
};
#endif
//mainWindow widget should remember PID even if widget was closed
//main layout is vertical
//
