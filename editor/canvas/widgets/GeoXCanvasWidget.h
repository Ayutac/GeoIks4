#ifndef GEOXCANVASWIDGET_H
#define GEOXCANVASWIDGET_H

#include <QWidget>

class GeoXCanvas;

/// This class implements the QWidget part of a GeoXCanvas.
/// The split is necessary because we cannot use multiple inheritence (clash of the Qt and GeoX reflection systems).
///
class GeoXCanvasWidget : public QWidget {
   Q_OBJECT
 private:
   bool deleting;
 protected:
   GeoXCanvas *canvas;

 public:
   explicit GeoXCanvasWidget(GeoXCanvas *canvas, QWidget *parent = 0, Qt::WindowFlags f = 0);

   ~GeoXCanvasWidget();
 friend class GeoXCanvas;
};

#endif // GEOXCANVASWIDGET_H
