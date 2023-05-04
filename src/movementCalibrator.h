#ifndef MCALIBRATOR_H
#define MCALIBRATOR_H
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QSlider>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QTextEdit>
#include <QSpinBox>
#include <QLayoutItem>

#include <array>
#include <vector>
#include <string>
class MCalibrator : public QWidget
{
Q_OBJECT
public:
	MCalibrator(QWidget *parent = 0);
	~MCalibrator();
private:
	uint m_configMaxIndex;
	uint m_configCurrent;
	std::string m_dir;
	std::string m_dirDest;
	QWidget *m_widget_ptr;
	QPushButton *m_burnValueBttn_ptr;
	QPushButton *m_captureBttn_ptr;//for enabling only calibration movement mode
	QPushButton *m_setBttn_ptr;//for setting temporal set of numbers 
	QPushButton *m_resetBttn_ptr;//erase config
	QPushButton *m_newConfigBttn_ptr;
	std::array<QPushButton *, 5> m_bttns_ptr_arr;//init

	std::array<QSpinBox *, 4> m_nums_ptr_arr;
	std::array<QLabel *, 4> m_labels_ptr_arr;
	std::array<QSlider *, 4> m_sliders_ptr_arr;//init

	QComboBox *m_axisQcb_ptr;
	QComboBox *m_fileQcb_ptr;
	std::array<QComboBox *, 2> m_qcb_ptr_arr;
	std::array<std::array<std::array<int, 4>, 201>, 4> m_movementDataRAM_arr;
	std::array<bool, 202> m_changedStructureElements;
	QTextEdit *m_viewer_ptr;
//	QLayoutItem *m_qli_ptr;
	std::array<QHBoxLayout *, 6> m_hbox_ptr_arr;
	std::array<QVBoxLayout *, 2> m_vbox_ptr_arr;//init
//	QGridLayout *m_grid_ptr;
	QHBoxLayout *m_hbox_ptr;

	std::array<char, 4> m_axisNames_arr;

	void m_setConfigMaxIndex();

	void m_setConfigs();
	void m_checkFolders();
	void m_createConfig();
	void m_initNums();
	
	std::string m_getFileData(const std::string &, const char);
	std::string m_getCurrentConfigDir();
	char m_getCurrentAxis();
	void m_initStructure();
private slots:
//	void s_processSet();
	void s_processNew();
	void s_openConfig(const std::string &, const char);

	void s_processQcbChange(int);
//	void s_storeValues();
	
//	void s_selectAxis();
signals:
	void sig_setMovement(int8_t, int8_t, int8_t, int8_t);
	void sig_openConfig(const std::string &, const char);


	
};
#endif
//qcombobox for axises
//qcombobox for files
//aproximate on s_processSet
//in file '$' means manually added value
//add qlabel with config number in m_qhb_ptr_arr[0] 
