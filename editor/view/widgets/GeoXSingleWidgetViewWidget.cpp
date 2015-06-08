#include "precompiledHeaders.h"

#include "GeoXSingleWidgetViewWidget.h"
#include <string>

using namespace std;

GeoXSingleWidgetViewWidget::GeoXSingleWidgetViewWidget(QWidget *parent, Qt::WindowFlags f, GeoXWorkSpace *context, GeoXCanvas *container, string data) : QWidget(parent, f)
{
	this->context = context;
	this->container = container;
	this->data = data;
}

