#include <QWidget>
#include <QGridLayout>
#include <QAction>
#include <QTextStream>

#include <QAction>

#include "algorithms.h"

Algorithms::Algorithms(QWidget *parent) : QWidget{parent}
{
	grid = new QGridLayout();
	initButtons();
	int width(0);
	int col(0), row(0);
	for(int i = 0; i < bttns.size(); ++i)
	{
		width += bttns[i]->size().width();
		if(width > 400)
		{
			col = 0;
			++row;
			width = 0;
		}
		else	col++;
		bttns[i]->setCheckable(1);
		bttns[i]->setChecked(0);
		grid->addWidget(bttns[i], row, col);
	}

	connect(bttns[0], &QPushButton::clicked, this, &Algorithms::depthControl);
	
	QWidget *cntnr = new QWidget(this);
	cntnr->setLayout(grid);
	cntnr->setMaximumWidth(410);
}
void Algorithms::initButtons()
{
	bttns.push_back(new QPushButton{"Depth", this});
	bttns.push_back(new QPushButton{"Depth", this});
	bttns.push_back(new QPushButton{"Depth", this});
	bttns.push_back(new QPushButton{"Depth", this});
}
void Algorithms::depthControl()
{
	QTextStream out{stdout};
	if(bttns[0]->isChecked())	out << "Depth control activated" << Qt::endl;
	else				out << "Depth control disactivated" << Qt::endl;
}


