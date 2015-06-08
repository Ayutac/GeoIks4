//---------------------------------------------------------------------------
#ifndef SimpleGLTriMeshMaterialH
#define SimpleGLTriMeshMaterialH
//---------------------------------------------------------------------------
#include "Persistent.h"
//---------------------------------------------------------------------------

class TriangleMesh;


class SimpleGLMeshMaterial : public Persistent {
	GEOX_CLASS(SimpleGLMeshMaterial)
 private:
   bool wireframe;
   bool drawNormalsVectors;
   float normalLength;
   bool normalsDirectionColors;
   bool useVertexNormals;
   bool showWireFrameOnly;
   bool showWireFrameOnlyDepthTest;
   bool blackWireFrame;
	bool cullBackFace;

 public:
   SimpleGLMeshMaterial();

   virtual void draw( TriangleMesh *mesh );

};




#endif
