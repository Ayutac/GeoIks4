#include "precompiledHeaders.h"
#include "MDICanvasContainer.h"
#include "GeoXCanvasWidget.h"

#include <QVBoxLayout>
#include "GeoXCanvas.h"


void MDICanvasContainer::closeEvent(QCloseEvent *event)
{
   event->accept();
   emit canvasClosed(name.c_str(), this);
}

MDICanvasContainer::MDICanvasContainer(QWidget *parent, Qt::WindowFlags f) :
QMdiSubWindow(parent, f) {
   canvasWidget = NULL;
}

void MDICanvasContainer::setup(GeoXCanvasWidget *canvasWidget, string name, string windowTitle)
{
   this->name = name;
   this->canvasWidget = canvasWidget;
   setWidget(canvasWidget);
   resize(400, 300);
   setWindowTitle(windowTitle.c_str());
   QWidget::connect(canvasWidget, SIGNAL(destroyed()), this, SLOT(deleteLater()));
}



