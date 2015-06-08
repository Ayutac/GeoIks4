#include "precompiledHeaders.h"
#include "GLFixedFunctionPointCloudView.h"
#include "SeparatorClassProperty.h"
#include "DynamicArrayOfStructures.h"
#include "DynamicArrayOfStructures.hpp"
#include "PointCloud.h"
#include "PointCloud.h"
#include "GeoXWorkSpace.h"
#include "NamedObjectList.h"
#include "GeoXData.h"
#include "OpenGLCanvas.h"
#include "OpenGLCanvas3D.h"
#include "NumericalClassProperty.h"
#include "FixedArrayClassProperty.h"
#include "BooleanClassProperty.h"
#include "StringClassProperty.h"
#include "GeoXOutput.h"
#include <GL/gl.h>




float32 GLFixedFunctionPointCloudView::getPointSize() const
{
   return pointSize;
}

void GLFixedFunctionPointCloudView::setPointSize(const float32 &value)
{
   pointSize = value;
}
string GLFixedFunctionPointCloudView::describeRenderMode(card32 val) {
   switch (val) {
      case MODE_SHADING_NORMALS: return "Shading: normals lighting";
      case MODE_SHADING_COLORS: return "Shading: vertex colors";
      case MODE_SHADING_NORMALS_AND_COLORS: return "Shading: vertex colors+normals";
      case MODE_SHADING_VERTEX_NORMALS_AS_COLORS: return "Shading: normals as colors";
      default: return "unknown";
   }
}

IMPLEMENT_GEOX_CLASS(GLFixedFunctionPointCloudView, 1)
{
   BEGIN_CLASS_INIT(GLFixedFunctionPointCloudView)
   ADD_CARD32_PROP_UPDATE_DESCR(renderMode, 0, NULL, &describeRenderMode, 3)
   ADD_SEPARATOR("default color / lighting")
   ADD_FLOAT32_PROP(pointSize ,0)
   ADD_VECTOR3F_PROP(baseColor, 0)
   ADD_VECTOR4F_PROP(lightPosition, 0)
   ADD_SEPARATOR("additonal normal vectors")
   ADD_BOOLEAN_PROP(drawNormalVectors ,0)
   ADD_FLOAT32_PROP(normalVectorLength ,0)
   ADD_SEPARATOR("advanced")
   ADD_STRING_PROP(positionChannel, 0)
   ADD_STRING_PROP(normalChannel, 0)
   ADD_STRING_PROP(colorChannel, 0)
}

GLFixedFunctionPointCloudView::GLFixedFunctionPointCloudView()
{
   renderMode = MODE_SHADING_NORMALS;
   baseColor = makeVector3f(0.7f, 0.7f, 0.7f);
   lightPosition = makeVector4f(100.0f,1000.0f,100.0f,1.0f);
   pointSize = 3;
   drawNormalVectors = false;
   normalVectorLength = 0.1f;
   positionChannel = "position";
   normalChannel = "normal";
   colorChannel = "color";
}

void GLFixedFunctionPointCloudView::renderGL(QGLWidget *glWidget)
{
   if (getContext()) {
      if (getData() != "") {
         GeoXData *dataPtr = dynamic_cast<GeoXData*>(getContext()->getData()->getObject(getData()));
         PointCloud *pc = dynamic_cast<PointCloud*>(dataPtr);
         if (pc) {
            drawPointCloud(pc);
         }
      }
   }
}

GLFixedFunctionPointCloudView::~GLFixedFunctionPointCloudView() {}

