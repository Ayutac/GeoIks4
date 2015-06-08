//---------------------------------------------------------------------------
#ifndef TriangleMeshH
#define TriangleMeshH
//---------------------------------------------------------------------------
#include "PointCloud.h"
//---------------------------------------------------------------------------


/// A simple triangle mesh: vertices and faces, both with attributes channels.
/// The triangle array stores indices of vertices in the "index" channel (name:"index", dimension:3, type:DATA_FORMAT_INT32).
/// It this channel is missing, some code might not work (not impossible to setup though).
class  TriangleMesh : public PointCloud {
   GEOX_CLASS(TriangleMesh)
 private:
   DynamicArrayOfStructures *triangles;

 public:
   /// constructor: default mesh has 0 triangles, 0 vertices
   /// triangles have an "index"(3i) channel
   /// vertices have an "position"(3f) channel
   TriangleMesh();

   /// property access (read-const)
   const DynamicArrayOfStructures *getTriangles() const {return triangles;}
   /// property access (write)
   virtual void setTriangles(DynamicArrayOfStructures *psTriangles);
   /// property access (read-modifiable)
   virtual DynamicArrayOfStructures *getTriangles() {return triangles;}

   virtual void assign(const Object* obj);

   /// Create a new triangle mesh. Make sure to include "index" (3i) as a channel.
   void clearAndSetup(const StructureDescriptor *vertDescr, mpcard numPoints,
                      const StructureDescriptor *triangleDescr, mpcard numTriangles);

   /// create a typical mesh. you can choose vertex attributes.
   /// face attributes limited to "index"(3i)
   void clearAndSetup(mpcard numPoints, mpcard numTriangles, bool hasNormal=true, bool hasColor = true, bool hasFlags = true);

   /// empties vertices and triangles arrays
   virtual void clear();

   // inherited
   virtual const MetaClass *getDefaultViewer();
   // inherited
   virtual const MetaClass *getDefaultViewerDefaultCanvas();


   ~TriangleMesh();
};





#endif
