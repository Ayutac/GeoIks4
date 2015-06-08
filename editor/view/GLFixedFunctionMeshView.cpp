#include "precompiledHeaders.h"
#include "GLFixedFunctionMeshView.h"
#include "SeparatorClassProperty.h"
#include "DynamicArrayOfStructures.h"
#include "DynamicArrayOfStructures.hpp"
#include "PointCloud.h"
#include "TriangleMesh.h"
#include "GeoXWorkSpace.h"
#include "NamedObjectList.h"
#include "GeoXData.h"
#include "OpenGLCanvas.h"
#include "OpenGLCanvas3D.h"
#include "FixedArrayClassProperty.h"
#include "StringClassProperty.h"
#include "NumericalClassProperty.h"
#include "BooleanClassProperty.h"
#include "GeoXOutput.h"
#include <GL/gl.h>
#include <GL/glu.h>



string GLFixedFunctionMeshView::describeRenderMode(card32 val) {
   switch (val) {
      case MODE_SHADING_NORMALS: return "Shading: normals lighting";
      case MODE_SHADING_COLORS: return "Shading: vertex colors+normals";
      case MODE_SHADING_VERTEX_NORMALS_AS_COLORS: return "Shading: normals as colors";
      case MODE_WIRE_FRAME: return "Wire frame";
      case MODE_WIRE_FRAME_HIDDEN_LINES_CULLING: return "Wire frame with hidden lines culling";
      default: return "unknown";
   }
}

IMPLEMENT_GEOX_CLASS(GLFixedFunctionMeshView, 1)
{
   BEGIN_CLASS_INIT(GLFixedFunctionMeshView)
   ADD_CARD32_PROP_UPDATE_DESCR(renderMode, 0, NULL, &describeRenderMode, 5)
   ADD_SEPARATOR("default color / lighting")
   ADD_VECTOR3F_PROP(baseColor, 0)
   ADD_VECTOR4F_PROP(lightPosition, 0)
   ADD_BOOLEAN_PROP(useTriangleNormals, 1)
   ADD_SEPARATOR("additonal normal vectors")
   ADD_BOOLEAN_PROP(drawNormalVectors ,0)
   ADD_FLOAT32_PROP(normalVectorLength ,0)
   ADD_SEPARATOR("advanced")
   ADD_STRING_PROP(positionChannel, 0)
   ADD_STRING_PROP(indexChannel, 0)
   ADD_STRING_PROP(normalChannel, 0)
   ADD_STRING_PROP(colorChannel, 0)
}

GLFixedFunctionMeshView::GLFixedFunctionMeshView()
{
   renderMode = MODE_WIRE_FRAME_HIDDEN_LINES_CULLING;
   baseColor = makeVector3f(0.7f, 0.7f, 0.7f);
   lightPosition = makeVector4f(100.0f,1000.0f,100.0f,1.0f);
   drawNormalVectors = false;
   normalVectorLength = 0.1f;
   useTriangleNormals = false;
   positionChannel = "position";
   indexChannel = "index";
   normalChannel = "normal";
   colorChannel = "color";
}

void GLFixedFunctionMeshView::renderGL(QGLWidget *glWidget)
{
   if (getContext()) {
      if (getData() != "") {
         GeoXData *dataPtr = dynamic_cast<GeoXData*>(getContext()->getData()->getObject(getData()));
         TriangleMesh *mesh = dynamic_cast<TriangleMesh*>(dataPtr);
         if (mesh) {
            drawMesh(mesh);
         }
      }
   }
}

GLFixedFunctionMeshView::~GLFixedFunctionMeshView() {}

