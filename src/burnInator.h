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
#include <string>
class BurnInator : public QWidget
{
Q_OBJECT
public:
	BurnInator(QWidget *parent = 0);
	~BurnInator();
private:
	std::string m_dir = "../settings";
	std::string m_filename = "/PID.txt";
	std::string m_pidName;
	uint8_t m_pidIndex;
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
	std::vector<std::pair<std::array<float, 3>, bool>> m_temporalValues_obj_vctr;
//	bool apply_f;
	void m_setupQle();
	void m_getConfigData();
	void m_setConfigData();
	void m_setTempObj();
private slots:
//	s_setDepthPdi();
//	s_setYawPdi();
	void s_processApply();
	void s_burnNumbers();
	void s_rebootBoard();
	void s_setupQle(QString);
signals:
	void sig_setDepthPdi(std::array<float, 3> &);
	void sig_setYawPdi(std::array<float, 3> &);
	void sig_setRollPdi(std::array<float, 3> &);
	void sig_setPitchPdi(std::array<float, 3> &);

	void sig_rebootBoard();
	void sig_burnNumbers();


	
};
#endif
//mainWindow widget should remember PID even if widget was closed
//main layout is vertical
//
