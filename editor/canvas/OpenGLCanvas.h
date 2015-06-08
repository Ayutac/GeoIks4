#ifndef OPENGLCANVAS_H
#define OPENGLCANVAS_H

#include "GeoXCanvas.h"

#include "ExaminerCameraController.h"
#include "Camera.h"
#include "ViewFrustum.h"

class GeoXOpenGLView;
class GeoXOpenGLWidget;
class OpenGLCanvasWidget;


/// This is a drawing area for visualizations of data using OpenGL
///
/// This canvas operates in immediate mode - i.e., the viewers are called back on
/// each redraw operation in order to refresh the display
///
/// This class is abstract. There are two (standard) concrete descedants; a 2D and a 3D version,
/// which handle rendering of 2D vector graphics and 3D graphics, respectively.
///
class OpenGLCanvas : public GeoXCanvas {
   GEOX_ABSTRACT_CLASS(OpenGLCanvas)
 protected:

   virtual void drawAxes() = 0;

   // --- delegated from GeoXOpenGLWidget contained in widget->ui
   virtual void glFramePaintGL() = 0;
   virtual void glFrameResize(int width, int height) = 0;
   virtual void glFrameMousePressEvent(QMouseEvent *event) = 0;
   virtual void glFrameMouseReleaseEvent (QMouseEvent * event) = 0;
   virtual void glFrameMouseMoveEvent(QMouseEvent *event) = 0;
   virtual void glFrameWheelEvent(QWheelEvent * event) = 0;

   // --- delegated from OpenGLCanvasWidget
   virtual void on_btnResetCamera_pressed() = 0;
   virtual void on_btnOrtho_clicked(bool b) = 0;
   virtual void on_btnDrawAxes_clicked(bool b) = 0;
   virtual void on_btnYAxis_clicked(bool b) = 0;

   inline OpenGLCanvasWidget *getOGLCWidget() {return (OpenGLCanvasWidget*)widget;}

 friend class GeoXOpenGLWidget;
 friend class OpenGLCanvasWidget;
 protected:
   virtual GeoXCanvasWidget *createWidget(QWidget *parent = 0, Qt::WindowFlags f = 0);

 public:
   OpenGLCanvas();

   // inherited
   /// adds a view, checks type
   virtual void addView(GeoXView *view, string &name);
   /// calls glRepaint
   virtual void update();

   /// set background color in float (1,1,1) == white, (0,0,0) == black, format (r,g,b)
   virtual void setBackgroundColor(Vector3f color) = 0;
   /// set background color with individual components, range [0..1]
   virtual void setBackgroundColor(float red, float green, float blue) = 0;
   /// set camera to initial position
   virtual void resetCamera() = 0;

   ~OpenGLCanvas();
};

#endif // OPENGLCANVAS_H
