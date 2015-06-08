#ifndef OPENGLCANVAS2D_H
#define OPENGLCANVAS2D_H

#include "GeoXCanvas.h"
#include "OpenGLCanvas.h"
#include "Camera.h"
#include "ViewFrustum.h"

class PanZoomCameraController2D;


/// This is a specialized OpenGL canvas that is used for 2D visualization.
/// In principle, the 3D and 2D version are equivalent in what they can handle (views are compatible).
/// The difference is in the user interface and the (default) camera controller.
///
class OpenGLCanvas2D : public OpenGLCanvas {
   GEOX_CLASS(OpenGLCanvas2D)
 protected:
   // --- camera control
   Camera camera;
   ViewFrustum viewFrustum;
   PanZoomCameraController2D* camControl;

   Vector3f bgColor;
   bool orthoView;

   void drawAxes();

   /// internal use: overriden to modify the widget configuration
   virtual GeoXCanvasWidget *createWidget(QWidget *parent = 0, Qt::WindowFlags f = 0);

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
   OpenGLCanvas2D();

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
   PanZoomCameraController2D *getCameraController() {return camControl;}

   ~OpenGLCanvas2D();
};


#endif // OPENGLCANVAS2D_H
