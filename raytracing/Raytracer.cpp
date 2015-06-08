#include "precompiledHeaders.h"

#include "Raytracer.h"
#include "RaytracingMath.hpp"
#include "DynamicArrayOfStructures.hpp"
#include "RaytracingMath.h"
#include <qrgb.h>
#include "TriangleMesh.h"
#include "RaytracingMaterial.h"
#include "KDTree.hpp"
#include "Timer.h"
#include "MathTools.h"
#include "MathTools.hpp"
#include "BooleanClassProperty.h"
#include <GL/gl.h>


IMPLEMENT_GEOX_CLASS(Raytracer, 0) {
	BEGIN_CLASS_INIT(Raytracer);
   ADD_BOOLEAN_PROP(useKDTree,0)
}

Raytracer::Raytracer() {
	kdTree = NULL;
   useKDTree = true;
	first = true;
	numLeafTriangles = 1;
}

Raytracer::~Raytracer()
{
	clearObjects();
	delete kdTree;
}


void Raytracer::addObject(TriangleMesh *mesh, RaytracingMaterial *mat)
{
	objects.push_back(mesh);
	materials.push_back(mat);

	Timer timer;	
	timer.getDeltaValue();

	for (size_t i=0; i<objects.size(); i++) {
		TriangleMesh *mesh = objects[i];
		DynamicArrayOfStructures *meshVertices = mesh->getVertices();
		DynamicArrayOfStructures *meshTriangles = mesh->getTriangles();
		const mpcard numVertices = meshVertices->getNumEntries();
		const mpcard numTriangles = meshTriangles->getNumEntries();
		AAT positionAAT = meshVertices->getAAT("position", 3, SAD::DATA_FORMAT_FLOAT32);
		AAT colorAAT = NULL_AAT; 
		if (meshVertices->providesAttribute("color", 3, SAD::DATA_FORMAT_FLOAT32)) {
			colorAAT = meshVertices->getAAT("color", 3, SAD::DATA_FORMAT_FLOAT32);
		}
		AAT indexAAT = meshTriangles->getAAT("index", 3, SAD::DATA_FORMAT_INT32);
		for (mpcard t=0; t<numTriangles; t++) {
			Vector3i indices = meshTriangles->get3i(t, indexAAT);
			for (int j=0; j<3; j++) {
				if (indices[j] < 0 || indices[j] >= (int)numVertices) {
					output << "warning: triangle index out of range\n";
					continue;
				}
			}
			Triangle tri;
			tri.color = NULL_VECTOR3F;
			for (int j=0; j<3; j++) {
				tri.vertices[j] = meshVertices->get3f( indices[j], positionAAT );
				if (colorAAT != NULL_AAT) {
					tri.color += meshVertices->get3f( indices[j], colorAAT );
				} else {
					tri.color += makeVector3f(1,1,1);
				}
			}
			tri.color /= 3.0f;
         tri.material = (card32)i;
         tri.index = (int)triangles.size();
			triangles.push_back(tri);
			if (first) {
				trianglesBB = BoundingBox3f(tri.vertices[0]);
				first = false;
			}
			for (int j=0; j<3; j++) trianglesBB.addPoint(tri.vertices[j]);
		}
	}


	delete kdTree;
	kdTree = new TriangleKDTree(triangles, 0, numLeafTriangles);
	output << "build kD tree with at most " << numLeafTriangles << " triangles per leaf node: " << timer.getDeltaValue() << "[ms]\n";
}

static inline int clamp(float v) {
	int result = v*255;
	if (result > 255) result = 255;
	if (result <0) result = 0;
	return result;
}

void Raytracer::render(QImage &result, card32 width, card32 height, Camera cam, ViewFrustum vf, float32 groundPlane, ViewerImgUpdateCallBack *displayCallback)
{
	Timer timer;	
	card32 lastTime = timer.getCurrentValue();

	result = QImage(width, height, QImage::Format_RGB32);

	float32 tanOfHalfViewingAngle = tan(vf.getVerticalFieldOfView()/180.0f*M_PI/2.0f);
	Vector3f up = cam.getOrthoNormUpDirection() * tanOfHalfViewingAngle;
	Vector3f right = cam.getOrthoNormRightDirection() * tanOfHalfViewingAngle;
	Vector3f view = cam.getOrthoNormViewDirection();
	Ray3f ray;
	ray.origin = cam.getPosition();
	float32 aspectRatio = (float32)width/(float32)height;
	for (card32 y=0; y<height; y++) {
		float32 rely = 2.0f * (((int32)height)/2 - (int32)y) / (float32)height;
		for (card32 x=0; x<width; x++) {
			float32 relx = 2.0f * (((int32)width)/2 - (int32)x) / (float32)width * aspectRatio;
			ray.direction = view + right * relx + up *rely;
			Vector3f col = NULL_VECTOR3F;
			traceRay(ray, col, groundPlane);
			result.setPixel(x,y, qRgb(clamp(col[0]), clamp(col[1]), clamp(col[2])));
		}
		if (timer.getCurrentValue() - lastTime >= 250) {
			lastTime = timer.getCurrentValue();
			if (displayCallback) displayCallback->viewImage(result);
		}

	}
	output << "rendering time: " << timer.getDeltaValue() << "[ms]\n";
}

