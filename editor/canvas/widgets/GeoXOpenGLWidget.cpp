//---------------------------------------------------------------------------
#include "precompiledHeaders.h"
//---------------------------------------------------------------------------
#include "GeoXOpenGLWidget.h"
#include <qevent.h>
#include "GeoXOutput.h"
#include "OpenGLCanvas.h"
//---------------------------------------------------------------------------



GeoXOpenGLWidget::GeoXOpenGLWidget(QWidget *parent, const QGLWidget *shareWidget, Qt::WindowFlags f)
: QGLWidget(parent, shareWidget, f)
{
   this->canvas = NULL;
}

void GeoXOpenGLWidget::setup(OpenGLCanvas *canvas)
{
   this->canvas = canvas;
}


void GeoXOpenGLWidget::paintGL()
{
   if (canvas) canvas->glFramePaintGL();
}

void GeoXOpenGLWidget::mousePressEvent(QMouseEvent *event)
{
   if (canvas) canvas->glFrameMousePressEvent(event);
}

void GeoXOpenGLWidget::mouseReleaseEvent (QMouseEvent * event)
{
   if (canvas) canvas->glFrameMouseReleaseEvent(event);
}

void GeoXOpenGLWidget::mouseMoveEvent(QMouseEvent *event)
{
   if (canvas) canvas->glFrameMouseMoveEvent(event);
}

void GeoXOpenGLWidget::resizeGL(int width, int height)
{
   if (canvas) canvas->glFrameResize(width, height);
}

void GeoXOpenGLWidget::wheelEvent(QWheelEvent * event)
{
   if (canvas) canvas->glFrameWheelEvent(event);
}

GeoXOpenGLWidget::~GeoXOpenGLWidget()
{
}





