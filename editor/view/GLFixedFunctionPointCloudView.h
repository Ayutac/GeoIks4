#ifndef GLFIXEDFUNCTIONPOINTCLOUDVIEW_H
#define GLFIXEDFUNCTIONPOINTCLOUDVIEW_H

#include "GeoXOpenGLView.h"
#include "LinearAlgebra.hpp"

class TriangleMesh;
class PointCloud;

/// This is a very simple OpenGL renderer for point clouds and triangle meshes.
///
class GLFixedFunctionPointCloudView : public GeoXOpenGLView {
   GEOX_CLASS(GLFixedFunctionPointCloudView)
 public:
   /// possible render mode: normals are used for lighting computation
   static const card32 MODE_SHADING_NORMALS = 0;
   /// possible render mode: vertex colors are displayed
   static const card32 MODE_SHADING_COLORS = 1;
   /// possible render mode: normals and vertex colors are used for lighting computaqtion
   static const card32 MODE_SHADING_NORMALS_AND_COLORS = 2;
   /// possible render mode: transform vertex normals to RGB colors, then apply color interpolation
   static const card32 MODE_SHADING_VERTEX_NORMALS_AS_COLORS = 3;

 private:
   /// The mode must be one of the constants defined above (MODE_SHADING_TRIANGLE_NORMAL, ...).
   /// We use JAVA-style enums due to the constraints of the GeoX type system.
   card32 renderMode;
   /// size of the points being drawn in pixel
   float32 pointSize;
   /// base color that should be used for wire frame and if no vertex colors are available
   Vector3f baseColor;
   /// position of the light source in homogeneous coordinates. Set the last component to zero for directional lighting.
   Vector4f lightPosition;

   // --- option: additional rendering of normal vectors
   /// Should normals be shown on top of the rendered mesh?
   bool  drawNormalVectors;
   float normalVectorLength;
   // --- configure wire frame

   // --- advanced
   /// This variable controls from which channel the position information of the vertices should be read.
   /// The channel must be a three-dimensional float32 channel. Default name is "position".
   /// The channel must exist for rendeirng to work.
   string positionChannel;

   /// This variable controls from which channel the normal information of the vertices should be read.
   /// The channel must be a three-dimensional float32 channel. Default name is "normal".
   /// The channel is optional.
   string normalChannel;

   /// This variable controls from which channel the color information of the vertices should be read.
   /// The channel must be a three-dimensional float32 channel. Default name is "color".
   /// The channel is optional.
   string colorChannel;

   /// helper method for reflection
   static string describeRenderMode(card32 val);

 protected:
   /// inherited method that performs the actual rendering
   void drawPointCloud(PointCloud *pc);

 public:
   GLFixedFunctionPointCloudView();

   /// inherited - do the drawing
	virtual void renderGL(QGLWidget *glWidget);

   /// getter. see renderMode for details.
   card32 getRenderMode() const;
   /// setter. see renderMode for details.
   void setRenderMode(const card32 &value);

   /// getter. see pointSize for details.
   float32 getPointSize() const;
   /// setter. see pointSize for details.
   void setPointSize(const float32 &value);

   /// getter. see baseColor for details.
   Vector3f getBaseColor() const;
   /// setter. see baseColor for details.
   void setBaseColor(const Vector3f &value);

   /// getter. see lightPosition for details.
   Vector4f getLightPosition() const;
   /// setter. see lightPosition for details.
   void setLightPosition(const Vector4f &value);

   /// getter. see drawNormalVectors for details.
   bool getDrawNormalVectors() const;
   /// setter. see drawNormalVectors for details.
   void setDrawNormalVectors(bool value);

   /// getter. see normalVectorLength for details.
   float getNormalVectorLength() const;
   /// setter. see normalVectorLength for details.
   void setNormalVectorLength(float value);

   /// getter. see positionChannel for details.
   string getPositionChannel() const;
   /// setter. see positionChannel for details.
   void setPositionChannel(const string &value);

   /// getter. see normalChannel for details.
   string getNormalChannel() const;
   /// setter. see normalChannel for details.
   void setNormalChannel(const string &value);

   /// getter. see colorChannel for details.
   string getColorChannel() const;
   /// setter. see colorChannel for details.
   void setColorChannel(const string &value);

	// inherited
	virtual bool canHandleCanvas(const MetaClass *canvasType);
	// inherited
	virtual bool canHandleData(const MetaClass *dataType);
	// inherited
	virtual const MetaClass *preferredCanvasType();

   ~GLFixedFunctionPointCloudView();
};

#endif // GLFixedFunctionPointCloudView_H
