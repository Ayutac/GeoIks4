//---------------------------------------------------------------------------
#ifndef DYNAMICARRAYOFSTRUCTURES2D_HPP
#define DYNAMICARRAYOFSTRUCTURES2D_HPP
//---------------------------------------------------------------------------
#include "DynamicArrayOfStructures2D.h"
#include "DynamicArrayOfStructures.hpp"
#include "StructureDescriptor.hpp"
//---------------------------------------------------------------------------


inline AttributeAccessToken DynamicArrayOfStructures2D::getAAT(string name) const {
	return data->getAAT(name);
}

inline AttributeAccessToken DynamicArrayOfStructures2D::getAAT(string name, card32 numComponents, card32 dataFormat) const {
	return data->getAAT(name, numComponents, dataFormat);
}

inline bool DynamicArrayOfStructures2D::providesAttribute(string name) const {
	return data->getDescr()->providesAttribute(name);
}

inline bool DynamicArrayOfStructures2D::providesAttribute(string name, card32 numComponents, card32 dataFormat) const {
	return data->getDescr()->providesAttribute(name, numComponents, dataFormat);
}

inline mpcard DynamicArrayOfStructures2D::getNumEntries() const {
	return data->getNumEntries();
}

inline mpcard DynamicArrayOfStructures2D::getElementSize() const {
	return data->getElementSize();
}


template<class Format, int dim>
inline StaticVector<Format, dim> DynamicArrayOfStructures2D::get(mpcard x, mpcard y,AAT token) const {
	mpcard i = y*width + x;
	return data->get<Format, dim>(i, token);
}

template<class Format, int dim>
inline void DynamicArrayOfStructures2D::set(mpcard x, mpcard y, AAT token, const StaticVector<Format, dim> &v) {
	mpcard i = y*width + x;
	data->set<Format, dim>(i, token, v);
}


template<typename ScalarType>
inline ScalarType DynamicArrayOfStructures2D::get(mpcard x, mpcard y,AAT token) const {
	mpcard i = y*width + x;
	return data->get<ScalarType>(i, token);
}

template<typename ScalarType>
inline void DynamicArrayOfStructures2D::set(mpcard x, mpcard y, AAT token, ScalarType v) {
	mpcard i = y*width + x;
	data->set<ScalarType>(i, token, v);
}


template<class Format, int dim1, int dim2>
inline StaticMatrix<Format, dim1, dim2> DynamicArrayOfStructures2D::get(mpcard x, mpcard y, AAT token) const {
	mpcard i = y*width + x;
	return data->get<Format, dim1, dim2>(i, token);
}


template<class Format, int dim1, int dim2>
inline void DynamicArrayOfStructures2D::set(mpcard x, mpcard y, AAT token, const StaticMatrix<Format, dim1, dim2> &m) {
	mpcard i = y*width + x;
	data->set<Format, dim1, dim2>(i, token, m);
}





inline void DynamicArrayOfStructures2D::setAllAttribs(mpcard x, mpcard y, const char *buffer) {
	mpcard i = y*width + x;
	data->setAllAttribs(i, buffer);
}


inline void DynamicArrayOfStructures2D::getAllAttribs(mpcard x, mpcard y, char *buffer) const {
	mpcard i = y*width + x;
	data->getAllAttribs(i, buffer);
}

inline char *DynamicArrayOfStructures2D::getDataPointer(mpcard x, mpcard y) {
	mpcard i = y*width + x;
	return data->getDataPointer(i);
}

inline const char *DynamicArrayOfStructures2D::getDataPointer(mpcard x, mpcard y) const {
	mpcard i = y*width + x;
	return data->getDataPointer(i);
}

template <typename FloatType>
inline void DynamicArrayOfStructures2D::getAsDVector(mpcard x, mpcard y, DynamicVector<FloatType> &values) const {
	mpcard i = y*width + x;
	data->getAsDVector(i, values);
}

template <typename FloatType>
inline void DynamicArrayOfStructures2D::setByDVector(mpcard x, mpcard y, const DynamicVector<FloatType> &values) const {
	mpcard i = y*width + x;
	data->setByDVector(i, values);
}





#endif