/// this code is old and ugly
void GLFixedFunctionPointCloudView::drawPointCloud( PointCloud *pc )
{
   DynamicArrayOfStructures *pts = pc->getVertices();
   const mpcard numPts = pts->getNumEntries();

   if (!pts->providesAttribute(positionChannel)) {
      warning("GLFixedFunctionPointCloudView::drawPointCloud -- position channel not found.");
      return;
   }
   AAT POS = pts->getAAT(positionChannel);

   AAT COL;
   bool hasCol = pts->providesAttribute(colorChannel);
   if (hasCol) COL = pts->getAAT(colorChannel);

   AAT NRM;
   bool hasNormal = pts->providesAttribute(normalChannel);
   if (hasNormal) NRM = pts->getAAT(normalChannel);

   glPushAttrib(GL_ALL_ATTRIB_BITS);

   glPointSize(pointSize);
   glEnable(GL_POINT_SMOOTH);
   glEnable(GL_ALPHA_TEST);
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


   if( renderMode == MODE_SHADING_VERTEX_NORMALS_AS_COLORS || renderMode == MODE_SHADING_COLORS ) {

      glEnable(GL_DEPTH_TEST);
      glBegin(GL_POINTS);

      for (mpcard i=0; i<numPts; i++) {
         Vector3f pos;
         Vector3f color;
         Vector3f normal = makeVector3f(1,1,1);
         pos = pts->get<float32, 3>(i, POS);

         if (hasNormal) {
            normal = pts->get<float32, 3>(i, NRM);
         }

         if (hasCol) {
            color = pts->get<float32, 3>(i, COL);
         } else {
            color = baseColor;
         }

         if (renderMode == MODE_SHADING_VERTEX_NORMALS_AS_COLORS) {
            color = normal +  makeVector3f(1,1,1)* 0.5f;
         }

         glColor3fv(color.data());
         glVertex3fv(pos.data());
      }
      glEnd();

   } else { // now:  renderMode != MODE_SHADING_VERTEX_NORMALS_AS_COLORS

       // now perform general shading with lighting
       glEnable(GL_DEPTH_TEST);
       glEnable(GL_LIGHTING);
       glEnable(GL_NORMALIZE);
       glEnable(GL_COLOR_MATERIAL);
       glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
       glEnable(GL_LIGHT0);
       glColor4f(baseColor[0], baseColor[1], baseColor[2], 1.0f);

       glLightfv( GL_LIGHT0, GL_POSITION, lightPosition.data() );

       glBegin(GL_POINTS);
       for (mpcard i=0; i<numPts; i++) {
          Vector3f pos;
          Vector3f color;
          Vector3f normal = YAXIS_VECTOR3F;

          pos = pts->get<float32, 3>(i, POS);

          if (hasNormal) {
             normal = pts->get<float32, 3>(i, NRM);
          }

          if (hasCol) {
             color = pts->get<float32, 3>(i, COL);
          } else {
             color = baseColor;
          }

          glColor3fv(color.data());
          glNormal3fv(normal.data());
          glVertex3fv(pos.data());
       }

       glEnd();
   }

   glPopAttrib();

   glPushAttrib(GL_ALL_ATTRIB_BITS);

   if( drawNormalVectors && hasNormal) {
      glEnable(GL_DEPTH_TEST);
      glDepthMask(true);
      glBegin( GL_LINES );
      glColor3f(0,1,0);
      for( mpcard i=0; i<numPts; i++ ) {
         Vector3f pos = pts->get<float32,3>(i, POS );
         Vector3f normal = pts->get<float32,3>(i, NRM );
         Vector3f p2 = pos + normal * normalVectorLength;
         glVertex3fv( &pos[0] );
         glVertex3fv( &p2[0] );
      }
      glEnd();
   }

   glPopAttrib();
}



card32 GLFixedFunctionPointCloudView::getRenderMode() const
{
   return renderMode;
}

void GLFixedFunctionPointCloudView::setRenderMode(const card32 &value)
{
   renderMode = value;
}

bool GLFixedFunctionPointCloudView::getDrawNormalVectors() const
{
   return drawNormalVectors;
}

void GLFixedFunctionPointCloudView::setDrawNormalVectors(bool value)
{
   drawNormalVectors = value;
}

float GLFixedFunctionPointCloudView::getNormalVectorLength() const
{
   return normalVectorLength;
}

void GLFixedFunctionPointCloudView::setNormalVectorLength(float value)
{
   normalVectorLength = value;
}

string GLFixedFunctionPointCloudView::getPositionChannel() const
{
   return positionChannel;
}

void GLFixedFunctionPointCloudView::setPositionChannel(const string &value)
{
   positionChannel = value;
}

string GLFixedFunctionPointCloudView::getNormalChannel() const
{
   return normalChannel;
}

void GLFixedFunctionPointCloudView::setNormalChannel(const string &value)
{
   normalChannel = value;
}

string GLFixedFunctionPointCloudView::getColorChannel() const
{
   return colorChannel;
}

void GLFixedFunctionPointCloudView::setColorChannel(const string &value)
{
   colorChannel = value;
}

Vector3f GLFixedFunctionPointCloudView::getBaseColor() const
{
   return baseColor;
}

void GLFixedFunctionPointCloudView::setBaseColor(const Vector3f &value)
{
   baseColor = value;
}

Vector4f GLFixedFunctionPointCloudView::getLightPosition() const
{
   return lightPosition;
}

void GLFixedFunctionPointCloudView::setLightPosition(const Vector4f &value)
{
   lightPosition = value;
}

bool GLFixedFunctionPointCloudView::canHandleCanvas(const MetaClass *canvasType)
{
	return canvasType != NULL && canvasType->inheritsFrom(OpenGLCanvas::getClass());
}

bool GLFixedFunctionPointCloudView::canHandleData(const MetaClass *dataType)
{
	return dataType != NULL && dataType->inheritsFrom(PointCloud::getClass());
}

const MetaClass *GLFixedFunctionPointCloudView::preferredCanvasType()
{
	return OpenGLCanvas3D::getClass();
}


