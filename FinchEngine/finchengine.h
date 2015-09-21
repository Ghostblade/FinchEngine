#ifndef FINCHENGINE_H
#define FINCHENGINE_H

#include <QtWidgets/QMainWindow>
#include "ui_finchengine.h"

class FinchEngine : public QMainWindow
{
	Q_OBJECT

public:
	FinchEngine(QWidget *parent = 0);
	~FinchEngine();

private:
	Ui::FinchEngineClass ui;
};

#endif // FINCHENGINE_H