void Raytracer::clearObjects()
{
	for (size_t i=0; i<objects.size(); i++) delete objects[i];
	objects.clear();
	for (size_t i=0; i<materials.size(); i++) delete materials[i];
	materials.clear();
	triangles.clear();
	first = true;
	delete kdTree;
	kdTree = NULL;
}

bool Raytracer::traceRay(const Ray3f &ray, Vector3f &color, float32 groundPlane)
{
	bool hit = false;
	float32 hitParam = 1.0e20f;
   float32 rayParam = 0;
	float32 xIntersect;
	float32 yIntersect;
	if (ray.direction[1] != 0) {
		intersectYPlane(ray, groundPlane, rayParam, xIntersect, yIntersect);
		if (rayParam > 0) {
			hitParam = rayParam;
			int xint = (int)xIntersect;
			int yint = (int)yIntersect;
			color = ((xint+yint)%2) == 0 ? makeVector3f(0.3f,0.4f,0.35f) : makeVector3f(0.9f,0.95f,0.93f);
			hit = true;
		}
	}
	float32 firstBB, lastBB;
	bool hitObjBB = intersectBoundingBox3f(ray, trianglesBB, firstBB, lastBB);
	if (hitObjBB) {
      if (useKDTree) {
         int triangleIndex;
			if (rayParam > 0) lastBB = min(rayParam, lastBB);
			float32 firstHit = lastBB;
         bool hitSomething = traceRayKDTree(kdTree, ray, firstBB, lastBB, firstHit, triangleIndex);
         if (hitSomething) {
            color = computeColor(ray, firstHit, triangleIndex);
         }

      } else {
		   for (size_t i=0; i<triangles.size(); i++) {
			   float32 hitParamNew;
			   bool newHit = intersectTriangle3f(ray, triangles[i].vertices[0], triangles[i].vertices[1], triangles[i].vertices[2], hitParamNew);
			   if (newHit) {
				   hit = true;
				   if (hitParamNew < hitParam) {
					   hitParam = hitParamNew;
					   color = computeColor(ray, newHit, i);
				   }
			   }
		   }
      }
	}
	return hit;
}

Vector3f Raytracer::computeColor(const Ray3f &ray, float32 firstHit, size_t triangleIndex)
{
	UNUSED(ray);
	UNUSED(firstHit);
	Triangle &tri = triangles[triangleIndex];
	Vector3f normal = (tri.vertices[1]-tri.vertices[0]).crossProduct(tri.vertices[2]-tri.vertices[0]);
	float32 l = norm(normal);
	if (l>0) normal *= 1/l;
	float32 diff = fabs(normal[2]);
	return triangles[triangleIndex].color*(diff);
}

/// this is the core method
bool Raytracer::traceRayKDTree(
		TriangleKDTree *node, 
		const Ray3f &ray,  
		float32 minParam, 
		float32 maxParam,
		float32 &firstHit,
		int &triangleIndex)
{
	// -- first, test all inner triangles (if they exist)
	//    in the current version, nodes with triangles are always leaf nodes
   bool hit = false;
	const vector<Triangle> &nodeTri = node->getNodeObjects();
   const size_t numTri = nodeTri.size();
	for (size_t i=0; i<numTri; i++) {
		const Triangle &tri = nodeTri[i];
		float32 hitParam;
		if (intersectTriangle3f(ray, tri.vertices[0], tri.vertices[1], tri.vertices[2], hitParam)) {
		   if (hitParam <= firstHit) {
			   triangleIndex = tri.index;
			   firstHit = hitParam;
            hit = true;
		   }
      }
	}

	// -- compute ray-split plane intersection (for overlapping nodes)
	float32 rayParamPlaneUpper = (node->getSplitPlaneBoundaryUpperNodeLowerBound() - ray.origin[node->getSplitDim()]) / ray.direction[node->getSplitDim()];
	float32 rayParamPlaneLower = (node->getSplitPlaneBoundaryLowerNodeUpperBound() - ray.origin[node->getSplitDim()]) / ray.direction[node->getSplitDim()];
	 
	// -- case one - node traverses from lower to upper
	if (ray.direction[node->getSplitDim()] >= 0) {
		if (node->getLower() && rayParamPlaneLower >= minParam) {
			if (traceRayKDTree   (node->getLower(), ray, minParam, min(firstHit, min(maxParam, rayParamPlaneLower)), firstHit, triangleIndex)) hit=true;
		}

		if (node->getUpper() && rayParamPlaneUpper < maxParam) {
			if (traceRayKDTree   (node->getUpper(), ray, max(minParam, rayParamPlaneUpper), min(firstHit, maxParam), firstHit, triangleIndex)) hit=true;
		}
	// -- case two - node traverses from upper to lower
	} else {
		if (node->getUpper() && rayParamPlaneUpper >= minParam) {
			if (traceRayKDTree   (node->getUpper(), ray, minParam, min(firstHit, min(maxParam, rayParamPlaneUpper)), firstHit, triangleIndex)) hit=true;
		}

		if (node->getLower() && rayParamPlaneLower < maxParam) {
			if (traceRayKDTree   (node->getLower(), ray, max(minParam, rayParamPlaneLower), min(firstHit, maxParam), firstHit, triangleIndex)) hit=true;
		}
	}
   return hit;
}

