//---------------------------------------------------------------------------
#include "precompiledHeaders.h"
//---------------------------------------------------------------------------
#include "PointCloud.h"
//---------------------------------------------------------------------------
#include "DynamicArrayOfStructures.h"
#include "DynamicArrayOfStructures.hpp"
#include "ObjectClassProperty.h"
#include "Tools.h"
#include "StructureDescriptor.hpp"
#include "GLFixedFunctionPointCloudView.h"
#include "OpenGLCanvas3D.h"
//---------------------------------------------------------------------------


IMPLEMENT_GEOX_CLASS(PointCloud, 0) {
	BEGIN_CLASS_INIT( PointCloud );
   ADD_OBJECT_PROP( points, 0, DynamicArrayOfStructures::getClass(), true );
}


// ----



PointCloud::PointCloud() {
   points = new DynamicArrayOfStructures();
}

void PointCloud::assign(const Object* obj) {
   const PointCloud *pobj = dynamic_cast<const PointCloud*>(obj);
   pAssert(pobj != NULL);
   if (pobj->points == NULL) {
      delete points;
      points = NULL;
   } else if (points != pobj->points) {
      delete points;
		points = (DynamicArrayOfStructures*)(pobj->points)->copy();
   }
}

const StructureDescriptor *PointCloud::getDescr() const {
   return points->getDescr();
}

card64 PointCloud::getNumPoints() const {
	if (points == NULL) return 0;
	return points->getNumEntries();
}


void PointCloud::clear() {
   points->clear();
}


void PointCloud::clearAndSetup(const StructureDescriptor *vertDescr, mpcard numPts) {
   points->clearAndSetup(numPts, vertDescr);
}


void PointCloud::clearAndSetup( mpcard numPoints, bool hasPosition/*=true*/, bool hasNormal/*=true*/, bool hasColor/*=true*/, bool hasFlags/*=true*/ )
{
	StructureDescriptor vd;
	if( hasPosition )
		vd.pushAttrib(SAD("position",3,SAD::DATA_FORMAT_FLOAT32));
	if( hasNormal )
		vd.pushAttrib(SAD("normal",3,SAD::DATA_FORMAT_FLOAT32));
	if( hasColor )
		vd.pushAttrib(SAD("color",3,SAD::DATA_FORMAT_FLOAT32));
	if( hasFlags )
		vd.pushAttrib(SAD("flags",1,SAD::DATA_FORMAT_INT32));
	clearAndSetup(&vd,numPoints);
}

Vector3f PointCloud::computeCenterOfMass() const
{
	Vector3f result = NULL_VECTOR3F;
	if (getNumPoints() < 1)
		return result;
	
	AAT POS = getAAT("position");
	for (mpcard i = 0; i < getNumPoints(); ++i)
		result += points->get3f(i, POS);
	result /= (float32)getNumPoints();
	return result;	
}

PointCloud *PointCloud::createCopyWithNewPointSet(DynamicArrayOfStructures *ps) const {
   DynamicArrayOfStructures *pscopy = this->points;
   ((PointCloud*)this)->points = NULL;
   PointCloud *result = (PointCloud*)copy();
   ((PointCloud*)this)->points = pscopy;
   result->points = ps;
   return result;
}

const MetaClass *PointCloud::getDefaultViewer()
{
   return GLFixedFunctionPointCloudView::getClass();
}

const MetaClass *PointCloud::getDefaultViewerDefaultCanvas()
{
   return OpenGLCanvas3D::getClass();
}


PointCloud::~PointCloud() {
   delete points;
}



