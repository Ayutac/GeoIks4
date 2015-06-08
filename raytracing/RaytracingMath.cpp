//---------------------------------------------------------------------------
#include "precompiledHeaders.h"
//---------------------------------------------------------------------------
#include "RaytracingMath.h"
#include "RaytracingMath.hpp"
//---------------------------------------------------------------------------
#include "LinearAlgebra.hpp"
#include "BoundingBox.hpp"
#include "MathTools.h"
#include "MathTools.hpp"
#include "PAssert.h"
//---------------------------------------------------------------------------


/** assumed to be zero in numerical calculations */
static const float32 EPSILON = 0.000001f;

struct Quad3f {
   Vector3f ul, ll, lr;
   Vector3f normal;
   Quad3f() {}
   Quad3f(Vector3f ul, Vector3f ll, Vector3f lr) {
      this->ul = ul; this->ll = ll; this->lr = lr;
      normal = (lr-ll).crossProduct(ul-ll);
   }
};

bool intersectBoundingBox3f(const Ray3f &ray, const BoundingBox3f &bb, float32 &firstHitParam, float32 &secondHitParam) {
   bool exitFound = false;
   float32 hitParam;

   if (ray.direction[0] != 0) {
      float32 yIntersect;
      float32 zIntersect;
      if (ray.direction[0] > 0) {
         intersectXPlane(ray, bb.upperCorner[0], hitParam, yIntersect, zIntersect);
      } else {
         intersectXPlane(ray, bb.lowerCorner[0], hitParam, yIntersect, zIntersect);
      }
      if (yIntersect >= bb.lowerCorner[1] && yIntersect <= bb.upperCorner[1]
       && zIntersect >= bb.lowerCorner[2] && zIntersect <= bb.upperCorner[2]) {
         secondHitParam = hitParam;
         exitFound = true;
      }
   }

   if (!exitFound) {
      float32 xIntersect;
      float32 zIntersect;
      if (ray.direction[1] != 0) {
         if (ray.direction[1] > 0) {
            intersectYPlane(ray, bb.upperCorner[1], hitParam, xIntersect, zIntersect);
         } else {
            intersectYPlane(ray, bb.lowerCorner[1], hitParam, xIntersect, zIntersect);
         }
         if (xIntersect >= bb.lowerCorner[0] && xIntersect <= bb.upperCorner[0]
          && zIntersect >= bb.lowerCorner[2] && zIntersect <= bb.upperCorner[2]) {
            secondHitParam = hitParam;
            exitFound = true;
         }
      }
   }

   if (!exitFound) {
      float32 xIntersect;
      float32 yIntersect;
      if (ray.direction[2] != 0) {
         if (ray.direction[2] > 0) {
            intersectZPlane(ray, bb.upperCorner[2], hitParam, xIntersect, yIntersect);
         } else {
            intersectZPlane(ray, bb.lowerCorner[2], hitParam, xIntersect, yIntersect);
         }
         if (xIntersect >= bb.lowerCorner[0] && xIntersect <= bb.upperCorner[0]
          && yIntersect >= bb.lowerCorner[1] && yIntersect <= bb.upperCorner[1]) {
            secondHitParam = hitParam;
            exitFound = true;
         }
      }
   }

   if (!exitFound) {
      return false;
   }

   bool entryFound = false;

   if (ray.direction[0] != 0) {
      float32 yIntersect;
      float32 zIntersect;
      if (ray.direction[0] > 0) {
         intersectXPlane(ray, bb.lowerCorner[0], hitParam, yIntersect, zIntersect);
      } else {
         intersectXPlane(ray, bb.upperCorner[0], hitParam, yIntersect, zIntersect);
      }
      if (yIntersect >= bb.lowerCorner[1] && yIntersect <= bb.upperCorner[1]
       && zIntersect >= bb.lowerCorner[2] && zIntersect <= bb.upperCorner[2]) {
         firstHitParam = hitParam;
         entryFound = true;
      }
   }

   if (!entryFound) {
      float32 xIntersect;
      float32 zIntersect;
      if (ray.direction[1] != 0) {
         if (ray.direction[1] > 0) {
            intersectYPlane(ray, bb.lowerCorner[1], hitParam, xIntersect, zIntersect);
         } else {
            intersectYPlane(ray, bb.upperCorner[1], hitParam, xIntersect, zIntersect);
         }
         if (xIntersect >= bb.lowerCorner[0] && xIntersect <= bb.upperCorner[0]
          && zIntersect >= bb.lowerCorner[2] && zIntersect <= bb.upperCorner[2]) {
            firstHitParam = hitParam;
            entryFound = true;
         }
      }
   }

   if (!entryFound) {
      float32 xIntersect;
      float32 yIntersect;
      if (ray.direction[2] != 0) {
         if (ray.direction[2] > 0) {
            intersectZPlane(ray, bb.lowerCorner[2], hitParam, xIntersect, yIntersect);
         } else {
            intersectZPlane(ray, bb.upperCorner[2], hitParam, xIntersect, yIntersect);
         }
         if (xIntersect >= bb.lowerCorner[0] && xIntersect <= bb.upperCorner[0]
          && yIntersect >= bb.lowerCorner[1] && yIntersect <= bb.upperCorner[1]) {
            firstHitParam = hitParam;
            entryFound = true;
         }
      }
   }

   if (!entryFound) {
      firstHitParam = 0.0f;
   }

   return true;
}

