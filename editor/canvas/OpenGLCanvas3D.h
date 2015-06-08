#ifndef OPENGLCANVAS3D_H
#define OPENGLCANVAS3D_H

#include "OpenGLCanvas.h"

#include "ExaminerCameraController.h"
#include "Camera.h"
#include "ViewFrustum.h"


/// This is a drawing area for 3D visualizations of data using OpenGL
///
/// This canvas operates in immediate mode - i.e., the viewers are called back on
/// each redraw operation in order to refresh the display
///
class OpenGLCanvas3D : public OpenGLCanvas {
   GEOX_CLASS(OpenGLCanvas3D)
 protected:
   // --- camera control
   Camera camera;
   ViewFrustum viewFrustum;
   ExaminerCameraController* camControl;

   Vector3f bgColor;
   bool orthoView;

   void drawAxes();

   // --- delegated from GeoXOpenGLWidget contained in widget->ui
   virtual void glFramePaintGL();
   virtual void glFrameResize(int width, int height);
   virtual void glFrameMousePressEvent(QMouseEvent *event);
   virtual void glFrameMouseReleaseEvent (QMouseEvent * event);
   virtual void glFrameMouseMoveEvent(QMouseEvent *event);
   virtual void glFrameWheelEvent(QWheelEvent * event);

   // --- delegated from OpenGLCanvasWidget
   void on_btnResetCamera_pressed();
   void on_btnOrtho_clicked(bool b);
   void on_btnDrawAxes_clicked(bool b);
   void on_btnYAxis_clicked(bool b);

 public:
   OpenGLCanvas3D();

   /// set background color in float (1,1,1) == white, (0,0,0) == black, format (r,g,b)
   void setBackgroundColor(Vector3f color);
   /// set background color with individual components, range [0..1]
   void setBackgroundColor(float red, float green, float blue);
   /// set camera to initial position
   void resetCamera();

   /// Orthographic camera used?
   bool getOrthoView() const { return orthoView; }
   /// Set camera to perspective / orthographic view.
   void setOrthoView(bool val) { orthoView = val; }

   /// return camera settings (position, look at, upward direction; field of view is stored in view frustum)
   const Camera &getCamera() {return camera;}
   /// return view frustum settings (including field of view)
   const ViewFrustum &getViewFrustum() {return viewFrustum;}
   /// returns the current camera controller
   ExaminerCameraController *getCameraController() {return camControl;}

   ~OpenGLCanvas3D();
};

#endif // OPENGLCANVAS3D_H
