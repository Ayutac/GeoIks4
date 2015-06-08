//---------------------------------------------------------------------------
#ifndef PointCloudH
#define PointCloudH
//---------------------------------------------------------------------------
#include "DynamicArrayOfStructures.h"
//---------------------------------------------------------------------------


class BasicUnstructeredInCorePointCloudIterator;

/// a simple point cloud (list of points)
class  PointCloud : public GeoXData {
   GEOX_CLASS(PointCloud)
 protected:
   DynamicArrayOfStructures *points;

 public:
   PointCloud();

   virtual void assign(const Object* obj);

   AAT getAAT(string name) const {return points->getAAT(name);}
   virtual const StructureDescriptor *getDescr() const;
   virtual card64 getNumPoints() const;
   virtual void  clear();

   // --- new methods
   void clearAndSetup(const StructureDescriptor *vertDescr, mpcard numPts);

   /// shortcut for one of the most commonly used setup
   void clearAndSetup(mpcard numPoints, bool hasPosition=true, bool hasNormal=true, bool hasColor=true, bool hasFlags=true);

   virtual const DynamicArrayOfStructures *getVertices() const {return points;}
   virtual void setVertices(DynamicArrayOfStructures *ps) {delete this->points; this->points = ps;}
   virtual DynamicArrayOfStructures *getVertices() {return points;}
   virtual PointCloud *createCopyWithNewPointSet(DynamicArrayOfStructures *points) const; // ownership of ps will be transfered to this

   // ---

   Vector3f computeCenterOfMass() const;

   // inherited
   virtual const MetaClass *getDefaultViewer();
   // inherited
   virtual const MetaClass *getDefaultViewerDefaultCanvas();

   ~PointCloud();
};





#endif
