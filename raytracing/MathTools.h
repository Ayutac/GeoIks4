//---------------------------------------------------------------------------
#ifndef MathToolsH
#define MathToolsH
//---------------------------------------------------------------------------
#include "PTypes.h"
#include "LinearAlgebra.h"
#include "BoundingBox.h"
#include <vector>
//---------------------------------------------------------------------------


// visual studio compatibility
#ifndef M_PI
   //const double M_PI = 3.1415926535897932384626433832795
   #define M_PI 3.14159265358979323846
#endif


// ---- general math


template<class T>
inline T sqr(const T t);

inline float32 power( const float32 b, const card32 e ){
	if ( e == 0 ) return 1;
	else return b * power( b, e-1 );
}

template<class T>
inline T clamp( const T& t, const T& minValue, const T& maxValue )
{
	return max( minValue, min( maxValue, t) );
}

inline card32 log_2( const card32 n ){
	if ( n == 1 ) return 0;
	else return 1 + log_2( n / 2 );
}

// Simple blending function
template <class T>
T blend( T oldVal, T newVal, float32 factor );

inline float hermiteInterpolation(float32 left, float32 leftDiv, float32 right, float32 rightDiv, float32 t);

inline int32 simpleLog2(card32 i);

inline int32 ilog2(card32 i) { return simpleLog2(i); }

// ---- statistics / random numbers


// calculate approximate errfunc [2/sqrt(Pi) * int(exp(-t^2), t=0..x)]
// based on a 65 entry table for arg =-3.2 .. 3.2 and linear interpolation
extern float32 approxErrFunc(float32 arg);

/// approximate inverse errfunc
extern float32 approxInvErrFunc(float32 arg);

/// calculate random number with unit Gaussian distribution
extern float32 gaussianRandom();



// ---- low dimensional eigenvalue problems


/// Compute the eigenstaructure of the matrix 'cov'
void computeEigenStructure( Matrix3f cov, Vector3f& eigenValues, Matrix3f& eigenVectors );

/// computes eigenspace of symmetric matrix
///  m = ( a b )       result: lambda1, lambda2 - eigenvalues
///      ( b c )               v1, v2           - eigenvectors
/// epsilon: minimum value for b before standard basis is returned
/// if normalize == true -> v1, v2 are normalized
extern void eigenspace2f(const float32 a, const float32 b, const float32 c,
                                 float32 &lambda1, Vector2f &v1, float32 &lambda2, Vector2f &v2,
                                 const float32 epsilon = 0.0f, bool normalize = true);

/// computes eigenvalues of symmetic matrix
///  m = ( a b )
///      ( b c )
/// result: lambda1, lambda2 - eigenvalues
extern void eigenValuesSym2f(const float32 a, const float32 b, const float32 c,
                                     float32 &lambda1, float32 &lambda2);

extern void eigenspaceMatQuad2f(const Matrix2f &m, float32 &lambda1, float32 &lambda2, Matrix2f &eSpace,
                                        bool sqrtEigenvalues = true, const float32 epsilon = 0.0f, bool normalize = true);

extern Matrix2f quadEigenSpaceRecompose(const Matrix2f &eigenspace, const float32 lambda1, const float32 lambda2);

/// v0, v1 define ellipsoid, transform to orthogonal main axises
extern void mainAxis2f(Vector2f &v0, Vector2f &v1);



// ---- linear algebra  & geometry


/// Solve 2x2 linear system
extern bool linsolve2f(const Matrix2f &m, const Vector2f &v, Vector2f &solution);

/// Solve 2x2 linear system, more general template version
template <typename FloatType>
bool  linsolve2(
   const StaticMatrix<FloatType, 2, 2> &m,
   const StaticVector<FloatType, 2> &v,
   StaticVector<FloatType, 2> &solution,
   FloatType EPSILON = (FloatType)1E-8
);

/// Solve 3x3 linear system
extern bool linsolve3f(const Matrix3f &m, const Vector3f &v, Vector3f &solution);

/// Get distance between a point and a section
extern float32 getDistancePointSection( const Vector3f& p, const Vector3f start, const Vector3f end );