bool intersectLineBoundingBox3f(const Ray3f &line, const BoundingBox3f &bb) {
   float32 hitParam;
   if (line.direction[0] != 0) {
      float32 yIntersect;
      float32 zIntersect;
      if (line.direction[0] > 0) {
         intersectXPlane(line, bb.lowerCorner[0], hitParam, yIntersect, zIntersect);
      } else {
         intersectXPlane(line, bb.upperCorner[0], hitParam, yIntersect, zIntersect);
      }
      if (yIntersect >= bb.lowerCorner[1] && yIntersect <= bb.upperCorner[1]
       && zIntersect >= bb.lowerCorner[2] && zIntersect <= bb.upperCorner[2]) {
         return true;
      }
   }

   if (line.direction[1] != 0) {
      float32 xIntersect;
      float32 zIntersect;
         if (line.direction[1] > 0) {
            intersectYPlane(line, bb.lowerCorner[1], hitParam, xIntersect, zIntersect);
         } else {
            intersectYPlane(line, bb.upperCorner[1], hitParam, xIntersect, zIntersect);
         }
      if (xIntersect >= bb.lowerCorner[0] && xIntersect <= bb.upperCorner[0]
       && zIntersect >= bb.lowerCorner[2] && zIntersect <= bb.upperCorner[2]) {
         return true;
      }
   }

   if (line.direction[2] != 0) {
      float32 xIntersect;
      float32 yIntersect;
         if (line.direction[2] > 0) {
            intersectZPlane(line, bb.lowerCorner[2], hitParam, xIntersect, yIntersect);
         } else {
            intersectZPlane(line, bb.upperCorner[2], hitParam, xIntersect, yIntersect);
         }
      if (xIntersect >= bb.lowerCorner[0] && xIntersect <= bb.upperCorner[0]
       && yIntersect >= bb.lowerCorner[1] && yIntersect <= bb.upperCorner[1]) {
         return true;
      }
   }

   return false;
}

bool intersectTriangle3f(const Ray3f &ray, const Vector3f &p1, const Vector3f &p2, const Vector3f &p3, float32 &hitParam) {
   Vector3f s1 = p2 - p1;
   Vector3f s2 = p3 - p1;
   Vector3f rhs = ray.origin - p1;

   Matrix3f m = makeMatrix3f(s1[0], s2[0], -ray.direction[0],
                             s1[1], s2[1], -ray.direction[1],
                             s1[2], s2[2], -ray.direction[2]).transpose();
   Vector3f solution;
   if (linsolve3f(m, rhs, solution)) {
      if (solution[0] >=0 && solution[1] >=0 && solution[0] + solution[1] <=1) {
         hitParam = solution[2];
         return true;
      } else {
         return false;
      }
   } else {
      return false;
   }
}

bool intersectTriangle3f(const Ray3f &ray, const Vector3f &p1, const Vector3f &p2, const Vector3f &p3,
                                       float32 &hitParam, float32 &firstTriangleParam, float32 &secondTriangleParam) {
   Vector3f s1 = p2 - p1;
   Vector3f s2 = p3 - p1;
   Vector3f rhs = ray.origin - p1;

   Matrix3f m = makeMatrix3f(s1[0], s2[0], -ray.direction[0],
                             s1[1], s2[1], -ray.direction[1],
                             s1[2], s2[2], -ray.direction[2]).transpose();
   Vector3f solution;
   if (linsolve3f(m, rhs, solution)) {
      if (solution[0] >=0 && solution[1] >=0 && solution[0] + solution[1] <=1) {
         hitParam = solution[2];
         firstTriangleParam = solution[0];
         secondTriangleParam = solution[1];
         return true;
      } else {
         return false;
      }
   } else {
      return false;
   }
}

