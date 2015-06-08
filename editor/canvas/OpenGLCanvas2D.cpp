#include "precompiledHeaders.h"
#include "OpenGLCanvas2D.h"
#include "PanZoomCameraController2D.h"
#include "OpenGLCanvasWidget.h"
#include "NamedObjectList.h"
#include "GeoXOpenGLView.h"
#include "EmbeddedObjectClassProperty.h"
#include "BooleanClassProperty.h"
#include "ObjectClassProperty.h"
#include "FixedArrayClassProperty.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <QMouseEvent>

IMPLEMENT_GEOX_CLASS(OpenGLCanvas2D, 0)
{
   BEGIN_CLASS_INIT(OpenGLCanvas2D)
   ADD_EMBEDDED_OBJECT_PROP(camera, 0, Camera::getClass())
   ADD_EMBEDDED_OBJECT_PROP(viewFrustum, 0, ViewFrustum::getClass());
   ADD_OBJECT_PROP(camControl, 0, CameraController::getClass(), true);
   SET_EDIT_READONLY(true)
   ADD_VECTOR3F_PROP(bgColor, 0)
   ADD_BOOLEAN_PROP(orthoView, 0)
}

OpenGLCanvas2D::OpenGLCanvas2D()
{
   camControl = new PanZoomCameraController2D();
   camControl->bindCamera(&camera);
	camControl->areaResize(100, 100);
	camControl->apply();
	bgColor = makeVector3f(1.0f,1.0f,1.0f);
   orthoView = false;
	//camera.setPosition(makeVector3f(0.0f,0.0f,10.0f));
	//camera.setLookAt(makeVector3f(0.0f,0.0f,0.0f));
	//camera.setUp(makeVector3f(0.0f,1.0f,0.0f));
}

void OpenGLCanvas2D::setBackgroundColor(Vector3f color)
{
   bgColor = color;
}

void OpenGLCanvas2D::setBackgroundColor(float red, float green, float blue)
{
   bgColor = makeVector3f(red, green, blue);
}

void OpenGLCanvas2D::glFramePaintGL()
{
   glViewport(0,0, getOGLCWidget()->getGLWidgetWidth(), getOGLCWidget()->getGLWidgetHeight());
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   float32 aspect;
   if (getOGLCWidget()->height() > 0) {
      aspect = ((float)getOGLCWidget()->getGLWidgetWidth())/((float)getOGLCWidget()->getGLWidgetHeight());
   } else {
      aspect = 1;
   }
   viewFrustum.setXToYAspectRatio(aspect);

   float32 zoom = camControl->getZoom();
   float32 xRelSize = zoom*aspect;
   glOrtho(-xRelSize, xRelSize, -zoom, zoom, viewFrustum.getZNearClip(), viewFrustum.getZFarClip());

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   Vector3f pos = camera.getPosition();
   Vector3f lookAt = camera.getLookAt();
   Vector3f up = camera.getUp();
   gluLookAt(pos[0],pos[1],pos[2],lookAt[0],lookAt[1],lookAt[2],up[0],up[1],up[2]);
   glClearColor(bgColor[0],bgColor[1],bgColor[2],0.0f);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//   glDisable(GL_DEPTH_TEST);
//   glDepthMask(false);
//   glEnable(GL_BLEND);
//   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//   glEnable(GL_LINE_SMOOTH);
//   glEnable(GL_POLYGON_SMOOTH);
//   glEnable(GL_POINT_SMOOTH);
	Matrix4f flipMat = makeMatrix4f(
		-1,0,0,0,
		 0,1,0,0,
		 0,0,1,0,
		 0,0,0,1
	);
	glMultMatrixf(flipMat.data());


   if (getOGLCWidget()->getDrawAxis()) drawAxes();
	QGLWidget *glWidget = NULL;
	OpenGLCanvasWidget *cw = getOGLCWidget();
	if (cw) glWidget = cw->getGLWidget();
   for (size_t i=0; i<views->getSize(); i++) {
		((GeoXOpenGLView*)(views->getEntry(i)))->renderGL(glWidget);
   }
}

void OpenGLCanvas2D::glFrameResize(int width, int height)
{
   camControl->areaResize(width, height);
}

void OpenGLCanvas2D::glFrameMousePressEvent(QMouseEvent *event)
{
   camControl->mouseDown(event->x(),event->y(),MouseButtons(event->button()==Qt::LeftButton,event->button()==Qt::RightButton, event->button()==Qt::MidButton),event->modifiers());
   getOGLCWidget()->repaint();
}

void OpenGLCanvas2D::glFrameMouseReleaseEvent(QMouseEvent *event)
{
   camControl->mouseUp(event->x(),event->y(),MouseButtons(event->button()==Qt::LeftButton,event->button()==Qt::RightButton, event->button()==Qt::MidButton),event->modifiers());
   getOGLCWidget()->repaint();
}

void OpenGLCanvas2D::glFrameMouseMoveEvent(QMouseEvent *event)
{
   camControl->mouseMoved(event->x(),event->y());
   getOGLCWidget()->repaint();
}

