#ifndef SINGLEWIDGETCANVAS_H
#define SINGLEWIDGETCANVAS_H

#include "GeoXCanvas.h"
#include <QVBoxLayout>

/// This canvas displays only one view. The view is represented by a QT Widget.
/// The class is accordingly simplistic. Be careful not to add more than one view - this
/// will raise an exception.
///
class SingleWidgetCanvas : public GeoXCanvas {
	GEOX_CLASS(SingleWidgetCanvas)
 private:
	GeoXCanvasWidget *myWidget;
	QVBoxLayout *layout;
 protected:
	/// override this method to define which widget to use
	virtual GeoXCanvasWidget *createWidget(QWidget *parent = 0, Qt::WindowFlags f = 0);

 public:
	SingleWidgetCanvas();

	/// You can only add one view. It must be of type SingleWidgetView.
	/// Delete the view before adding another one (not recommended, but this works).
	virtual void addView(GeoXView *view, string &name);
	/// Delete the current view (which must be the same as given as parameter).
	virtual void deleteView(GeoXView *view);
	/// Delete the current view (which must be the same as given as parameter).
	virtual void deleteView(string name);

	~SingleWidgetCanvas();
};

#endif // SINGLEWIDGETCANVAS_H
