#include "snaqe.h"
#include <QtWidgets/QApplication>
#include <QtPlugin>

#ifdef QT_NO_DEBUG
Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin);
#endif

extern "C"
{
	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Snaqe w;
	w.show();
	return a.exec();
}
