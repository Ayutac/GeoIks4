#ifndef GEOXIMAGEVIEW_H
#define GEOXIMAGEVIEW_H

#include "SingleWidgetView.h"

class GeoXImageViewWidget;

/// This viewer displays 2D images
///
class GeoXImageView : public SingleWidgetView {
	GEOX_CLASS(GeoXImageView)
 private:
	GeoXImageViewWidget *ivWidget;
 public:
	GeoXImageView();

	virtual QWidget *createWidget(QWidget *parent, Qt::WindowFlags f);

	// inherited
	virtual bool canHandleCanvas(const MetaClass *canvasType);
	// inherited
	virtual bool canHandleData(const MetaClass *dataType);

	~GeoXImageView();
};

#endif
