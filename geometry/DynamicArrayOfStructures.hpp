//---------------------------------------------------------------------------
#ifndef DYNAMICARRAYOFSTRUCTURES_HPP
#define DYNAMICARRAYOFSTRUCTURES_HPP
//---------------------------------------------------------------------------
#include "DynamicArrayOfStructures.h"
#include "StructureDescriptor.hpp"
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------


inline char *DynamicArrayOfStructures::getDataPtr() const {
   return data;
}

inline AttributeAccessToken DynamicArrayOfStructures::getAAT(string name) const {
   return getDescr()->getToken(name);
}

inline AttributeAccessToken DynamicArrayOfStructures::getAAT(string name, card32 numComponents, card32 dataFormat) const {
   return getDescr()->getToken(name, numComponents, dataFormat);
}

inline bool DynamicArrayOfStructures::providesAttribute(string name) const {
   return getDescr()->providesAttribute(name);
}

inline bool DynamicArrayOfStructures::providesAttribute(string name, card32 numComponents, card32 dataFormat) const {
   return getDescr()->providesAttribute(name, numComponents, dataFormat);
}

inline mpcard DynamicArrayOfStructures::getNumEntries() const {
   return numEntries;
}

inline mpcard DynamicArrayOfStructures::getElementSize() const {
   return elementSize;
}


template<class Format, int dim>
inline StaticVector<Format, dim> DynamicArrayOfStructures::get(mpcard i,AAT token) const {
   #ifdef _DEBUG
   pAssert(sizeof(StaticVector<Format, dim>) == token.size);
   #endif
   pAssert(i<numEntries);
   char *pos = data + i*elementSize  + token.offset;
   return *((StaticVector<Format, dim>*)(pos));
}

template<class Format, int dim>
inline void DynamicArrayOfStructures::set(mpcard i, AAT token, const StaticVector<Format, dim> &v) {
   #ifdef _DEBUG
   pAssert(sizeof(StaticVector<Format, dim>) == token.size);
   #endif
   pAssert(i<numEntries);
   char *pos = data + i*elementSize  + token.offset;
   *((StaticVector<Format, dim>*)(pos)) = v;
}


template<typename ScalarType>
inline ScalarType DynamicArrayOfStructures::get(mpcard i,AAT token) const {
   #ifdef _DEBUG
   pAssert(sizeof(ScalarType) == token.size);
   #endif
   pAssert(i<numEntries);
   char *pos = data + i*elementSize  + token.offset;
   return *((ScalarType*)(pos));
}

template<typename ScalarType>
inline void DynamicArrayOfStructures::set(mpcard i, AAT token, ScalarType v) {
   #ifdef _DEBUG
   pAssert(sizeof(ScalarType) == token.size);
   #endif
   pAssert(i<numEntries);
   char *pos = data + i*elementSize  + token.offset;
   *((ScalarType*)(pos)) = v;
}


template<class Format, int dim1, int dim2>
inline StaticMatrix<Format, dim1, dim2> DynamicArrayOfStructures::get(mpcard i,AAT token) const {
   #ifdef _DEBUG
	pAssert(sizeof(StaticMatrix<Format, dim1, dim2>) == token.size);
	#endif
	char *pos = data + i*elementSize  + token.offset;
	return *((StaticMatrix<Format, dim1, dim2>*)pos);
}


template<class Format, int dim1, int dim2>
inline void DynamicArrayOfStructures::set(mpcard i, AAT token, const StaticMatrix<Format, dim1, dim2> &m) {
   #ifdef _DEBUG
	pAssert(sizeof(StaticMatrix<Format, dim1, dim2>) == token.size);
	#endif
	char *pos = data + i*elementSize  + token.offset;
	*((StaticMatrix<Format, dim1, dim2>*)pos) = m;
}





inline void DynamicArrayOfStructures::setAllAttribs(mpcard index, const char *buffer) {
   char *pos = data + index*elementSize;
   for (mpcard i=0; i<elementSize; i++) {
      pos[i] = buffer[i];
   }
}


inline void DynamicArrayOfStructures::getAllAttribs(mpcard index, char *buffer) const {
   char *pos = data + index*elementSize;
   for (mpcard i=0; i<elementSize; i++) {
      buffer[i] = pos[i];
   }
}

inline char *DynamicArrayOfStructures::getDataPointer(mpcard index) {
   return data + index*elementSize;
}

inline const char *DynamicArrayOfStructures::getDataPointer(mpcard index) const {
   return data + index*elementSize;
}

