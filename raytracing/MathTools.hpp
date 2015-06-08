//---------------------------------------------------------------------------
#ifndef MathToolsInlineH
#define MathToolsInlineH
//---------------------------------------------------------------------------
#include "MathTools.h"
//---------------------------------------------------------------------------
#include "GeoXOutput.h"
#include "LinearAlgebra.hpp"
#include "BoundingBox.hpp"
//---------------------------------------------------------------------------



template<class T>
inline T sqr(const T t)
{
   return t*t;
};

template <class T>
T blend( T oldVal, T newVal, float32 factor )
{
   // Check input
   if ( factor <= 0.0f ) return newVal;
   if ( factor >= 1.0f ) return oldVal;
   // Modulate old with newVal
   return newVal * ( 1.0 - factor ) + oldVal * factor;
}

template<class FloatType>
FloatType determinant(const StaticMatrix<FloatType, 3, 3> &m) {
   return  m[0][0]*m[1][1]*m[2][2] - m[0][0]*m[1][2]*m[2][1]
         - m[1][0]*m[0][1]*m[2][2] + m[1][0]*m[0][2]*m[2][1]
         + m[2][0]*m[0][1]*m[1][2] - m[2][0]*m[0][2]*m[1][1];
}

template<class FloatType>
FloatType determinant(const StaticVector<FloatType, 3> &v0,
                      const StaticVector<FloatType, 3> &v1,
                      const StaticVector<FloatType, 3> &v2) {
   return  v0[0]*v1[1]*v2[2] - v0[0]*v1[2]*v2[1]
         - v1[0]*v0[1]*v2[2] + v1[0]*v0[2]*v2[1]
         + v2[0]*v0[1]*v1[2] - v2[0]*v0[2]*v1[1];
}

template<class FloatType>
FloatType determinant(const StaticMatrix<FloatType, 2, 2> &m) {
   return  m[0][0]*m[1][1]-m[0][1]*m[1][0];
}

template<class FloatType>
FloatType determinant(const StaticVector<FloatType, 2> &v0,
                      const StaticVector<FloatType, 2> &v1) {
   return  v0[0]*v1[1]-v0[1]*v1[0];
}

inline Vector3f unpack16BitNormal(const int16 normalX, const int16 normalY, const int16 normalZ) {
   const float32 scale = 1.0f / 32767.0f;
   return makeVector3f((float32)normalX*scale, (float32)normalY*scale,  (float32)normalZ*scale);
}

template <class FloatType, unsigned dim>
void limitLengthRatioToSmaller(StaticVector<FloatType, dim> &vect1, StaticVector<FloatType, dim> &vect2,
                               const FloatType maxFact) {
   FloatType l1 = norm(vect1);
   FloatType l2 = norm(vect2);
   if (l1 > l2) {
      if (l1 > maxFact*l2) {
         vect1 *= (l2/l1)*maxFact;
      }
   } else if (l2 > l1) {
      if (l2 > maxFact*l1) {
         vect2 *= (l1/l2)*maxFact;
      }
   }
}

template <class FloatType>
void limitRatioToSmaller(FloatType &l1, FloatType &l2, const FloatType maxFact) {
   if (l1 > l2) {
      if (l1 > maxFact*l2) {
         l1 *= (l2/l1)*maxFact;
      }
   } else if (l2 > l1) {
      if (l2 > maxFact*l1) {
         l2 *= (l1/l2)*maxFact;
      }
   }
}

template <class FloatType>
void limitRatioToLarger(FloatType &l1, FloatType &l2, const FloatType maxFact) {
   if (l1 > l2) {
      if (l1 > maxFact*l2) {
         l2 = l1/maxFact;
      }
   } else if (l2 > l1) {
      if (l2 > maxFact*l1) {
         l1 = l2/maxFact;
      }
   }
}

inline float hermiteInterpolation(float32 left, float32 leftDiv, float32 right, float32 rightDiv, float32 t) {
   static const Matrix4f HERM_MATR = makeMatrix4f( 1, 0, 0, 0,
                                                  0, 1, 0, 0,
                                                 -3,-2, 3,-1,
                                                  2, 1,-2, 1 );
   Vector4f coeff = HERM_MATR*makeVector4f(left, leftDiv, right, rightDiv);
   return coeff[0] + t*(coeff[1] + t*(coeff[2] + t*coeff[3]));
}


inline int32 simpleLog2(card32 i) {
   int32 result = -1;
   while (i) {
      i >>= 1;
      result++;
   }
   return result;
}

template <typename FloatType>
bool linsolve2(
   const StaticMatrix<FloatType, 2, 2> &m,
   const StaticVector<FloatType, 2> &v,
   StaticVector<FloatType, 2> &solution,
   FloatType EPSILON)
{
   FloatType det = m[1][1]*m[0][0]-m[0][1]*m[1][0];
   if (fabs(det) < EPSILON) return false;
   FloatType invDet = 1.0f/det;
   solution[0] = (m[1][1]*v[0]-m[0][1]*v[1]) * invDet;
   solution[1] = (m[0][0]*v[1]-m[1][0]*v[0]) * invDet;
   return true;
}




#endif
 