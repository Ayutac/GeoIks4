//---------------------------------------------------------------------------
#include "precompiledHeaders.h"
//---------------------------------------------------------------------------
#include "MathTools.h"
#include "MathTools.hpp"
//---------------------------------------------------------------------------
#include "LinearAlgebra.h"
#include "LinearAlgebra.hpp"
#include "PAssert.h"
#include "Random.h"
#include "BoundingBox.hpp"
#include <float.h>
//---------------------------------------------------------------------------

const float32 EPSILON = 1E-8f;

/**
 * Get distance between a point and a section
 */
float32 getDistancePointSection( const Vector3f& p, const Vector3f start, const Vector3f end )
{
	if ( ( end - start ) * ( p - start ) < 0 ){
		return norm( start - p );
	} else if ( ( start - end ) * ( p - end ) < 0 ){
		return norm( end - p );
	} else {
		return norm( ( p - start ).crossProduct( normalize( end - start ) ) );
	}
}



/// Return the world coordinates defined by v
/// alpha is in the x-y-plane from [0;PI];
/// beta is in the x-z-plane from [0;2*PI]
void getAbsoluteAngle( Vector3f v, float& alpha, float& beta )
{
	float PI = 3.14159265f;
	float PI_2 = 1.570796325f;
	float eps = 1e-6f;

	v = normalize( v );

	// Compute alpha (side-view angle between 0 and PI)
	if ( v[1] >= 0 )
		alpha = PI_2 - acos( sqrt( sqr( v[0] ) + sqr( v[2] ) ) );
	else 
		alpha = PI_2 + acos( sqrt( sqr( v[0] ) + sqr( v[2] ) ) );
	

	// Compute beta (top-view-angle between 0 and 2*PI)
	if ( v[0] >= 0 && v[2] >= 0 )
		beta = ( fabs( v[0] ) > eps )? PI_2 - atan( v[2]/v[0] ) : 0;
	else if ( v[0] >= 0 && v[2] < 0 )
		beta = ( fabs( v[0] ) > eps )? PI_2 + atan( -v[2]/v[0] ) : PI;
	else if ( v[0] < 0 && v[2] < 0 )
		beta = ( fabs( v[0] ) > eps )? PI * 1.5f - atan( v[2]/v[0] ) : PI;
	else 
		beta = ( fabs( v[0] ) > eps )? PI * 1.5f + atan( -v[2]/v[0] ) : 0;
}

bool linsolve2f(const Matrix2f &m, const Vector2f &v, Vector2f &solution) {
   float32 det = m[1][1]*m[0][0]-m[0][1]*m[1][0];
   if (fabs(det) < EPSILON) return false;
   float32 invDet = 1.0f/det;
   solution[0] = (m[1][1]*v[0]-m[0][1]*v[1]) * invDet;
   solution[1] = (m[0][0]*v[1]-m[1][0]*v[0]) * invDet;
   return true;
}

bool linsolve3f(const Matrix3f &m, const Vector3f &v, Vector3f &solution) {
   float32 det = m[2][2]*m[0][0]*m[1][1] + m[0][2]*m[2][1]*m[1][0] - m[0][2]*m[2][0]*m[1][1]
                -m[0][0]*m[1][2]*m[2][1] + m[0][1]*m[1][2]*m[2][0] - m[2][2]*m[0][1]*m[1][0];

   if (fabs(det) < EPSILON) return false;

   solution[0] = (-m[0][2]*v[2]*m[1][1] + m[2][2]*v[0]*m[1][1] - m[2][2]*m[0][1]*v[1]
                  -v[0]*m[1][2]*m[2][1] + m[0][1]*m[1][2]*v[2] + m[0][2]*m[2][1]*v[1]) / det;
   solution[1] =-( m[2][2]*m[1][0]*v[0] - m[2][2]*v[1]*m[0][0] + m[1][2]*v[2]*m[0][0]
                  -m[1][2]*m[2][0]*v[0] - m[1][0]*m[0][2]*v[2] + v[1]*m[0][2]*m[2][0]) / det;
   solution[2] = (-m[2][0]*v[0]*m[1][1] + m[2][0]*m[0][1]*v[1] + m[2][1]*m[1][0]*v[0]
                  -m[2][1]*v[1]*m[0][0] + v[2]*m[0][0]*m[1][1] - v[2]*m[0][1]*m[1][0]) / det;
   return true;
}