void OpenGLCanvas2D::glFrameWheelEvent(QWheelEvent *event)
{
   camControl->mouseWheelRotated(event->delta(), Modifiers(event->modifiers()));
   getOGLCWidget()->repaint();
}

void OpenGLCanvas2D::on_btnResetCamera_pressed()
{
   resetCamera();
   getOGLCWidget()->repaint();
}

void OpenGLCanvas2D::on_btnDrawAxes_clicked(bool down)
{
   UNUSED(down);
   getOGLCWidget()->repaint();
}

void OpenGLCanvas2D::on_btnOrtho_clicked(bool down)
{
   setOrthoView(down);
   getOGLCWidget()->repaint();
}

void OpenGLCanvas2D::drawAxes()
{
   glPushAttrib(GL_ALL_ATTRIB_BITS);

   glDisable(GL_LIGHTING);
   glLineWidth(1.0f);

   glBegin(GL_LINES);

   glColor4f(1.0f,0.0f,0.0f,1.0f);
   glVertex3f(-2000.0f, 0.0f, 0.0f);
   glVertex3f( 2000.0f, 0.0f, 0.0f);

   glColor4f(0.0f,1.0f,0.0f,1.0f);
   glVertex3f(0.0f, -2000.0f, 0.0f);
   glVertex3f(0.0f,  2000.0f, 0.0f);

   glColor4f(0.0f,0.0f,1.0f,1.0f);
   glVertex3f(0.0f, 0.0f, -2000.0f);
   glVertex3f(0.0f, 0.0f,  2000.0f);

   glColor4f(0.8f,0.8f,0.8f,1.0f);
   glVertex3f(-1.0f,-1.0f,-1.0f);
   glVertex3f( 1.0f,-1.0f,-1.0f);

   glVertex3f( 1.0f,-1.0f,-1.0f);
   glVertex3f( 1.0f, 1.0f,-1.0f);

   glVertex3f( 1.0f, 1.0f,-1.0f);
   glVertex3f(-1.0f, 1.0f,-1.0f);

   glVertex3f(-1.0f, 1.0f,-1.0f);
   glVertex3f(-1.0f,-1.0f,-1.0f);

   glVertex3f(-1.0f,-1.0f, 1.0f);
   glVertex3f( 1.0f,-1.0f, 1.0f);

   glVertex3f( 1.0f,-1.0f, 1.0f);
   glVertex3f( 1.0f, 1.0f, 1.0f);

   glVertex3f( 1.0f, 1.0f, 1.0f);
   glVertex3f(-1.0f, 1.0f, 1.0f);

   glVertex3f(-1.0f, 1.0f, 1.0f);
   glVertex3f(-1.0f,-1.0f, 1.0f);

   glVertex3f(-1.0f,-1.0f,-1.0f);
   glVertex3f(-1.0f,-1.0f,-1.0f);

   glVertex3f(-1.0f,-1.0f,-1.0f);
   glVertex3f(-1.0f, 1.0f,-1.0f);

   glVertex3f(-1.0f, 1.0f,-1.0f);
   glVertex3f(-1.0f, 1.0f, 1.0f);

   glVertex3f(-1.0f, 1.0f, 1.0f);
   glVertex3f(-1.0f,-1.0f, 1.0f);

   glVertex3f(-1.0f,-1.0f, 1.0f);
   glVertex3f(-1.0f,-1.0f,-1.0f);

   glVertex3f( 1.0f,-1.0f,-1.0f);
   glVertex3f( 1.0f, 1.0f,-1.0f);

   glVertex3f( 1.0f, 1.0f,-1.0f);
   glVertex3f( 1.0f, 1.0f, 1.0f);

   glVertex3f( 1.0f, 1.0f, 1.0f);
   glVertex3f( 1.0f,-1.0f, 1.0f);

   glVertex3f( 1.0f,-1.0f, 1.0f);
   glVertex3f( 1.0f,-1.0f,-1.0f);

   glEnd();

   glPopAttrib();

}

GeoXCanvasWidget *OpenGLCanvas2D::createWidget(QWidget *parent, Qt::WindowFlags f)
{
   GeoXCanvasWidget *newWidget = OpenGLCanvas::createWidget(parent, f);
   OpenGLCanvasWidget *ow = dynamic_cast<OpenGLCanvasWidget*>(newWidget);
   if (ow) ow->set2DInterface(true);
   return newWidget;
}

void OpenGLCanvas2D::on_btnYAxis_clicked(bool down)
{
   ExaminerCameraController *cc = dynamic_cast<ExaminerCameraController*>(camControl);
   if (cc) cc->setRotAroundZAxis(!down);
   getOGLCWidget()->repaint();
}

void OpenGLCanvas2D::resetCamera()
{
   camControl->reset();
}

//void OpenGLCanvas2D::setCameraController(CameraController *camControl)
//{
//   if (this->camControl != camControl) {
//      delete this->camControl;
//      this->camControl = camControl;
//   }
//}

OpenGLCanvas2D::~OpenGLCanvas2D()
{
   delete camControl;
}

