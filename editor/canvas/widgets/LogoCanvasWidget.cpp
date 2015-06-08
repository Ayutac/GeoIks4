//---------------------------------------------------------------------------
#include "precompiledHeaders.h"
//---------------------------------------------------------------------------
#include "LogoCanvasWidget.h"
//---------------------------------------------------------------------------





LogoCanvasWidget::LogoCanvasWidget(GeoXCanvas *canvas, QWidget *parent, Qt::WindowFlags f)
   : GeoXCanvasWidget(canvas, parent, f)
{
   ui.setupUi(this);
   ui.textEditCredits->setVisible(false);
}

LogoCanvasWidget::~LogoCanvasWidget() {}