float32 approxErrFunc(float32 arg) {
   // table:
   // for i=0 to 42 do
   //    v[i] := erf(i/10)   (Maple)
   static const float32 table[43] = {0.0f,
      0.11246292f, 0.22270259f, 0.32862676f, 0.42839236f, 0.52049988f, 0.60385609f, 0.67780119f,
      0.74210096f, 0.79690821f, 0.84270079f, 0.88020507f, 0.91031398f, 0.93400794f, 0.95228512f,
      0.96610515f, 0.97634838f, 0.98379046f, 0.98909050f, 0.99279043f, 0.99532227f, 0.99702053f,
      0.99813715f, 0.99885682f, 0.99931149f, 0.99959305f, 0.99976397f, 0.99986567f, 0.99992499f,
      0.99995890f, 0.99997791f, 0.99998835f, 0.99999397f, 0.99999694f, 0.99999848f, 0.99999926f,
      0.99999964f, 0.99999983f, 0.99999992f, 0.99999997f, 0.99999998f, 0.99999999f, 1.0000000f};

   float32 value;
   float32 index = fabs(arg*10.0f);
   if (ceil(index)>42) {
      value = 1.0f;
   } else {
      card32 lower = (card32)floor(index);
      card32 upper = (card32)ceil(index);
      float32 weight = fmod(index,1.0f);
      value = table[lower]*(1.0f-weight) + table[upper]*weight;
   }

   if (arg < 0) {
      return -value;
   } else {
      return value;
   }
}

float32 approxInvErrFunc(float32 arg) {
   // table:
   // for i=0 to 42 do
   //    v[i] := erf(i/10)   (Maple)
   static const float32 table[43] = {0.0f,
      0.11246292f, 0.22270259f, 0.32862676f, 0.42839236f, 0.52049988f, 0.60385609f, 0.67780119f,
      0.74210096f, 0.79690821f, 0.84270079f, 0.88020507f, 0.91031398f, 0.93400794f, 0.95228512f,
      0.96610515f, 0.97634838f, 0.98379046f, 0.98909050f, 0.99279043f, 0.99532227f, 0.99702053f,
      0.99813715f, 0.99885682f, 0.99931149f, 0.99959305f, 0.99976397f, 0.99986567f, 0.99992499f,
      0.99995890f, 0.99997791f, 0.99998835f, 0.99999397f, 0.99999694f, 0.99999848f, 0.99999926f,
      0.99999964f, 0.99999983f, 0.99999992f, 0.99999997f, 0.99999998f, 0.99999999f, 1.0000000f};

   float32 absArg = fabs(arg);
   if (absArg > 1.0f) throw PException("approxInvErrFunc - arg out of range");
   card32 upper = arrayBisectionSearch(table, 0, 42, absArg);
   if (upper == 0) return 0.0f;
   card32 lower = upper-1;
   float32 lowerVal = lower/10.0f;
   float32 upperVal = upper/10.0f;
   float32 upperElem = table[upper];
   float32 lowerElem = table[lower];
   float32 value;
   if (upperElem == lowerElem) {
      value = 4.2f;
   } else {
      value = lowerVal + (absArg-lowerElem)*(upperVal-lowerVal)/(upperElem-lowerElem);
   }
   if (arg < 0) {
      return -value;
   } else {
      return value;
   }
}

float32 gaussianRandom() {
   return approxInvErrFunc(2.0f*rnd01()-1.0f);
}

float32 calcMinProjection(const Vector3f &center, const Vector3f &prjVect, const BoundingBox3f &bb) {
   Vector3f d = bb.lowerCorner - center;
   float32 minPrj = prjVect * d;
   Vector3f b = makeVector3f(bb.lowerCorner[0], bb.lowerCorner[1], bb.upperCorner[2]);
   d = b - center;
   float32 test = prjVect * d;
   if (test < minPrj) minPrj = test;
   b = makeVector3f(bb.lowerCorner[0], bb.upperCorner[1], bb.lowerCorner[2]);
   d = b - center;
   test = prjVect * d;
   if (test < minPrj) minPrj = test;
   b = makeVector3f(bb.lowerCorner[0], bb.upperCorner[1], bb.upperCorner[2]);
   d = b - center;
   test = prjVect * d;
   if (test < minPrj) minPrj = test;
   b = makeVector3f(bb.upperCorner[0], bb.lowerCorner[1], bb.lowerCorner[2]);
   d = b - center;
   test = prjVect * d;
   if (test < minPrj) minPrj = test;
   b = makeVector3f(bb.upperCorner[0], bb.lowerCorner[1], bb.upperCorner[2]);
   d = b - center;
   test = prjVect * d;
   if (test < minPrj) minPrj = test;
   b = makeVector3f(bb.upperCorner[0], bb.upperCorner[1], bb.lowerCorner[2]);
   d = b - center;
   test = prjVect * d;
   if (test < minPrj) minPrj = test;
   b = makeVector3f(bb.upperCorner[0], bb.upperCorner[1], bb.upperCorner[2]);
   d = b - center;
   test = prjVect * d;
   if (test < minPrj) minPrj = test;
   return minPrj;
}           

