#include "precompiledHeaders.h"

#include "GeoXImageView.h"
#include "GeoXImageViewWidget.h"
#include "SingleWidgetCanvas.h"
#include "Tools.h"


IMPLEMENT_GEOX_CLASS(GeoXImageView, 0) {
	BEGIN_CLASS_INIT(GeoXImageView)
}

GeoXImageView::GeoXImageView() {}

QWidget *GeoXImageView::createWidget(QWidget *parent, Qt::WindowFlags f)
{
   UNUSED(f);
   ivWidget = new GeoXImageViewWidget(parent, f, getContext(), getContainer(), getData());
   return ivWidget;
}

bool GeoXImageView::canHandleCanvas(const MetaClass *canvasType)
{
	return canvasType != NULL && canvasType->inheritsFrom(SingleWidgetCanvas::getClass());
}

bool GeoXImageView::canHandleData(const MetaClass *dataType)
{
	return dataType != NULL && dataType->inheritsFrom(DynamicArrayOfStructures2D::getClass());
}

GeoXImageView::~GeoXImageView() {}
