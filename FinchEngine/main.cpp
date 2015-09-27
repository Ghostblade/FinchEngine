#include "finchengine.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	FinchEngine w;
	w.show();

	return a.exec();
}
