//---------------------------------------------------------------------------
#ifndef KDTreeH
#define KDTreeH
//---------------------------------------------------------------------------
#include "PTypes.h"
#include <vector>
using namespace std;
//---------------------------------------------------------------------------



/// A KD tree with variable dimension and flexible object storage.
/// the requirements for "ObjectType" are documented below:
///
/// expected from ObjectType:
///   
///   --- constructur / destructor (no parameters)
///   StaticVector<FloatType, dimension> getCentroid()
///
///   --- decide wether to put an object into lower or upper node along coordinate (or into the inner node)
///   BinaryNodeSortType sort(unsigned dim, FloatType coordinate);
///
/// PS: it is templated, which makes the code faster but not so easy to read.
///
template <class ObjectType, typename FloatType, unsigned dimension>
class  KDTree {
 private:          
   struct CoordinateComperator {
      unsigned dim;

      CoordinateComperator(unsigned dim) {this->dim = dim;}
      bool operator() (const ObjectType &a, const ObjectType &b) { return (a.getCentroid()[dim]<b.getCentroid()[dim]);}
   };

   vector<ObjectType> nodeObjects;
   unsigned splitDim;
	FloatType splitPlaneBoundaryLowerNodeUpperBound;
	FloatType splitPlaneBoundaryUpperNodeLowerBound;
   KDTree *lower;
   KDTree *upper;
	 
 public:
   inline KDTree();
   /// Build a KDTree recursively. The method is using a simple n*log^2 n build algorithm.
   /// minNodeSize specifies the stopping criterion for further splitting object lists.
   KDTree(vector<ObjectType> objs, unsigned splitDim, size_t minNodeSize = 1);

   inline const vector<ObjectType> &getNodeObjects();
   inline KDTree *getLower();
   inline KDTree *getUpper();
	inline FloatType getSplitPlaneBoundaryLowerNodeUpperBound();
	inline FloatType getSplitPlaneBoundaryUpperNodeLowerBound();
   inline unsigned getSplitDim();
	~KDTree();
};





#endif