static void renderLine( Vector3f a, Vector3f b ) 
{
	glVertex3f(a[0], a[1], a[2]);
	glVertex3f(b[0], b[1], b[2]);
}

static void renderBox(const BoundingBox3f& bbox)
{
	Vector3f p0 = bbox.lowerCorner;
	Vector3f p1 = p0 + makeVector3f( bbox.getSideLength( 0 ), 0, 0 );
	Vector3f p2 = p0 + makeVector3f( bbox.getSideLength( 0 ), bbox.getSideLength( 1 ), 0 );
	Vector3f p3 = p0 + makeVector3f( 0, bbox.getSideLength( 1 ), 0 );
	Vector3f p4 = p0 + makeVector3f( 0, 0, bbox.getSideLength( 2 ) );
	Vector3f p5 = p1 + makeVector3f( 0, 0, bbox.getSideLength( 2 ) );
	Vector3f p6 = p2 + makeVector3f( 0, 0, bbox.getSideLength( 2 ) );
	Vector3f p7 = p3 + makeVector3f( 0, 0, bbox.getSideLength( 2 ) );
	renderLine( p0, p1 );
	renderLine( p1, p2 );
	renderLine( p2, p3 );
	renderLine( p3, p0 );
	renderLine( p0, p4 );
	renderLine( p1, p5 );
	renderLine( p2, p6 );
	renderLine( p3, p7 );
	renderLine( p4, p5 );
	renderLine( p5, p6 );
	renderLine( p6, p7 );
	renderLine( p7, p4 );
}



void Raytracer::renderKDTreeGL()
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);	
	glEnable(GL_DEPTH_TEST);
	glDisable( GL_LIGHTING );
	glColor3f( 0.75f, 0.75f, 0.75f );
	glLineWidth(1);
	glBegin( GL_LINES );
	if (kdTree) renderKDTreeGLRecursive(kdTree, trianglesBB);
	glEnd();
	glPopAttrib();
}

void Raytracer::renderKDTreeGLRecursive(TriangleKDTree *node, BoundingBox3f bb)
{
	renderBox(bb);
	if (node->getLower()) {
		BoundingBox3f bb2 = bb;
		bb2.upperCorner[node->getSplitDim()] = node->getSplitPlaneBoundaryLowerNodeUpperBound();
		renderKDTreeGLRecursive(node->getLower(), bb2);
	}
	if (node->getUpper()) {
		BoundingBox3f bb2 = bb;
		bb2.lowerCorner[node->getSplitDim()] = node->getSplitPlaneBoundaryUpperNodeLowerBound();
		renderKDTreeGLRecursive(node->getUpper(), bb2);
	}
}