template <typename FloatType>
inline void DynamicArrayOfStructures::getAsDVector(mpcard index, DynamicVector<FloatType> &values) const {
   pAssert( values.getDim() >= getDescr()->computeOverallDimension() );
	const char *dataPtr = getDataPointer(index);
	const size_t n = sDescr->getNumAttributes();
	mpcard vecIndex = 0;
	for (size_t i=0; i<n; i++) {
      card32 numComponents = sDescr->getAttribute((card32)i).numComponents;
		for (card32 j=0; j<numComponents; j++) {
         switch (sDescr->getAttribute((card32)i).dataFormat) {
				case SAD::DATA_FORMAT_CARD8: {
					values[vecIndex++] = (FloatType)*((card8*)dataPtr);
					dataPtr += sizeof(card8);
					break;
				}
				case SAD::DATA_FORMAT_CARD16: {
					values[vecIndex++] = (FloatType)*((card16*)dataPtr);
					dataPtr += sizeof(card16);
					break;
				}
				case SAD::DATA_FORMAT_CARD32: {
					values[vecIndex++] = (FloatType)*((card32*)dataPtr);
					dataPtr += sizeof(card32);
					break;
				}
				case SAD::DATA_FORMAT_CARD64: {
					values[vecIndex++] = (FloatType)*((card64*)dataPtr);
					dataPtr += sizeof(card64);
					break;
				}
				case SAD::DATA_FORMAT_FLOAT32: {
					values[vecIndex++] = (FloatType)*((float32*)dataPtr);
					dataPtr += sizeof(float32);
					break;
				}
				case SAD::DATA_FORMAT_FLOAT64: {
					values[vecIndex++] = (FloatType)*((float64*)dataPtr);
					dataPtr += sizeof(float64);
					break;
				}
				case SAD::DATA_FORMAT_INT8: {
					values[vecIndex++] = (FloatType)*((int8*)dataPtr);
					dataPtr += sizeof(int8);
					break;
				}
				case SAD::DATA_FORMAT_INT16: {
					values[vecIndex++] = (FloatType)*((int16*)dataPtr);
					dataPtr += sizeof(int16);
					break;
				}
				case SAD::DATA_FORMAT_INT32: {
					values[vecIndex++] = (FloatType)*((int32*)dataPtr);
					dataPtr += sizeof(int32);
					break;
				}
				case SAD::DATA_FORMAT_INT64: {
					values[vecIndex++] = (FloatType)*((int64*)dataPtr);
					dataPtr += sizeof(int64);
					break;
				}
				default: pAssert(false); break;
			}
		}
	}
}
template <typename FloatType>
inline void DynamicArrayOfStructures::setByDVector(mpcard index, const DynamicVector<FloatType> &values) {
	char *dataPtr = getDataPointer(index);
	pAssert( values.getDim() >= getDescriptor()->computeOverallDimension() );
	const size_t n = sDescr->getNumAttributes();
	mpcard vecIndex = 0;
	for (size_t i=0; i<n; i++) {
		card32 numComponents = sDescr->getAttribute(i).numComponents;
		for (card32 j=0; j<numComponents; j++) {
			switch (sDescr->getAttribute(i).dataFormat) {
				case SAD::DATA_FORMAT_CARD8: {
					*((card8*)dataPtr) = (card8)values[vecIndex++];
					dataPtr += sizeof(card8);
					break;
				}
				case SAD::DATA_FORMAT_CARD16: {
					*((card16*)dataPtr) = (card16)values[vecIndex++];
					dataPtr += sizeof(card16);
					break;
				}
				case SAD::DATA_FORMAT_CARD32: {
					*((card32*)dataPtr) = (card32)values[vecIndex++];
					dataPtr += sizeof(card32);
					break;
				}
				case SAD::DATA_FORMAT_CARD64: {
					*((card64*)dataPtr) = (card64)values[vecIndex++];
					dataPtr += sizeof(card64);
					break;
				}
				case SAD::DATA_FORMAT_FLOAT32: {
					*((float32*)dataPtr) = (float32)values[vecIndex++];
					dataPtr += sizeof(float32);
					break;
				}
				case SAD::DATA_FORMAT_FLOAT64: {
					*((float64*)dataPtr) = (float64)values[vecIndex++];
					dataPtr += sizeof(float64);
					break;
				}
				case SAD::DATA_FORMAT_INT8: {
					*((int8*)dataPtr) = (int8)values[vecIndex++];
					dataPtr += sizeof(int8);
					break;
				}
				case SAD::DATA_FORMAT_INT16: {
					*((int16*)dataPtr) = (int16)values[vecIndex++];
					dataPtr += sizeof(int16);
					break;
				}
				case SAD::DATA_FORMAT_INT32: {
					*((int32*)dataPtr) = (int32)values[vecIndex++];
					dataPtr += sizeof(int32);
					break;
				}
				case SAD::DATA_FORMAT_INT64: {
					*((int64*)dataPtr) = (int64)values[vecIndex++];
					dataPtr += sizeof(int64);
					break;
				}
				default: pAssert(false); break;
			}
		}
	}
}





#endif
