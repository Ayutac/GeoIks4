#include "precompiledHeaders.h"
#include "OpenGLCanvas.h"
#include "NamedObjectList.h"
#include "EmbeddedObjectClassProperty.h"
#include "ObjectClassProperty.h"
#include "FixedArrayClassProperty.h"
#include "BooleanClassProperty.h"
#include "OpenGLCanvasWidget.h"
#include "GeoXOpenGLWidget.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include "GeoXOpenGLView.h"


IMPLEMENT_GEOX_ABSTRACT_CLASS(OpenGLCanvas, 0)
{
   BEGIN_CLASS_INIT(OpenGLCanvas)
}

OpenGLCanvas::OpenGLCanvas() {}

GeoXCanvasWidget *OpenGLCanvas::createWidget(QWidget *parent, Qt::WindowFlags f)
{
   return new OpenGLCanvasWidget(this, parent, f);
}

void OpenGLCanvas::addView(GeoXView *view, string &name)
{
   GeoXOpenGLView *glView = dynamic_cast<GeoXOpenGLView*>(view);
   if (glView) {
      GeoXCanvas::addView(glView, name);
   } else {
      throw ETypeMismatch("OpenGLCanvas::addView() - not a GeoXOpenGLView.");
   }
}

void OpenGLCanvas::update()
{
   GeoXCanvas::update();
   getOGLCWidget()->ui.glFrame->repaint();
}


OpenGLCanvas::~OpenGLCanvas()
{
}

