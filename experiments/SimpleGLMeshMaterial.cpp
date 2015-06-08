//---------------------------------------------------------------------------
#include "precompiledHeaders.h"
 //---------------------------------------------------------------------------
#include "SimpleGLMeshMaterial.h"
//---------------------------------------------------------------------------
#include "BooleanClassProperty.h"
#include "NumericalClassProperty.h"
#include "LinearAlgebra.hpp"
#include "DynamicArrayOfStructures.hpp"
#include "TriangleMesh.h"
#include <GL/gl.h>
//---------------------------------------------------------------------------


IMPLEMENT_GEOX_CLASS(SimpleGLMeshMaterial, 0) {
   BEGIN_CLASS_INIT(SimpleGLMeshMaterial)
   ADD_BOOLEAN_PROP(wireframe ,0)
   ADD_BOOLEAN_PROP(drawNormalsVectors ,0)
   ADD_FLOAT32_PROP(normalLength ,0)
   ADD_BOOLEAN_PROP(normalsDirectionColors ,0)
   ADD_BOOLEAN_PROP(useVertexNormals ,0)
   ADD_BOOLEAN_PROP(showWireFrameOnly ,0)
   ADD_BOOLEAN_PROP(showWireFrameOnlyDepthTest ,0)
   ADD_BOOLEAN_PROP(blackWireFrame ,0)
   ADD_BOOLEAN_PROP(cullBackFace, 0)
}


// ----


SimpleGLMeshMaterial::SimpleGLMeshMaterial() {
   wireframe = false;
   drawNormalsVectors = false;
   normalLength = 0.1f;
   normalsDirectionColors = true;
   useVertexNormals = true;
   showWireFrameOnly = false;
   showWireFrameOnlyDepthTest = false;
   blackWireFrame = true;
	cullBackFace = false;
}


void SimpleGLMeshMaterial::draw( TriangleMesh *mesh )
{
   if (mesh != NULL) {
      DynamicArrayOfStructures *pts = mesh->getVertices();

      if (!pts->providesAttribute("position")) return;
      AAT POS = pts->getAAT("position");

      AAT COL;
      bool hasCol = pts->providesAttribute("color");
      if (hasCol) COL = pts->getAAT("color");

      AAT NRM;
      bool hasNormal = pts->providesAttribute("normal");
      if (hasNormal) NRM = pts->getAAT("normal");

      DynamicArrayOfStructures *idx = mesh->getTriangles();
      if (!idx->providesAttribute("index")) return;
      AAT IDX = idx->getAAT("index");
      
      glPushAttrib(GL_ENABLE_BIT);

      if( showWireFrameOnly && showWireFrameOnlyDepthTest )
      {
         glEnable(GL_DEPTH_TEST);
			if( cullBackFace )
		      glEnable(GL_CULL_FACE);
			else
			   glDisable(GL_CULL_FACE);
         glEnable( GL_POLYGON_OFFSET_FILL );
         glPolygonOffset( 1.2f, 4.0f );

         const card32 numTri = idx->getNumEntries();
         glBegin(GL_TRIANGLES);
         glColor3f(1,1,1);
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
      }
      else
      if( !showWireFrameOnly ) {
          if( normalsDirectionColors )
          {
             glEnable(GL_DEPTH_TEST);
                if( cullBackFace )
                  glEnable(GL_CULL_FACE);
                else
                   glDisable(GL_CULL_FACE);

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

                if (!hasNormal || !useVertexNormals) {
                   Vector3f tnormal = (pos[2]-pos[0]).crossProduct(pos[1]-pos[0]);
                   float32 n = norm(tnormal);
                   if (n > 1e-7)
                      tnormal /= n;
                   else
                      continue;
                   normal[0] = tnormal;
                   normal[1] = tnormal;
                   normal[2] = tnormal;
                }

                if (hasNormal && useVertexNormals) {
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
          }
          else {

             glEnable(GL_DEPTH_TEST);
             glEnable(GL_LIGHTING);
             glEnable(GL_NORMALIZE);
             glEnable(GL_COLOR_MATERIAL);
             glDisable(GL_CULL_FACE);
             glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
             glEnable(GL_LIGHT0);
             glColor4f(1,1,1,1);
             float lightPos[] = {100.0f,1000.0f,100.0f,1.0f};
             glLightfv( GL_LIGHT0, GL_POSITION, lightPos );

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

                if (!hasNormal || !useVertexNormals) {
                   Vector3f tnormal = (pos[2]-pos[0]).crossProduct(pos[1]-pos[0]);
                   float32 n = norm(tnormal);
                   if (n > 1e-20) tnormal /= n;
                   normal[0] = tnormal;
                   normal[1] = tnormal;
                   normal[2] = tnormal;
                }

                if (hasCol) {
                   color[0] = pts->get<float32, 3>(tind[0], COL);
                   color[1] = pts->get<float32, 3>(tind[1], COL);
                   color[2] = pts->get<float32, 3>(tind[2], COL);
                }
                if (hasNormal && useVertexNormals) {
                   normal[0] = pts->get<float32, 3>(tind[0], NRM);
                   normal[1] = pts->get<float32, 3>(tind[1], NRM);
                   normal[2] = pts->get<float32, 3>(tind[2], NRM);
                }

                if (hasCol) glColor3fv(color[0].data());
                glNormal3fv(normal[0].data());
                glVertex3fv(pos[0].data());
                if (hasCol) glColor3fv(color[1].data());
                glNormal3fv(normal[1].data());
                glVertex3fv(pos[1].data());
                if (hasCol) glColor3fv(color[2].data());
                glNormal3fv(normal[2].data());
                glVertex3fv(pos[2].data());
             }
             glEnd();
          }
       }
      glPopAttrib();

      glPushAttrib(GL_ENABLE_BIT);

       if (wireframe || showWireFrameOnly) {
         if( showWireFrameOnlyDepthTest && showWireFrameOnly)
            glEnable(GL_DEPTH_TEST);
         else
            glDisable(GL_DEPTH_TEST);
         glDisable(GL_LIGHTING);
         glDisable(GL_COLOR_MATERIAL);
         if( !blackWireFrame )
            glColor4f(1,1,1,0.2f);
         else
            glColor4f(0,0,0,0.2f);

			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
         glDepthMask(false);
         glEnable(GL_LINE_SMOOTH);
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

      glPushAttrib(GL_ENABLE_BIT);
      if( drawNormalsVectors && hasNormal)
      {
         glBegin( GL_LINES );
         glColor3f( 1,0,0);
         for( card32 i=0;i<pts->getNumEntries();i++ )
         {
            Vector3f pos = pts->get<float32,3>(i, POS );
            Vector3f normal = pts->get<float32,3>(i, NRM );
            Vector3f p2 = pos + normal * normalLength;
            glVertex3fv( &pos[0] );
            glVertex3fv( &p2[0] );
         }
         glEnd();
      }

      glPopAttrib();
   }
}



