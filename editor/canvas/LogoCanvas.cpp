#include "precompiledHeaders.h"
#include "LogoCanvas.h"
#include "LogoCanvasWidget.h"

IMPLEMENT_GEOX_CLASS(LogoCanvas, 0) {
   BEGIN_CLASS_INIT(LogoCanvas)
}

GeoXCanvasWidget *LogoCanvas::createWidget(QWidget *parent, Qt::WindowFlags f)
{
   return new LogoCanvasWidget(this, parent, f);
}

LogoCanvas::LogoCanvas() {}

LogoCanvas::~LogoCanvas() {}


