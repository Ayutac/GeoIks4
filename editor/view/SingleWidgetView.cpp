#include "precompiledHeaders.h"

#include "SingleWidgetView.h"
#include "SingleWidgetCanvas.h"



IMPLEMENT_GEOX_ABSTRACT_CLASS(SingleWidgetView, 0) {
	BEGIN_CLASS_INIT(SingleWidgetView)
}


SingleWidgetView::SingleWidgetView()
{
}

SingleWidgetView::~SingleWidgetView()
{
}

const MetaClass *SingleWidgetView::preferredCanvasType()
{
	return SingleWidgetCanvas::getClass();
}
