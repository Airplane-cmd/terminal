#pragma once
#include <QWidget>
#include <QGridLayout>
#include <QPushButton>

#include <vector>
class Algorithms : public QWidget
{
public:
	Algorithms(QWidget *parent = 0);
private:
	std::vector<QPushButton *> bttns;
	void initButtons();
	QGridLayout *grid;

};