/// Return the world coordinates defined by v
/// alpha is in the x-y-plane from [0;PI];
/// beta is in the x-z-plane from [0;2*PI]
extern void getAbsoluteAngle( Vector3f v, float& alpha, float& beta );


template<class FloatType>
FloatType determinant(const StaticMatrix<FloatType, 3, 3> &m);

template<class FloatType>
FloatType determinant(const StaticVector<FloatType, 3> &v0,
                      const StaticVector<FloatType, 3> &v1,
                      const StaticVector<FloatType, 3> &v2);

template<class FloatType>
FloatType determinant(const StaticMatrix<FloatType, 2, 2> &m);

template<class FloatType>
FloatType determinant(const StaticVector<FloatType, 2> &v0,
                      const StaticVector<FloatType, 2> &v1);

extern float32 calcMinProjection(const Vector3f &center, const Vector3f &prjVect, const BoundingBox3f &bb);


/// returns orthogonal componenent of nonOrtho to v. assumes norm(v) == 1. result is not normalized.
extern Vector3f orthogonalize(const Vector3f &v, const Vector3f &nonOrtho);

/// assumes norm(v) == 1
extern Vector3f makeOrthonormalVector(const Vector3f &v);

/// projects vector x (world coordinates) into local (n,u,v) system,
/// scales it to point into the plane n==1 and returns u and v coordinates
/// u,v,n assumed to be orthonormal
extern Vector2f projectToN1Plane(const Vector3f &x, const Vector3f &n, const Vector3f &u, const Vector3f &v);

/// projects vector v to plane defined by normal n in direction d
/// (adding as much of d to v to reach the plane)
/// minPrjDenominator limits the enlargment caused by projection singularities
extern Vector3f projectVectByNormalAndDirection(const Vector3f v, const Vector3f n, const Vector3f d,
                                                        const float32 minPrjDenominator = 1.0E-7f);


/// calculate orthonormal coordinate system [tangent_u, tangent_v,normal]
extern Matrix3f calcTangentSystem( const Vector3f &normal );


// ---- misc


extern void make16BitNormal(const Vector3f normal, int16 &normalX, int16 &normalY, int16 &normalZ);
inline Vector3f unpack16BitNormal(const int16 normalX, const int16 normalY, const int16 normalZ);

template <class FloatType, unsigned dim>
void limitLengthRatioToSmaller(StaticVector<FloatType, dim> &vect1, StaticVector<FloatType, dim> &vect2,
                               const FloatType maxFact);

template <class FloatType>
void limitRatioToSmaller(FloatType &l1, FloatType &l2, const FloatType maxFact);

template <class FloatType>
void limitRatioToLarger(FloatType &l1, FloatType &l2, const FloatType maxFact);

/*template <class FloatType>
int32 getExponentOfTwo(const FloatType v) {
   notImplemented();
}*/
extern int32 getExponentOfTwo(const float32 v);
extern int32 getExponentOfTwo(const float64 v);



// ---- bisection search



/** search in a monotonicly raising list of entries: search for first entry in a vector wich has a value
    larger than searchValue. class EntryType must define operators <= and >= for comparison with NumericalType*/
template<typename EntryType, typename NumericalType>
inline card32 bisectionSearch(const vector<EntryType> &entries, card32 lower, card32 upper, NumericalType searchValue) {
   while (upper - lower > 1) {
      card32 mid = (upper + lower) / 2;
      if (entries[mid] <= searchValue) {
         lower = mid;
      } else {
         upper = mid;
      }
   }
   if (entries[lower] >= searchValue) {
      return lower;
   } else {
      return upper;
   }
}

/// returns index of first value >= search value
template<class ElemPtrType, class NumericalType>
inline card32 arrayBisectionSearch(const ElemPtrType entries, card32 lower, card32 upper, NumericalType searchValue) {
   while (upper - lower > 1) {
      card32 mid = (upper + lower) / 2;
      if (entries[mid] <= searchValue) {
         lower = mid;
      } else {
         upper = mid;
      }
   }
   if (entries[lower] >= searchValue) {
      return lower;
   } else {
      return upper;
   }
}


#endif