/// copy from above, but with visualization
bool Raytracer::traceRayKDTreeVis(
	TriangleKDTree *node, 
	const Ray3f &ray,  
	float32 minParam, 
	float32 maxParam,
	float32 &firstHit,
	int &triangleIndex,
	BoundingBox3f bb)
{
	if (maxParam < minParam) return false;
	renderBox(bb);
	BoundingBox3f bbLower = bb;
	BoundingBox3f bbUpper = bb;
	if (node->getLower()) {
		bbLower.upperCorner[node->getSplitDim()] = node->getSplitPlaneBoundaryLowerNodeUpperBound();
	}
	if (node->getUpper()) {
		bbUpper.lowerCorner[node->getSplitDim()] = node->getSplitPlaneBoundaryUpperNodeLowerBound();
	}

	bool hit = false;
	const vector<Triangle> &nodeTri = node->getNodeObjects();
	const size_t numTri = nodeTri.size();
	for (size_t i=0; i<numTri; i++) {
		const Triangle &tri = nodeTri[i];
		float32 hitParam;
		if (intersectTriangle3f(ray, tri.vertices[0], tri.vertices[1], tri.vertices[2], hitParam)) {
			if (hitParam <= firstHit) {
				triangleIndex = tri.index;
				firstHit = hitParam;
				glColor3f(0.1f,0.5f,0.1f);
				glVertex3fv(tri.vertices[0].data());
				glVertex3fv(tri.vertices[1].data());

				glVertex3fv(tri.vertices[1].data());
				glVertex3fv(tri.vertices[2].data());

				glVertex3fv(tri.vertices[2].data());
				glVertex3fv(tri.vertices[0].data());
				glColor4f( 1.0f, 0.05f, 0.25f, 0.2f );
				hit = true;
			}
		}
	}

	float32 rayParamPlaneUpper = (node->getSplitPlaneBoundaryUpperNodeLowerBound() - ray.origin[node->getSplitDim()]) / ray.direction[node->getSplitDim()];
	float32 rayParamPlaneLower = (node->getSplitPlaneBoundaryLowerNodeUpperBound() - ray.origin[node->getSplitDim()]) / ray.direction[node->getSplitDim()];

	/// case one - node traverses from lower to upper
	if (ray.direction[node->getSplitDim()] >= 0) {
		if (node->getLower() && rayParamPlaneLower >= minParam) {
			if (traceRayKDTreeVis(node->getLower(), ray, minParam, min(firstHit, min(maxParam, rayParamPlaneLower)), firstHit, triangleIndex, bbLower)) hit=true;
		}

		if (node->getUpper() && rayParamPlaneUpper < maxParam) {
			if (traceRayKDTreeVis(node->getUpper(), ray, max(minParam, rayParamPlaneUpper), min(firstHit, maxParam), firstHit, triangleIndex, bbUpper)) hit=true;
		}
		/// case two - node traverses from upper to lower
	} else {
		if (node->getUpper() && rayParamPlaneUpper >= minParam) {
			if (traceRayKDTreeVis(node->getUpper(), ray, minParam, min(firstHit, min(maxParam, rayParamPlaneUpper)), firstHit, triangleIndex, bbUpper)) hit=true;
		}

		if (node->getLower() && rayParamPlaneLower < maxParam) {
			if (traceRayKDTreeVis(node->getLower(), ray, max(minParam, rayParamPlaneLower), min(firstHit, maxParam), firstHit, triangleIndex, bbLower)) hit=true;
		}
	}
	return hit;
}


void Raytracer::traceRayVis(const Ray3f &ray)
{
	float32 firstBB, lastBB;
	bool hitObjBB = intersectBoundingBox3f(ray, trianglesBB, firstBB, lastBB);
	if (hitObjBB) {
		int triangleIndex;
		float32 firstHit = lastBB;
		traceRayKDTreeVis(kdTree, ray, firstBB, lastBB, firstHit, triangleIndex, trianglesBB);
	}
}

void Raytracer::renderCenterRayGL(card32 width, card32 height, Camera cam, ViewFrustum vf)
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);	
	glDisable( GL_LIGHTING );
	glEnable(GL_DEPTH_TEST);
	glDepthMask(false);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glEnable(GL_BLEND);
	glLineWidth(5);
	glBegin( GL_LINES );
	float32 tanOfHalfViewingAngle = tan(vf.getVerticalFieldOfView()/180.0f*M_PI/2.0f);
	Vector3f up = cam.getOrthoNormUpDirection() * tanOfHalfViewingAngle;
	Vector3f right = cam.getOrthoNormRightDirection() * tanOfHalfViewingAngle;
	Vector3f view = cam.getOrthoNormViewDirection();
	Ray3f ray;
	ray.origin = cam.getPosition();
	float32 aspectRatio = (float32)width/(float32)height;
	card32 y = height/2;
	float32 rely = 2.0f * (((int32)height)/2 - (int32)y) / (float32)height;
	card32 x = width/2;
	float32 relx = 2.0f * (((int32)width)/2 - (int32)x) / (float32)width * aspectRatio;
	ray.direction = view + right * relx + up *rely;
	glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
	glVertex3fv( ray.origin.data() );
	glVertex3fv( (ray.origin + ray.direction*1.0E5f).data() );

	if (kdTree) {
		glColor4f( 1.0f, 0.05f, 0.25f, 0.2f );
		traceRayVis(ray);
	}

	glEnd();
	glPopAttrib();
}

void Raytracer::setNumLeafTriangles(card32 numLeafTriangles)
{
	this->numLeafTriangles = numLeafTriangles;
}


void Raytracer::rebuildKDTree()
{
	if (!triangles.empty()) {
		Timer timer;	
		timer.getDeltaValue();
		delete kdTree;
		kdTree = new TriangleKDTree(triangles, 0, numLeafTriangles);
		output << "build kD tree with at most " << numLeafTriangles << " triangles per leaf node: " << timer.getDeltaValue() << "[ms]\n";
	}
}
