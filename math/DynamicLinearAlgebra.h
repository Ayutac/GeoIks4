//---------------------------------------------------------------------------
#ifndef DynamicLinearAlgebraH
#define DynamicLinearAlgebraH
//---------------------------------------------------------------------------
#include <math.h>
#include <string>
//---------------------------------------------------------------------------
#include "PTypes.h"
#include "LinearAlgebra.h"
#include "PAssert.h"
//---------------------------------------------------------------------------
#include "Exceptions.h"
//---------------------------------------------------------------------------

using namespace std;

class OutputObjectStream;
class InputObjectStream;

// ---------------------------------------------------------------------------
//
//                                 DynamicVector
//
// ---------------------------------------------------------------------------


template <typename FloatType>
class DynamicVector {
 private:
	mpcard dim;
   FloatType *v;

 public:
   //  ---- constructors, copy constructor
	inline DynamicVector<FloatType>(mpcard dim = 0);
   inline DynamicVector<FloatType>(const DynamicVector<FloatType> &init);

	inline mpcard getDim() const;
   /// alias to getDim()
	inline mpcard size() const;
	inline void setDim(const mpcard dim);
   /// set all entries to (FloatType)0
   inline void setZero();

	inline FloatType& operator[](const mpcard &index);
	inline const FloatType& operator[](const mpcard &index) const;

   //  ----  operators +, -, *, /, etc.
   inline DynamicVector<FloatType> operator+(const DynamicVector<FloatType> &op) const;
   inline DynamicVector<FloatType> operator-(const DynamicVector<FloatType> &op) const;
   inline DynamicVector<FloatType> operator-() const;
   inline FloatType operator*(const DynamicVector<FloatType> op) const;
   inline DynamicVector<FloatType> operator*(const FloatType &s) const;
   inline DynamicVector<FloatType> operator/(const FloatType &s) const;
   inline DynamicVector<FloatType> componentProduct(const DynamicVector<FloatType> &op) const;
   inline DynamicVector<FloatType> componentAddScalar(const FloatType &s) const;
   inline DynamicVector<FloatType> componentSubstractScalar(const FloatType &s) const;
   inline FloatType componentSum() const;
   inline FloatType average() const;

   //  ---- operators +=, -=, *=, /=
   inline DynamicVector<FloatType> operator+=(const DynamicVector<FloatType> &op);
   inline DynamicVector<FloatType> operator-=(const DynamicVector<FloatType> &op);
   inline DynamicVector<FloatType> operator*=(const FloatType &s);
   inline DynamicVector<FloatType> operator/=(const FloatType &s);

   //  ---- operators =, ==, !=
   inline void operator=(const DynamicVector<FloatType> &op);
   inline bool operator==(const DynamicVector<FloatType> op) const;
   inline bool operator!=(const DynamicVector<FloatType> op) const;

   inline FloatType maxComponent() const;
   inline FloatType minComponent() const;
   inline DynamicVector<FloatType> absoluteValues() const;

   inline bool allComponentsLargerEqual(const DynamicVector<FloatType> &op) const;
   inline bool allComponentsLarger(const DynamicVector<FloatType> &op) const;
   inline bool allComponentsSmallerEqual(const DynamicVector<FloatType> &op) const;
   inline bool allComponentsSmaller(const DynamicVector<FloatType> &op) const;
   inline float32 getNorm() const;

   //  ---- misc.
   void print();
   string toString() const;

   inline FloatType* data();
   inline const FloatType* data() const;
   inline FloatType* dataPtr();
   inline const FloatType* dataPtr() const;
	inline DynamicVector<FloatType> copyElements(const mpcard &howMany) const;

   void write(OutputObjectStream *s) const;
   void read(InputObjectStream *s);	


   //Warning: data at "dataPointer" will be deleted by deleting DynamicVector
	inline void setDataPointer(FloatType* x, const mpcard dim);

   // ---- destructor
   inline ~DynamicVector();
};



// ---------------------------------------------------------------------------
//
//                                 DynamicMatrix
//
// ---------------------------------------------------------------------------



template <typename FloatType>
class DynamicMatrix {
 private:
	mpcard rows;
	mpcard columns;
   DynamicVector<FloatType> *theColumns;

 public:

   // ---- constructors, copy constructor
	inline DynamicMatrix( mpcard c = 0, mpcard r = 0, bool initAsIdentity = true);
   inline DynamicMatrix(const DynamicMatrix<FloatType> &init);

	inline void setDimension(mpcard columns = 0, mpcard rows = 0, bool initAsIdentity = true);
   inline void setZero();
	inline mpcard getRowsDim() const;
	inline mpcard getColsDim() const;
	inline mpcard getRows() const;
	inline mpcard getColumns() const;

	inline DynamicVector<FloatType>& operator[](const mpcard &index);
	inline const DynamicVector<FloatType>& operator[](const mpcard &index) const;

