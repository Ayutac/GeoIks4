//---------------------------------------------------------------------------
#ifndef RaytracingMathInlineH
#define RaytracingMathInlineH
//---------------------------------------------------------------------------
#include "RaytracingMath.h"
//---------------------------------------------------------------------------
#include "LinearAlgebra.hpp"
#include "BoundingBox.hpp"
#include "Sphere.h"
//---------------------------------------------------------------------------

/// inline / template implementation for RaytracingMath


template<class FloatType, unsigned int dimension>
Ray<FloatType, dimension>::Ray(StaticVector<FloatType, dimension> origin, StaticVector<FloatType, dimension> direction) {
   this->origin = origin;
   this->direction = direction;
}

inline Ray3f transformRay(const Ray3f ray, const Matrix4f &invTransform) {
   Ray3f result;
   result.origin = transformVector3f(invTransform, ray.origin);
   result.direction = transformVector3f(invTransform, ray.origin+ray.direction) - result.origin;
   return result;
}

inline void intersectXPlane(const Ray3f &ray, const float32 xPlane, float32 &rayParam, float32 &yIntersect, float32 &zIntersect) {
   rayParam = (xPlane - ray.origin[0]) / ray.direction[0];
   yIntersect = ray.direction[1]*rayParam + ray.origin[1];
   zIntersect = ray.direction[2]*rayParam + ray.origin[2];
}

inline void intersectYPlane(const Ray3f &ray, const float32 yPlane, float32 &rayParam, float32 &xIntersect, float32 &zIntersect) {
   rayParam = (yPlane - ray.origin[1]) / ray.direction[1];
   xIntersect = ray.direction[0]*rayParam + ray.origin[0];
   zIntersect = ray.direction[2]*rayParam + ray.origin[2];
}

inline void intersectZPlane(const Ray3f &ray, const float32 zPlane, float32 &rayParam, float32 &xIntersect, float32 &yIntersect) {
   rayParam = (zPlane - ray.origin[2]) / ray.direction[2];
   xIntersect = ray.direction[0]*rayParam + ray.origin[0];
   yIntersect = ray.direction[1]*rayParam + ray.origin[1];
}

inline bool intersectBoundingSphere3f(const Sphere3f &sphere, const Ray3f &ray) {
   float32 dc0 = ray.origin[0] - sphere.center[0];
   float32 dc1 = ray.origin[1] - sphere.center[1];
   float32 dc2 = ray.origin[2] - sphere.center[2];

   float32 cp0 = ray.direction[1]*dc2 - ray.direction[2]*dc1;
   float32 cp1 = ray.direction[2]*dc0 - ray.direction[0]*dc2;
   float32 cp2 = ray.direction[0]*dc1 - ray.direction[1]*dc0;

   return cp0*cp0 + cp1*cp1 + cp2*cp2 <= sphere.radius*sphere.radius;

//   Vector3f d = ray.direction.crossProduct(ray.origin - sphere.center);
//   return d*d <= sphere.radius*sphere.radius;
}


/// assumes normalized "normal"
inline Ray3f mirrorRay(const Ray3f &ray, const Vector3f &point, const Vector3f &normal) {
   Ray3f mirrored;
   mirrored.origin = point;
   mirrored.direction = ray.direction - normal*((normal*ray.direction)*2.0f);
   return mirrored;
}

/// assumes normalized "normal", normal * v > 0
inline Vector3f mirrorVector(const Vector3f& v, const Vector3f &normal) {
   return normal*((normal*v)*2.0f) - v;
}


#endif


 