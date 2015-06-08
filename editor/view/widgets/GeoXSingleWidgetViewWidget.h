#ifndef GEOXSINGLEWIDGETVIEWWIDGET_H
#define GEOXSINGLEWIDGETVIEWWIDGET_H

#include <QWidget>
#include <string>

class GeoXWorkSpace;
class GeoXCanvas;

/// Super class of the widgets used in the single widget views.
/// Not much to see here (just a bit of administrative wiring).
///
class GeoXSingleWidgetViewWidget : public QWidget {
	Q_OBJECT
 protected:
	GeoXWorkSpace *context;
	GeoXCanvas *container;
    std::string data;

 public:
    explicit GeoXSingleWidgetViewWidget(QWidget *parent, Qt::WindowFlags f, GeoXWorkSpace *context, GeoXCanvas *container, std::string data);

};

#endif // GEOXSINGLEWIDGETVIEWWIDGET_H
