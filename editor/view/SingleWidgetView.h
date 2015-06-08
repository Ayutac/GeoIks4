#ifndef SINGLEWIDGETVIEW_H
#define SINGLEWIDGETVIEW_H

#include "GeoXView.h"
#include <QWidget>

/// This class is the parent class of GeoXViews that show content by just a single QTWidget
/// (for example, a text editor or an image viewer).
///
class SingleWidgetView : public GeoXView {
	GEOX_ABSTRACT_CLASS(SingleWidgetView)
 public:
	SingleWidgetView();

	virtual QWidget *createWidget(QWidget *parent, Qt::WindowFlags f) = 0;

	// inherited
	virtual const MetaClass *preferredCanvasType();

	~SingleWidgetView();
};

#endif // SINGLEWIDGETVIEW_H