bool intersectParallelogram3f(const Ray3f &ray, const Vector3f &p1, const Vector3f &p2, const Vector3f &p3, float32 &hitParam) {
   Vector3f s1 = p2 - p1;
   Vector3f s2 = p3 - p1;
   Vector3f rhs = ray.origin - p1;
   Matrix3f m = makeMatrix3f(s1[0], s2[0], -ray.direction[0],
                             s1[1], s2[1], -ray.direction[1],
                             s1[2], s2[2], -ray.direction[2]).transpose();
   Vector3f solution;
   if (linsolve3f(m, rhs, solution)) {
      if (solution[0] >=0 && solution[0] <=1 && solution[1] >=0 && solution[1] <=1 ) {
         hitParam = solution[2];
         return true;
      } else {
         return false;
      }
   } else {
      return false;
   }
}

//static inline void returnOriginal(const Vector3f &t1, const Vector3f &t2, const Vector3f &t3,
//                                  Vector3f &s1, Vector3f &s2, Vector3f &s3)
//{
//   s1 = t1; s2 = t2; s3 = t3;
//}


float32 minDistQuadRayToPoint(const Ray3f &ray, const Vector3f &point)
{
   Vector3f cp = ray.direction.crossProduct(point - ray.origin);
   return cp*cp;
}


bool intersectPlaneRay(const Ray3f &ray, const Vector3f &planeNormal, const Vector3f &planeOrigin,
                       float32 &rayParam)
{
   float32 divBy = ray.direction * planeNormal;
   if (fabs(divBy) <= EPSILON) return false;
   rayParam = (planeNormal * (planeOrigin - ray.origin)) / divBy;
   return true;
}

bool calcDistToLine(const Ray3f &lineSegmentRay, const Ray3f &ray,
                    float32 &distQuad, float32 &rayParam, float32 &lineSegParam)
{
   Vector3f distNormal = lineSegmentRay.direction.crossProduct(ray.direction);
   if (distNormal*distNormal < EPSILON * EPSILON) return false;
   Vector3f planeRayNormal = ray.direction.crossProduct(distNormal);
   bool iFound = intersectPlaneRay(lineSegmentRay, planeRayNormal, ray.origin, lineSegParam);
   if (!iFound || lineSegParam < 0 || lineSegParam > 1) {
      return false;
   }
   Vector3f planeLineSegNormal = lineSegmentRay.direction.crossProduct(distNormal);
   iFound = intersectPlaneRay(ray, planeLineSegNormal, lineSegmentRay.origin, rayParam);
   if (iFound) {
      Vector3f dist =   lineSegmentRay.origin + lineSegmentRay.direction * lineSegParam
                      -           (ray.origin +            ray.direction * rayParam);
      distQuad = dist*dist;
      return true;
   }
   return false;
}

Vector3f refractNormal(const Vector3f &incomming, const Vector3f &surfaceNormal,
                       const float32 inOutDensityQuotient) {
   float32 nMultI = surfaceNormal*incomming;
   float32 det = 1-sqr(inOutDensityQuotient)*(1-sqr(nMultI));
   if (det < 0.0f) {
      return mirrorVector(incomming, surfaceNormal);
   } else {
      return surfaceNormal*(-inOutDensityQuotient*(nMultI) - sqrt(det)) + incomming*inOutDensityQuotient;
   }
}

float32 intersectPlane3f(const Vector3f &p1, const Vector3f &p2, const Vector3f &p3, const Ray3f &ray) {
   Vector3f n = (p2-p1).crossProduct(p3-p1);
   return (n*(p1 - ray.origin)) / (n*ray.direction);
}

void minLineDistRayParam3f(const Vector3f &rayOrigin, const Vector3f &rayDirection,
                           const Vector3f &lineOrigin, const Vector3f &lineDirection,
                           float32 &rayParam, float32 &lineParam)
{
   Vector3f distNormal = rayDirection.crossProduct(lineDirection);
   Vector3f lineNormalPlaneNormal = distNormal.crossProduct(lineDirection);
   float32 divBy = lineNormalPlaneNormal*rayDirection;
   if (fabs(divBy) <= 1E-7) {
      rayParam = rayDirection*(lineOrigin-rayOrigin);
   } else {
      rayParam = (lineNormalPlaneNormal*(lineOrigin - rayOrigin)) / divBy;
   }

   Vector3f rayNormalPlaneNormal = distNormal.crossProduct(rayDirection);
   divBy = rayNormalPlaneNormal*lineDirection;
   if (fabs(divBy) <= 1E-7) {
      lineParam = 0;
   } else {
      lineParam = (rayNormalPlaneNormal*(rayOrigin - lineOrigin)) / divBy;
   }
}






