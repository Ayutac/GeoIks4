//---------------------------------------------------------------------------
#ifndef KDTreeInlineH
#define KDTreeInlineH
//---------------------------------------------------------------------------
#include "KDTree.h"
#include <stddef.h>
//---------------------------------------------------------------------------


template <class ObjectType, typename FloatType, unsigned dimension>
inline KDTree<ObjectType, FloatType, dimension>::KDTree() {
   splitDim = 0;
   lower = NULL;
   upper = NULL;
	//splitPlanePos = 0;
}

template <class ObjectType, typename FloatType, unsigned dimension>
KDTree<ObjectType, FloatType, dimension>::KDTree(vector<ObjectType> objs, unsigned splitDim, size_t minNodeSize) {
	this->splitDim = splitDim;
	lower = NULL;
	upper = NULL;
	size_t numObjs = objs.size();
	if (numObjs < minNodeSize) {
		this->nodeObjects = objs;
	} else {
		CoordinateComperator comp(splitDim);
		sort(objs.begin(), objs.end(), comp);
		ObjectType center = objs[numObjs/2];
		FloatType splitPlanePos = center.getCentroid()[splitDim];
		splitPlaneBoundaryLowerNodeUpperBound = -1E30f;
		splitPlaneBoundaryUpperNodeLowerBound = 1E30f;

		vector<ObjectType> lowerList;
		vector<ObjectType> upperList;
		for (size_t i=0; i<numObjs; i++) {
			FloatType centroidCoord = objs[i].getCentroid(splitDim);
			bool sortIntoUpper = false;
			if (centroidCoord >= splitPlanePos) {
				sortIntoUpper = true;
			}

			if (sortIntoUpper) {
				upperList.push_back(objs[i]); 
				splitPlaneBoundaryUpperNodeLowerBound = min(splitPlaneBoundaryUpperNodeLowerBound, objs[i].getMinBB(splitDim));
			} else {
				lowerList.push_back(objs[i]); 
				splitPlaneBoundaryLowerNodeUpperBound = max(splitPlaneBoundaryLowerNodeUpperBound, objs[i].getMaxBB(splitDim));
			}
		}
		if (lowerList.size() == objs.size() || upperList.size() == objs.size()) {
			this->nodeObjects = objs;
		} else {
			if (!lowerList.empty()) lower = new KDTree(lowerList, (splitDim+1) % dimension, minNodeSize);
			if (!upperList.empty()) upper = new KDTree(upperList, (splitDim+1) % dimension, minNodeSize);
		}
	}
}


template <class ObjectType, typename FloatType, unsigned dimension>
const vector<ObjectType> &KDTree<ObjectType, FloatType, dimension>::getNodeObjects() {
   return nodeObjects;
}

template <class ObjectType, typename FloatType, unsigned dimension>
KDTree<ObjectType, FloatType, dimension> *KDTree<ObjectType, FloatType, dimension>::getLower() {
   return lower;
}

template <class ObjectType, typename FloatType, unsigned dimension>
KDTree<ObjectType, FloatType, dimension> *KDTree<ObjectType, FloatType, dimension>::getUpper() {
   return upper;
}

template <class ObjectType, typename FloatType, unsigned dimension>
unsigned KDTree<ObjectType, FloatType, dimension>::getSplitDim() {
   return splitDim;
}

template <class ObjectType, typename FloatType, unsigned dimension>
FloatType KDTree<ObjectType, FloatType, dimension>::getSplitPlaneBoundaryLowerNodeUpperBound()
{
	return splitPlaneBoundaryLowerNodeUpperBound;
}

template <class ObjectType, typename FloatType, unsigned dimension>
FloatType KDTree<ObjectType, FloatType, dimension>::getSplitPlaneBoundaryUpperNodeLowerBound()
{
	return splitPlaneBoundaryUpperNodeLowerBound;
}

template <class ObjectType, typename FloatType, unsigned dimension>
KDTree<ObjectType, FloatType, dimension>::~KDTree()
{
	delete lower;
	delete upper;
}



#endif
