#ifndef GLGeometryViewerH
#define GLGeometryViewerH

#include "GeoXCanvas.h"


class LogoCanvas : public GeoXCanvas {
   GEOX_CLASS(LogoCanvas)
 protected:
   virtual GeoXCanvasWidget *createWidget(QWidget *parent = 0, Qt::WindowFlags f = 0);

 public:
   LogoCanvas();
   ~LogoCanvas();
};





#endif