   //  ----  operators +, -, *, /
   inline DynamicMatrix<FloatType> operator+(const DynamicMatrix<FloatType> &op) const;
   inline DynamicMatrix<FloatType> operator-(const DynamicMatrix<FloatType> &op) const;
   inline DynamicMatrix<FloatType> operator-() const;
   inline DynamicMatrix<FloatType> operator*(const FloatType &s) const;
   inline DynamicMatrix<FloatType> operator*(const DynamicMatrix<FloatType> &op) const;
   inline DynamicMatrix<FloatType> operator/(const FloatType &s) const;

   //  ---- operators +=, -=, *=, /=
   inline DynamicMatrix<FloatType> operator+=(const DynamicMatrix<FloatType> &op);
   inline DynamicMatrix<FloatType> operator-=(const DynamicMatrix<FloatType> &op);
   inline DynamicMatrix<FloatType> operator*=(const FloatType &op);
   inline DynamicMatrix<FloatType> operator*=(const DynamicMatrix<FloatType> &op);
   inline DynamicMatrix<FloatType> operator/=(const FloatType &op);

   //  ---- operators =, ==, !=
   inline void operator=(const DynamicMatrix<FloatType> &op);
   inline bool operator==(const DynamicMatrix<FloatType> &op) const;
   inline bool operator!=(const DynamicMatrix<FloatType> &op) const;

   // ---- multiplying with vectors
   inline DynamicVector<FloatType> operator*(const DynamicVector<FloatType> &v) const;

   // ----  misc.
   inline void print();
   string toString() const;

   inline void copyDataToArray(FloatType* target) const;
   inline void copyDataFromArray(FloatType* source);
   ///get top-left 3x3 values (for debug)
   inline Matrix3f getMatrix3f();

	inline void changeRows(mpcard row1, mpcard row2);
	inline void multRow(mpcard row, FloatType value);
	inline void combineRows(mpcard row, mpcard with, FloatType by);
	inline void addRows(mpcard row, mpcard with);
   inline DynamicMatrix<FloatType> transpose() const;

   /// copies subset of columns to new matrix
	inline DynamicMatrix<FloatType> copyColumns(mpcard startColumn, mpcard numColumns);

   inline FloatType getDeterminant() const;

	inline FloatType maxComponent() const;
	inline FloatType minComponent() const;

   void write(OutputObjectStream *s) const;
   void read(InputObjectStream *s);	

   // ---- destructor
	inline ~DynamicMatrix();
};


// ---------------------------------------------------------------------------
//
//                                   instances
//
// ---------------------------------------------------------------------------


typedef DynamicVector<float> DVectorF;
typedef DynamicVector<double> DVectorD;
typedef DynamicVector<int> DVectorI;

typedef DynamicMatrix<float> DMatrixF;
typedef DynamicMatrix<double> DMatrixD;
typedef DynamicMatrix<int> DMatrixI;


// ---------------------------------------------------------------------------
//
//                                 aux. routines
//
// ---------------------------------------------------------------------------


template <typename FloatType>
inline DynamicVector<FloatType> scalarDVector(mpcard dim, const FloatType &s);

template <typename FloatType>
inline DynamicVector<FloatType> nullDVector(mpcard dim);

template <typename FloatType>
inline DynamicVector<FloatType> makeDVector(mpcard dim, const FloatType * dataptr);

template <typename FloatType1, typename FloatType2>
inline void convertDVectorType(const DynamicVector<FloatType1> &source, DynamicVector<FloatType2> &dest);


template <typename FloatType>
inline DynamicMatrix<FloatType> scalarDMatrix(mpcard c, mpcard r, const FloatType &s);

template <typename FloatType>
inline DynamicMatrix<FloatType> nullDMatrix(mpcard c, mpcard r);

/// v1^t v2
template <typename FloatType>
inline DynamicMatrix<FloatType> outerProduct( const DynamicVector<FloatType> &v1,const DynamicVector<FloatType> &v2 );

template <typename FloatType>
inline FloatType normQuad(const DynamicVector<FloatType> &v);

template <typename FloatType>
inline FloatType norm(const DynamicVector<FloatType> &v);

template <typename FloatType>
inline FloatType normInfinity(const DynamicVector<FloatType> &v);

template <typename FloatType>
inline mpcard searchPivot(const DynamicMatrix<FloatType> &mat, mpcard step);

template <typename FloatType>
inline DynamicMatrix<FloatType> invertMatrix(const DynamicMatrix<FloatType> &mat);


template <typename FloatType>
inline DynamicVector<FloatType> maxVector( const DynamicVector<FloatType> &v1,const DynamicVector<FloatType> &v2);


template <typename FloatType>
inline DynamicVector<FloatType> minVector( const DynamicVector<FloatType> &v1,const DynamicVector<FloatType> &v2);




#include "DynamicLinearAlgebra.hpp"

#endif
