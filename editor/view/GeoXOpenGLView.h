#ifndef GEOXOPENGLVIEW_H
#define GEOXOPENGLVIEW_H

#include "GeoXView.h"

class QGLWidget;

/// This is the super class of all views that perform immediate model OpenGL rendering
class GeoXOpenGLView : public GeoXView {
   GEOX_ABSTRACT_CLASS(GeoXOpenGLView)
 public:
   GeoXOpenGLView();

   /// Override this method: it draws all the 3D objects to the screen (immediate mode)-
   /// GL context will be active.
	virtual void renderGL(QGLWidget *glWidget) = 0;

   ~GeoXOpenGLView();
};

#endif // GEOXOPENGLVIEW_H
