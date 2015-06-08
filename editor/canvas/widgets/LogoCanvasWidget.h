#ifndef LogoCanvasWidget3DH
#define LogoCanvasWidget3DH

#include <QtOpenGL/QGLWidget>
#include <QtOpenGL/qgl.h>

#include "GeoXCanvasWidget.h"

#include "ui_LogoCanvas.h"


class LogoCanvasWidget : public GeoXCanvasWidget {
   Q_OBJECT
 private:
   Ui_LogoCanvas ui;

 public:
   explicit LogoCanvasWidget(GeoXCanvas *canvas, QWidget *parent = 0, Qt::WindowFlags f = 0);
	
   virtual ~LogoCanvasWidget();
};

#endif
