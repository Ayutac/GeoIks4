#ifndef GEOXTEXTVIEW_H
#define GEOXTEXTVIEW_H

#include "SingleWidgetView.h"

class GeoXTextViewWidget;

/// This viewer shows the data contained in an object as plain text. It is a two-way tool: One can dump the content of a data
/// object to the text console, edit it, and re-read it.
///
/// Limitations: Currently, only objects of type "DynamicArrayOfStructures" are supported.
///
class GeoXTextView : public SingleWidgetView {
	GEOX_CLASS(GeoXTextView)
 private:
	GeoXTextViewWidget *tvWidget;
 public:
	GeoXTextView();

	virtual QWidget *createWidget(QWidget *parent, Qt::WindowFlags f);

	// inherited
	virtual bool canHandleCanvas(const MetaClass *canvasType);
	// inherited
	virtual bool canHandleData(const MetaClass *dataType);

	~GeoXTextView();
};

#endif // GEOXTEXTVIEW_H