void make16BitNormal(const Vector3f normal, int16 &normalX, int16 &normalY, int16 &normalZ) {
   Vector3f qnormal = normalize(normal);
   qnormal *= 32767;
   if (qnormal[0] < -32767) qnormal[0] = -32767; if (qnormal[0] > 32767) qnormal[0] = 32767;
   if (qnormal[1] < -32767) qnormal[1] = -32767; if (qnormal[1] > 32767) qnormal[1] = 32767;
   if (qnormal[2] < -32767) qnormal[2] = -32767; if (qnormal[2] > 32767) qnormal[2] = 32767;

   normalX = (int16)(qnormal[0]);
   normalY = (int16)(qnormal[1]);
   normalZ = (int16)(qnormal[2]);
}

Vector3f orthogonalize(const Vector3f &v, const Vector3f &nonOrtho) {
   float32 h = v*nonOrtho;
   return nonOrtho - v*h;
}


Vector3f makeOrthonormalVector(const Vector3f &v) {
   Vector3f candidates[3];
   candidates[0] = XAXIS_VECTOR3F;
   candidates[1] = YAXIS_VECTOR3F;
   candidates[2] = ZAXIS_VECTOR3F;

   float32 min = FLT_MAX;
   card32 cIndex = 0;
   for (card32 i=0; i<3; i++) {
      float32 p = fabs(v*candidates[i]);
      if (p<min) {
         min = p;
         cIndex = i;
      }
   }
   Vector3f result = orthogonalize(v, candidates[cIndex]);
   return normalize(result);
}

Vector2f projectToN1Plane(const Vector3f &x, const Vector3f &n, const Vector3f &u, const Vector3f &v) {
   float32 np = x*n;
   float32 up = x*u;
   float32 vp = x*v;
   if (np != 0) {
      float32 npInv = 1.0f/np;
      up *= npInv;
      vp *= npInv;
      return makeVector2f(up, vp);
   } else {
      return makeVector2f(FLT_MAX, FLT_MAX);
   }
}

void eigenValuesSym2f(const float32 a, const float32 b, const float32 c,
                      float32 &lambda1, float32 &lambda2) {
   float32 det = c*c-2*a*c+a*a+4*b*b;
   float32 sqrtDet = sqrt(fabs(det));
   lambda1 = 0.5f*(c+a+sqrtDet);
   lambda2 = 0.5f*(c+a-sqrtDet);
}

void eigenspace2f(const float32 a, const float32 b, const float32 c,
                  float32 &lambda1, Vector2f &v1, float32 &lambda2, Vector2f &v2,
                  const float32 epsilon, bool normalize)
{
   float32 det = c*c-2*a*c+a*a+4*b*b;
   float32 sqrtDet = sqrt(fabs(det));
   //pAssert(det >= 0);
   if (fabs(b) > epsilon) {
      lambda1 = 0.5f*(c+a+sqrtDet);
      v1 = makeVector2f(-0.5f*(c-a-sqrtDet)/b, 1.0f);
      lambda2 = 0.5f*(c+a-sqrtDet);
      v2 = makeVector2f(-0.5f*(c-a+sqrtDet)/b, 1.0f);
      if (normalize) {
         v1 = ::normalize(v1);
         v2 = ::normalize(v2);
      }
   } else {
      lambda1 = a;
      lambda2 = c;
      v1 = makeVector2f(1,0);
      v2 = makeVector2f(0,1);
   }
}
void eigenspaceMatQuad2f(const Matrix2f &fpm, float32 &lambda1, float32 &lambda2, Matrix2f &eSpace,
                         bool sqrtEigenvalues, const float32 epsilon, bool normalize)
{
   float32 a = fpm[0][0]*fpm[0][0]+fpm[0][1]*fpm[0][1];
   float32 b = fpm[0][0]*fpm[1][0]+fpm[0][1]*fpm[1][1];
   float32 c = fpm[1][0]*fpm[1][0]+fpm[1][1]*fpm[1][1];

   eigenspace2f(a, b, c, lambda1, eSpace[0], lambda2, eSpace[1], epsilon, normalize);
   if (sqrtEigenvalues) {
      lambda1 = sqrt(fabs(lambda1));
      lambda2 = sqrt(fabs(lambda2));
   }
}

