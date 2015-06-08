//---------------------------------------------------------------------------
#ifndef GeoXGLWidget3DH
#define GeoXGLWidget3DH
//---------------------------------------------------------------------------
#include <QtOpenGL/QGLWidget>
#include <GL/glu.h>
#include "VisTypes.h"
#include "ExaminerCameraController.h"
#include "Camera.h"
#include "ViewFrustum.h"
//---------------------------------------------------------------------------


class GeoXGLWidget3D : public QGLWidget {
   Q_OBJECT
public:
	static const int MODE_PICK = 0;
	static const int MODE_CAM = 1;
	static const int MODE_CREATE = 2;
	static const int MODE_CAM_ORTHO = 4;

private:
	std::vector<Point3D> points; 
	std::vector<Line> lines;
	std::vector<Triangle> triangles;
		
	GLenum fillMode;
	
	void findClosestPoint(int x, int y);

	int mode;

	int pickedPointIndex;
	float32 pickedZ;
	
	Camera camera;
	ViewFrustum vf;
	CameraController* camControl;

	Vector3f bgColor;

	bool lightOnOff;
	bool recomputeNormal;	
	bool drawAxisFlag;	
	bool drawHandlePoints;
	bool faceLighting;
	
signals:
	void widgetContentChanged();

 protected:
	virtual void paintGL();
	virtual void initializeGL();
	void drawAxis();
	virtual void resizeGL(int width, int height);
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent (QMouseEvent * event);
	virtual void mouseMoveEvent(QMouseEvent *event);

 public:
   GeoXGLWidget3D(QWidget *parent=0, const QGLWidget *shareWidget=0, Qt::WindowFlags f=0);
   virtual ~GeoXGLWidget3D();
	
	// add primitives
	int      addPoint(Point3D p);
	int      addLine(Line l);
	int      addTriangle(Triangle t);

	// return primitives
	Point3D  getPoint(int index);
	Line     getLine(int index);
	Triangle getTriangle	(int index);
	
	// manipulate primitives
	void     setPoint    (int index, Point3D p);
	void     setLine     (int index, Line l);
	void     setTriangle (int index, Triangle t);

	// inform about primitives
   mpint      getNumberOfPoints()    {return points.size();};
   mpint      getNumberOfLines()     {return lines.size();};
   mpint      getNumberOfTriangles() {return triangles.size();};

	// remove primitives
	void     clear();           // all
	void     clearLines();		 // only lines
	void     clearTriangles();  // only triangles

	void     swapFillMode();      // toggle solid / wire frame
	void     swapLighting();	   // toggle light on / off
	void     swapLightingMode();	// toggle per face lighting / per vertex lighting
	void     swapDrawAxis();	   // toggle axis drawing
	void     swapHandleDrawing(); // toggle control point visibility
	void     resetCamera();       // set camera to initial position
	
	void     setCameraMode(){mode = MODE_CAM;};
	void     setPickMode()  {mode = MODE_PICK;};
	void     setCameraOrthoMode() { mode = MODE_CAM_ORTHO; }
	int      getMode() const { return mode; }

	void     setBackgroundColor(Vector3f color){bgColor = color;};
	void     setBackgroundColor(float red, float green, float blue){bgColor = makeVector3f(red,green,blue);};
};

#endif