/// this code is old and ugly
void GLFixedFunctionMeshView::drawMesh( TriangleMesh *mesh )
{
   DynamicArrayOfStructures *pts = mesh->getVertices();

   if (!pts->providesAttribute(positionChannel)) {
      warning("GLFixedFunctionMeshView::drawMesh -- position channel not found.");
      return;
   }
   AAT POS = pts->getAAT(positionChannel);

   AAT COL;
   bool hasCol = pts->providesAttribute(colorChannel);
   if (hasCol) COL = pts->getAAT(colorChannel);

   AAT NRM;
   bool hasNormal = pts->providesAttribute(normalChannel);
   if (hasNormal) NRM = pts->getAAT(normalChannel);

   DynamicArrayOfStructures *idx = mesh->getTriangles();
   if (!idx->providesAttribute(indexChannel)) {
      warning("GLFixedFunctionMeshView::drawMesh -- index channel not found.");
      return;
   }
   AAT IDX = idx->getAAT(indexChannel);

   glPushAttrib(GL_ALL_ATTRIB_BITS);

   if( renderMode == MODE_WIRE_FRAME_HIDDEN_LINES_CULLING ) {
      glEnable(GL_DEPTH_TEST);
      glDepthMask(true);
      glEnable( GL_POLYGON_OFFSET_FILL );
      glPolygonOffset( 1.2f, 4.0f );
      const card32 numTri = idx->getNumEntries();

      glBegin(GL_TRIANGLES);

      Vector3f invBaseColor = makeVector3f(1,1,1) - baseColor;
      if (norm(invBaseColor-baseColor) < 0.2) {
         if (norm(baseColor)>0.5) {
            invBaseColor = NULL_VECTOR3F;
         } else {
            invBaseColor = makeVector3f(1,1,1);
         }
      }
      glColor4f(invBaseColor[0], invBaseColor[1], invBaseColor[2], 0.25f);
      for (card32 i=0; i<numTri; i++) {
         Vector3i tind = idx->get<int32, 3>(i, IDX);
         Vector3f pos[3];
         pos[0] = pts->get<float32, 3>(tind[0], POS);
         pos[1] = pts->get<float32, 3>(tind[1], POS);
         pos[2] = pts->get<float32, 3>(tind[2], POS);


         glVertex3fv(pos[0].data());
         glVertex3fv(pos[1].data());
         glVertex3fv(pos[2].data());
      }

      glEnd();

      glDisable(GL_LIGHTING);
      glDisable(GL_COLOR_MATERIAL);
      glDisable( GL_POLYGON_OFFSET_FILL );

      glColor4f(baseColor[0], baseColor[1], baseColor[2], 1.0f);
      if (norm(invBaseColor) < norm(baseColor)) {
         glBlendFunc(GL_SRC_ALPHA, GL_ONE);
         glDepthMask(false);
         glEnable(GL_LINE_SMOOTH);
         glEnable(GL_BLEND);
      }
      glDisable( GL_CULL_FACE );

      glBegin(GL_LINES);
      for (card32 i=0; i<numTri; i++) {
         Vector3i tind = idx->get<int32, 3>(i, IDX);
         Vector3f pos[3];
         pos[0] = pts->get<float32, 3>(tind[0], POS);
         pos[1] = pts->get<float32, 3>(tind[1], POS);
         pos[2] = pts->get<float32, 3>(tind[2], POS);

         glVertex3fv(pos[0].data());
         glVertex3fv(pos[1].data());

         glVertex3fv(pos[1].data());
         glVertex3fv(pos[2].data());

         glVertex3fv(pos[2].data());
         glVertex3fv(pos[0].data());
      }
      glEnd();

      glDepthMask(true);

   } else if( renderMode != MODE_WIRE_FRAME ) {

      if( renderMode == MODE_SHADING_VERTEX_NORMALS_AS_COLORS ) {

         glEnable(GL_DEPTH_TEST);
         const card32 numTri = idx->getNumEntries();

         glBegin(GL_TRIANGLES);

         for (card32 i=0; i<numTri; i++) {
            Vector3i tind = idx->get<int32, 3>(i, IDX);
            Vector3f pos[3];
            Vector3f color[3];
            Vector3f normal[3];
            pos[0] = pts->get<float32, 3>(tind[0], POS);
            pos[1] = pts->get<float32, 3>(tind[1], POS);
            pos[2] = pts->get<float32, 3>(tind[2], POS);

            if (!hasNormal || useTriangleNormals) {
               Vector3f tnormal = (pos[1]-pos[0]).crossProduct(pos[2]-pos[0]);
               float32 n = norm(tnormal);
               if (n > 1e-7) {
                  tnormal /= n;
               } else {
                  continue;
               }
               normal[0] = tnormal;
               normal[1] = tnormal;
               normal[2] = tnormal;
            } else {
               normal[0] = pts->get<float32, 3>(tind[0], NRM);
               normal[1] = pts->get<float32, 3>(tind[1], NRM);
               normal[2] = pts->get<float32, 3>(tind[2], NRM);
            }

            for( card32 j=0; j<3; j++ ) {
               color[j] = (normal[j] +  makeVector3f(1,1,1))* 0.5f;
            }

            glColor3fv(color[0].data());
            glVertex3fv(pos[0].data());
            glColor3fv(color[1].data());
            glVertex3fv(pos[1].data());
            glColor3fv(color[2].data());
            glVertex3fv(pos[2].data());
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

          const card32 numTri = idx->getNumEntries();
          glBegin(GL_TRIANGLES);
          for (card32 i=0; i<numTri; i++) {
             Vector3i tind = idx->get<int32, 3>(i, IDX);
             Vector3f pos[3];
             Vector3f color[3];
             Vector3f normal[3];

             pos[0] = pts->get<float32, 3>(tind[0], POS);
             pos[1] = pts->get<float32, 3>(tind[1], POS);
             pos[2] = pts->get<float32, 3>(tind[2], POS);

             if (!hasNormal || useTriangleNormals) {
                Vector3f tnormal = (pos[1]-pos[0]).crossProduct(pos[2]-pos[0]);
                float32 n = norm(tnormal);
                if (n > 1e-20) tnormal /= n;
                normal[0] = tnormal;
                normal[1] = tnormal;
                normal[2] = tnormal;
             } else {
                normal[0] = pts->get<float32, 3>(tind[0], NRM);
                normal[1] = pts->get<float32, 3>(tind[1], NRM);
                normal[2] = pts->get<float32, 3>(tind[2], NRM);
             }

             if (hasCol) {
                color[0] = pts->get<float32, 3>(tind[0], COL);
                color[1] = pts->get<float32, 3>(tind[1], COL);
                color[2] = pts->get<float32, 3>(tind[2], COL);
             } else {
                color[0] = baseColor;
                color[1] = baseColor;
                color[2] = baseColor;
             }

             glColor3fv(color[0].data());
             glNormal3fv(normal[0].data());
             glVertex3fv(pos[0].data());
             glColor3fv(color[1].data());
             glNormal3fv(normal[1].data());
             glVertex3fv(pos[1].data());
             glColor3fv(color[2].data());
             glNormal3fv(normal[2].data());
             glVertex3fv(pos[2].data());
          }

          glEnd();
      }
   } else { // ordinary wireframe mode

      glDisable(GL_LIGHTING);
      glDisable(GL_COLOR_MATERIAL);

      glColor4f(baseColor[0], baseColor[1], baseColor[2], 0.25f);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE);
      glDepthMask(false);
      glEnable(GL_LINE_SMOOTH);
      glEnable(GL_BLEND);
      glDisable( GL_CULL_FACE );

      const card32 numTri = idx->getNumEntries();
      glBegin(GL_LINES);
      for (card32 i=0; i<numTri; i++) {
         Vector3i tind = idx->get<int32, 3>(i, IDX);
         Vector3f pos[3];
         pos[0] = pts->get<float32, 3>(tind[0], POS);
         pos[1] = pts->get<float32, 3>(tind[1], POS);
         pos[2] = pts->get<float32, 3>(tind[2], POS);

         glVertex3fv(pos[0].data());
         glVertex3fv(pos[1].data());

         glVertex3fv(pos[1].data());
         glVertex3fv(pos[2].data());

         glVertex3fv(pos[2].data());
         glVertex3fv(pos[0].data());
      }
      glEnd();

      glDepthMask(true);
   }

   glPopAttrib();

   glPushAttrib(GL_ALL_ATTRIB_BITS);

   if( drawNormalVectors && hasNormal) {
      glEnable(GL_DEPTH_TEST);
      glDepthMask(true);
      glBegin( GL_LINES );
      glColor3f(0,1,0);
      for( card32 i=0;i<pts->getNumEntries();i++ ) {
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



card32 GLFixedFunctionMeshView::getRenderMode() const
{
   return renderMode;
}

void GLFixedFunctionMeshView::setRenderMode(const card32 &value)
{
   renderMode = value;
}

bool GLFixedFunctionMeshView::getDrawNormalVectors() const
{
   return drawNormalVectors;
}

void GLFixedFunctionMeshView::setDrawNormalVectors(bool value)
{
   drawNormalVectors = value;
}

float GLFixedFunctionMeshView::getNormalVectorLength() const
{
   return normalVectorLength;
}

void GLFixedFunctionMeshView::setNormalVectorLength(float value)
{
   normalVectorLength = value;
}

string GLFixedFunctionMeshView::getPositionChannel() const
{
   return positionChannel;
}

void GLFixedFunctionMeshView::setPositionChannel(const string &value)
{
   positionChannel = value;
}

string GLFixedFunctionMeshView::getIndexChannel() const
{
   return indexChannel;
}

void GLFixedFunctionMeshView::setIndexChannel(const string &value)
{
   indexChannel = value;
}

string GLFixedFunctionMeshView::getNormalChannel() const
{
   return normalChannel;
}

void GLFixedFunctionMeshView::setNormalChannel(const string &value)
{
   normalChannel = value;
}

string GLFixedFunctionMeshView::getColorChannel() const
{
   return colorChannel;
}

void GLFixedFunctionMeshView::setColorChannel(const string &value)
{
	colorChannel = value;
}

bool GLFixedFunctionMeshView::canHandleCanvas(const MetaClass *canvasType)
{
	return canvasType != NULL && canvasType->inheritsFrom(OpenGLCanvas::getClass());
}

bool GLFixedFunctionMeshView::canHandleData(const MetaClass *dataType)
{
	return dataType != NULL && dataType->inheritsFrom(TriangleMesh::getClass());
}

const MetaClass *GLFixedFunctionMeshView::preferredCanvasType()
{
	return OpenGLCanvas3D::getClass();
}

bool GLFixedFunctionMeshView::getUseTriangleNormals() const
{
   return useTriangleNormals;
}

void GLFixedFunctionMeshView::setUseTriangleNormals(bool value)
{
   useTriangleNormals = value;
}


Vector3f GLFixedFunctionMeshView::getBaseColor() const
{
   return baseColor;
}

void GLFixedFunctionMeshView::setBaseColor(const Vector3f &value)
{
   baseColor = value;
}

Vector4f GLFixedFunctionMeshView::getLightPosition() const
{
   return lightPosition;
}

void GLFixedFunctionMeshView::setLightPosition(const Vector4f &value)
{
   lightPosition = value;
}