Matrix2f quadEigenSpaceRecompose(const Matrix2f &eigenspace, const float32 lambda1, const float32 lambda2) {
   float32 ra = eigenspace[0][0]*eigenspace[0][0]*lambda1+eigenspace[0][1]*eigenspace[0][1]*lambda2;
   float32 rb = eigenspace[0][0]*lambda1*eigenspace[1][0]+eigenspace[0][1]*lambda2*eigenspace[1][1];
   float32 rc = eigenspace[1][0]*eigenspace[1][0]*lambda1+eigenspace[1][1]*eigenspace[1][1]*lambda2;

   return makeMatrix2f(ra, rb, rb, rc);
}

void mainAxis2f(Vector2f &v0, Vector2f &v1) {
   float32 lambda1 = 0;
   float32 lambda2 = 0;
   Matrix2f eigenspace;

   float32 a = v0[0]*v0[0]+v0[1]*v0[1];
   float32 b = v0[0]*v1[0]+v0[1]*v1[1];
   float32 c = v1[0]*v1[0]+v1[1]*v1[1];

   eigenspace2f(a, b, c, lambda1, eigenspace[0], lambda2, eigenspace[1], 0.001f);
   if (lambda2 > 0) {
      lambda1 = sqrt(fabs(lambda1));
      lambda2 = sqrt(fabs(lambda2));

      float32 ra = eigenspace[0][0]*eigenspace[0][0]*lambda1+eigenspace[0][1]*eigenspace[0][1]*lambda2;
      float32 rb = eigenspace[0][0]*lambda1*eigenspace[1][0]+eigenspace[0][1]*lambda2*eigenspace[1][1];
      float32 rc = eigenspace[1][0]*eigenspace[1][0]*lambda1+eigenspace[1][1]*eigenspace[1][1]*lambda2;

      v0[0] = ra; v1[0] = rb;
      v0[1] = rb; v1[1] = rc;
   }
}


Vector3f projectVectByNormalAndDirection(const Vector3f v, const Vector3f n, const Vector3f d,
                                         const float32 minPrjDenominator)
{
   float32 divBy = n*d;
   if (fabs(divBy) < minPrjDenominator) {
      if (divBy >= 0) {
         divBy = minPrjDenominator;
      } else {
         divBy = -minPrjDenominator;
      }
   }
   return v - d*((n*v)/divBy);
}

Matrix3f calcTangentSystem( const Vector3f &normal )
{
	Vector3f n = normal;
	float l = norm(n);
	if( l < 1e-10f )
		throw PException("calcTangentSystem() - vector with norm 0 given!");
	n /= l;
	Vector3f u,v;

	if( fabs(n[0]) > 0.5f )
		u = n.crossProduct( makeVector3f(0,1,0));
	else
		u = n.crossProduct( makeVector3f(1,0,0));
	v = u.crossProduct( n );
	u = v.crossProduct( n );
	u.normalize();
	v.normalize();
	return makeMatrix3f( u[0], v[0], n[0], 
								u[1], v[1], n[1],
								u[2], v[2], n[2] );
}


int32 getExponentOfTwo(const float32 v) {
   const void* vptr = &v;
   card32 vAsCard32 = *((card32*)vptr);
   return ((int32)((vAsCard32 & 0x7F800000) >> 23)) - 127;
}

int32 getExponentOfTwo(const float64 v) {
   return ((int32)(((*(((card32*)&v)+1)) & 0x7FF00000) >> 20)) - 1023;
}

#define ITMAX 100
#define CGOLD 0.3819660
#define ZEPS 1.0e-10
/*Here ITMAX is the maximum allowed number of iterations; CGOLD is the golden ratio; ZEPS is
a small number that protects against trying to achieve fractional accuracy for a minimum that
happens to be exactly zero.*/
#define SHFT(a,b,c,d) (a)=(b);(b)=(c);(c)=(d);
#define SIGN(a,b) ((b) >= 0.0 ? fabs(a) : -fabs(a))
#define nrerror(a) throw "Error in brent function!\n";
