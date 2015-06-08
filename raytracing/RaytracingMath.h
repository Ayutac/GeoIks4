//---------------------------------------------------------------------------
#ifndef RaytracingMathH
#define RaytracingMathH
//---------------------------------------------------------------------------
#include "PTypes.h"
#include "LinearAlgebra.h"
#include "BoundingBox.h"
#include "Sphere.h"
//---------------------------------------------------------------------------
 

/// This is a library with a number of more or less useful routines for raytracing.
/// Comes without warrenty (taken from an old project); the code should work correctly, but not all parts are tested extensively.


/// A ray; nothing special.
template<class FloatType, unsigned int dimension>
class Ray {
 public:
   StaticVector<FloatType, dimension> origin;
   StaticVector<FloatType, dimension> direction;
   Ray() {}
   Ray(StaticVector<FloatType, dimension> origin, StaticVector<FloatType, dimension> direction);
};

/// 3D ray with float numbers
typedef Ray<float32, 3> Ray3f;


/// if the scene is transformed by "transform", the ray must be tranformed by "invTranform" = transform^-1.
inline Ray3f transformRay(const Ray3f ray, const Matrix4f &invTransform);

/// intersection tests...
bool intersectLineBoundingBox3f(const Ray3f &line, const BoundingBox3f &bb);

/// intersection tests...
bool intersectBoundingBox3f(const Ray3f &ray, const BoundingBox3f &bb, float32 &firstHitParam, float32 &secondHitParam);

/// intersection tests...
bool intersectTriangle3f(const Ray3f &ray, const Vector3f &p1, const Vector3f &p2, const Vector3f &p3,
                         float32 &hitParam);

/// intersection tests...
bool intersectTriangle3f(const Ray3f &ray, const Vector3f &p1, const Vector3f &p2, const Vector3f &p3,
                         float32 &hitParam, float32 &firstTriangleParam, float32 &secondTriangleParam);

/// intersection tests...
bool intersectParallelogram3f(const Ray3f &ray, const Vector3f &p1, const Vector3f &p2, const Vector3f &p3, float32 &hitParam);


/// squared distance ray <-> point
float32 minDistQuadRayToPoint(const Ray3f &ray, const Vector3f &point);

/** returns true if ray intersects plane (at any ray parameter). rayParameter is invalid if no intersection is found. */
bool intersectPlaneRay(const Ray3f &ray, const Vector3f &planeNormal, const Vector3f &planeOrigin,
                       float32 &rayParam);

/** returns true if the minimal distance between the two lines is found at a line segment parameter
    within the range [0..1]. in this case, distQuad is set to the squared minimal distance */
bool calcDistToLine(const Ray3f &lineSegmentRay, const Ray3f &ray,
                    float32 &distQuad, float32 &rayParam, float32 &lineSegParam);


/// intersection tests...
inline void intersectXPlane(const Ray3f &ray, const float32 xPlane, float32 &rayParam, float32 &yIntersect, float32 &zIntersect);
/// intersection tests...
inline void intersectYPlane(const Ray3f &ray, const float32 yPlane, float32 &rayParam, float32 &xIntersect, float32 &zIntersect);
/// intersection tests...
inline void intersectZPlane(const Ray3f &ray, const float32 zPlane, float32 &rayParam, float32 &xIntersect, float32 &yIntersect);

/// intersection tests...
inline bool intersectBoundingSphere3f(const Sphere3f &sphere, const Ray3f &ray);


/// assumes normalized "normal"
inline Ray3f mirrorRay(const Ray3f &ray, const Vector3f &point, const Vector3f &normal);

/// assumes normalized "normal", normal * v > 0
inline Vector3f mirrorVector(const Vector3f& v, const Vector3f &normal);

extern Vector3f refractNormal(const Vector3f &incomming, const Vector3f &surfaceNormal, const float32 inOutDensityQuotient);

/// calcs ray param of ray-plane intersection. assumtion: ray / plane are not parallel
extern float32 intersectPlane3f(const Vector3f &p1, const Vector3f &p2, const Vector3f &p3, const Ray3f &ray);

/// calcs ray param with min. dist to another line
extern void minLineDistRayParam3f(const Vector3f &rayOrigin, const Vector3f &rayDirection,
                                  const Vector3f &lineOrigin, const Vector3f &lineDirection,
                                  float32 &rayParam, float32 &lineParam);
                                     


#endif


