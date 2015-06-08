#include "precompiledHeaders.h"

#include "SingleWidgetCanvas.h"
#include "GeoXCanvasWidget.h"
#include "SingleWidgetView.h"
#include "NamedObjectList.h"

#include <QVBoxLayout>


IMPLEMENT_GEOX_CLASS(SingleWidgetCanvas, 0) {
	BEGIN_CLASS_INIT(SingleWidgetCanvas)
}


SingleWidgetCanvas::SingleWidgetCanvas()
{
	myWidget = NULL;
	layout = NULL;
}

SingleWidgetCanvas::~SingleWidgetCanvas() {}

GeoXCanvasWidget *SingleWidgetCanvas::createWidget(QWidget *parent, Qt::WindowFlags f)
{
	if (myWidget) {
		throw EInvalidState("SingleWidgetCanvas::createWidget() - cannot create multiple widgets");
	} else {
		GeoXCanvasWidget *w = new GeoXCanvasWidget(this, parent = 0, f);
		this->myWidget = w;
		return w;
	}

}

void SingleWidgetCanvas::addView(GeoXView *view, string &name)
{
	if (!myWidget) {
		throw EInvalidState("SingleWidgetCanvas::addView() -- called too early; the canvas' widget has not yet been created.");
	}
	if (views->getSize() > 0) {
		throw EInvalidState("SingleWidgetCanvas::addView() -- SingleWidgetCanvas can only display a single view (tried to add a second one).");
	}
	SingleWidgetView *sview = dynamic_cast<SingleWidgetView*>(view);
	if (!sview) {
		throw ETypeMismatch("SingleWidgetCanvas::addView() -- argument view must be of type SingleWidgetView.");
	}

	GeoXCanvas::addView(sview, name);
	layout = new QVBoxLayout(myWidget);
	layout->setSpacing(0);
	layout->setMargin(0);
	layout->insertWidget(0, sview->createWidget(myWidget, 0));
}

void SingleWidgetCanvas::deleteView(GeoXView *view)
{
	delete layout;
	layout = NULL;
	GeoXCanvas::deleteView(view);
}

void SingleWidgetCanvas::deleteView(string name)
{
	delete layout;
	layout = NULL;
	GeoXCanvas::deleteView(name);
}

