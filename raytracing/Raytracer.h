//---------------------------------------------------------------------------
#ifndef RaytracerH
#define RaytracerH
//---------------------------------------------------------------------------
#include "PTypes.h"
#include "Camera.h"
#include "ViewFrustum.h"
//---------------------------------------------------------------------------
#include <qimage.h>
#include "RaytracingMath.h"
#include "KDTree.h"
#include "ViewerImgUpdateCallBack.h"
//---------------------------------------------------------------------------


class TriangleMesh;
class RaytracingMaterial;


/// My simple raytracer...
///
/// It uses a kdTree with overlapping bounding volumes and iterated split of the dimensions (this was fastest)
/// splitting the longest dimension increases runtime by about 30%
/// storing triangles that intersect the split plane in inner nodes increases the runtime by a factor of 10
///
/// Note: To adapt the class for more complex rendering, the function "traceRayKDTree" might be most useful.
///       In case, make it public.
///
class  Raytracer : public Persistent {
	GEOX_CLASS(Raytracer)
 private:          

	/// this class is an internal helper to represent triangles
	/// it is used to instantiate the templated KDtree
	/// see the KDTree comments for details
	struct Triangle {
		Vector3f vertices[3];
		Vector3f color;
		card32 material;
		int index;

		/// get centroid of triangle
		inline Vector3f getCentroid() const {return (vertices[0]+vertices[1]+vertices[2])*(1.0f/3.0f);}
		/// get centroid by dimension
		inline float32 getCentroid(unsigned dim) const {return (vertices[0][dim]+vertices[1][dim]+vertices[2][dim])*(1.0f/3.0f);}
		/// get minimum extend in dimension
		inline float32 getMinBB(unsigned dim) {return min(vertices[0][dim], min(vertices[1][dim], vertices[2][dim]));}
		/// get maximum extend in dimension
		inline float32 getMaxBB(unsigned dim) {return max(vertices[0][dim], max(vertices[1][dim], vertices[2][dim]));}
	};

	/// Here I instantiate my KD tree for the triangles defined above.
	typedef KDTree<Triangle, float32, 3> TriangleKDTree;

	// volatile state -- these variables are temporaries that represent the scene and the kdTree (cached over multiple renders)
	vector<TriangleMesh*> objects;
	vector<RaytracingMaterial*> materials;
	vector<Triangle> triangles;
	TriangleKDTree *kdTree;
	BoundingBox3f trianglesBB;
	bool first; // for updating BB
	card32 numLeafTriangles;

   // persistent state
   bool useKDTree;

	/// shot a ray (including floor)
	bool traceRay(const Ray3f &ray, Vector3f &color, float32 groundPlane);
	/// compute the color - very simple shader right now.
	Vector3f computeColor(const Ray3f &ray, float32 firstHit, size_t triangleIndex);
	/// trace a ray using the kdTree - this is the core routine!
	bool traceRayKDTree(TriangleKDTree *node, const Ray3f &ray, float32 minParam, float32 maxParam, float32 &firstHit, int &triangleIndex);
	/// trace a ray using the kdTree, but this time only simulated with GL output
	void traceRayVis(const Ray3f &ray);
	/// recursive method called by traceRayVis
	bool traceRayKDTreeVis(TriangleKDTree *node, const Ray3f &ray, float32 minParam, float32 maxParam, float32 &firstHit, int &triangleIndex, BoundingBox3f bb);

	/// visualize the kdTree using OpenGL
	void renderKDTreeGLRecursive(TriangleKDTree *node, BoundingBox3f bb);

public:
	/// create a new raytracer. contains no objects initially.
	Raytracer();

	/// change the maximum number of triangles per leaf node. 1..20 work, with 1 being fastest (but memory intense)
	void setNumLeafTriangles(card32 numLeafTriangles);

	/// add a triangle mesh to the raytracer. This object takes the ownership, 
	/// i.e., it will delete the mesh in the destructor (in case, make a copy before passing the mesh to this method)
	void addObject(TriangleMesh *mesh, RaytracingMaterial *mat);
	/// delete all the meshes and aux. structures
	void clearObjects();
	/// rebuild the kdTree after changing parameters (numLeafTriangles); keep scene
	void rebuildKDTree();
	/// perform rendering. the displayCallback will be called each 250ms to give user feedback. No action if NULL.
	void render(QImage &result, card32 width, card32 height, Camera cam, ViewFrustum vf, float32 groundPlane, ViewerImgUpdateCallBack *displayCallback);

	/// debugging / visualization: display the kdTree using OpenGL
	void renderKDTreeGL();
	/// debugging / visualization: display the center ray using OpenGL
	void renderCenterRayGL(card32 width, card32 height, Camera cam, ViewFrustum vf);

	~Raytracer();

};





#endif
