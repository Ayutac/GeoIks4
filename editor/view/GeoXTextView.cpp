#include "precompiledHeaders.h"

#include "GeoXTextView.h"
#include "GeoXTextViewWidget.h"
#include "SingleWidgetCanvas.h"
#include "DynamicArrayOfStructures.hpp"
#include "Tools.h"


IMPLEMENT_GEOX_CLASS(GeoXTextView, 0) {
	BEGIN_CLASS_INIT(GeoXTextView)
}

GeoXTextView::GeoXTextView() {}

QWidget *GeoXTextView::createWidget(QWidget *parent, Qt::WindowFlags f)
{
   UNUSED(f);
   tvWidget = new GeoXTextViewWidget(parent, f, getContext(), getContainer(), getData());
   return tvWidget;
}

bool GeoXTextView::canHandleCanvas(const MetaClass *canvasType)
{
	return canvasType != NULL && canvasType->inheritsFrom(SingleWidgetCanvas::getClass());
}

bool GeoXTextView::canHandleData(const MetaClass *dataType)
{
	return dataType != NULL && dataType->inheritsFrom(DynamicArrayOfStructures::getClass());
}

GeoXTextView::~GeoXTextView() {}
