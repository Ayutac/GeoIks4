#include "precompiledHeaders.h"
#include "GeoXCanvasWidget.h"
#include "GeoXCanvas.h"

GeoXCanvasWidget::GeoXCanvasWidget(GeoXCanvas *canvas, QWidget *parent, Qt::WindowFlags f) : QWidget(parent, f)
{
   deleting = false;
   this->canvas = canvas;
}

GeoXCanvasWidget::~GeoXCanvasWidget()
{
   deleting = true;
   if (!canvas->deleting) delete canvas;
}

