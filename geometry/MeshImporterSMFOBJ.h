//---------------------------------------------------------------------------
#ifndef MeshImporterSMFOBJ_H
#define MeshImporterSMFOBJ_H
//---------------------------------------------------------------------------
#include "Persistent.h"
#include "LinearAlgebra.hpp"
//---------------------------------------------------------------------------

class TriangleMesh;


/// Import Reader for SimpleModelFormat(SMF) or OBJ files
/// smf: should work with most basic smf-files but doesn't support textures, transformations or grouping
/// obj: should work with simple obj files but doesn't support many features
class MeshImporterSMFOBJ : public Persistent {
   GEOX_CLASS(MeshImporterSMFOBJ)
 private:
	/// holds all positions
   vector<Vector3f> positions;
	/// holds all colors
   vector<Vector3f> colors;
	/// holds all normals
   vector<Vector3f> normals;
	/// holds all faces
   vector<Vector3i> faces;
   /// polygon ids of face.
   vector<card32>	facePolyID;
	/// holds all texture coordinates
   vector<Vector2f> textureCoords;


 public:
   MeshImporterSMFOBJ();

   bool performImport(string filename);

   void getPositions(vector<Vector3f> &positions);
   void getColors(vector<Vector3f> &colors);
   void getNormals(vector<Vector3f> &normals);
   void getFaceIndices(vector<Vector3i> &faces);
   void getTextureCoordinates(vector<Vector2f> &textureCoords);

   TriangleMesh *createTriangleMesh();

   ~MeshImporterSMFOBJ();
};



#endif
