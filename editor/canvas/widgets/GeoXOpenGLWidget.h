//---------------------------------------------------------------------------
#ifndef BasicGeoXGLWidget3DH
#define BasicGeoXGLWidget3DH
//---------------------------------------------------------------------------
#include <QtOpenGL/QGLWidget>
#include <QtOpenGL/qgl.h>
//---------------------------------------------------------------------------

class OpenGLCanvas;


class GeoXOpenGLWidget : public QGLWidget {
   Q_OBJECT
 private:
   OpenGLCanvas *canvas;
 protected:
	virtual void paintGL();
	virtual void resizeGL(int width, int height);
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent (QMouseEvent * event);
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void wheelEvent(QWheelEvent * event);

 public:
   GeoXOpenGLWidget(QWidget *parent=0, const QGLWidget *shareWidget=0, Qt::WindowFlags f=0);

   void setup(OpenGLCanvas *canvas);
	
   virtual ~GeoXOpenGLWidget();
};

#endif
