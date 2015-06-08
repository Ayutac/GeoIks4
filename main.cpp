#include "precompiledHeaders.h"

#include "ObjectEditor4.h"
#include "ExceptionHandling.h"
#include "LogoCanvas.h"
#include "GeoXOutput.h"
#include "InitGeoX.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	setStructureExceptionHandler();

	QApplication qapp(argc, argv);

	init();

	//QApplication::setStyle(new QCleanlooksStyle);
	//QApplication::setStyle(new QPlastiqueStyle);

	// create main window
   ObjectEditor4 *mainWindow = new ObjectEditor4;
	mainWindow->show();
   //mainWindow->setDisplayWidget(new LogoViewer());
	qapp.setActiveWindow( mainWindow );

	//Close all windows with main window
	QApplication::connect(mainWindow, SIGNAL(destroyed()), &qapp, SLOT(closeAllWindows()));

	output << "GeoX started.\n";

	// run it
	int result = qapp.exec();

	shutdown();

	return result;


}
