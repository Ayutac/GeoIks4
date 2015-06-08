//---------------------------------------------------------------------------
#include "precompiledHeaders.h"
//---------------------------------------------------------------------------
#include "DynamicArrayOfStructures2D.h"
#include "DynamicArrayOfStructures2D.hpp"
#include "GeoXImageView.h"
#include "SingleWidgetCanvas.h"
#include "ClassMethods.h"
#include "NumericalClassProperty.h"
#include "ObjectClassProperty.h"
//---------------------------------------------------------------------------
#include "LinearAlgebra.hpp"
//---------------------------------------------------------------------------



IMPLEMENT_GEOX_CLASS(DynamicArrayOfStructures2D, 0) {
	BEGIN_CLASS_INIT(DynamicArrayOfStructures2D)
	ADD_OBJECT_PROP( data, 0, DynamicArrayOfStructures::getClass(), true )
	ADD_MAX_PLATFORM_CARD_PROP(width, 0)
	ADD_MAX_PLATFORM_CARD_PROP(height, 0)
    ADD_NOARGS_METHOD(DynamicArrayOfStructures2D::createExampleDataSet)
}

// ----

void DynamicArrayOfStructures2D::createExampleDataSet()
{
	static const mpcard WIDTH = 128;
	static const mpcard HEIGHT = 64;
	StructureDescriptor sd;
	sd.pushAttrib(mSAD("color", 3, SAD::DATA_FORMAT_FLOAT32));
	clearAndSetup(WIDTH, HEIGHT, &sd);
	AAT colorAAT = getAAT("color");
	for (mpcard y=0; y<HEIGHT; y++) {
		for (mpcard x=0; x<WIDTH; x++) {
			float32 xf = (float32)x/(float32)WIDTH;
			float32 yf = (float32)y/(float32)HEIGHT;
			set3f(x,y, colorAAT, makeVector3f(xf, 0, yf));
		}
	}
}

DynamicArrayOfStructures2D::DynamicArrayOfStructures2D() {
   data = NULL;
	width = 0;
	height = 0;
}

void DynamicArrayOfStructures2D::assign(const Object* obj) {
	const DynamicArrayOfStructures2D *other = dynamic_cast<const DynamicArrayOfStructures2D*>(obj);
	if (other) {
		delete this->data;
		this->data = (DynamicArrayOfStructures*)other->data->copy();
		this->width = other->width;
		this->height = other->height;
	} else {
		pAssert(false);
	}
}

void DynamicArrayOfStructures2D::clear() {
	delete data;
	data = NULL;
	width = 0;
	height = 0;
}

void DynamicArrayOfStructures2D::clearAndSetup(mpcard width, mpcard height, const StructureDescriptor *vertDescr) {
	delete data;
	data = new DynamicArrayOfStructures();
	data->clearAndSetup(width*height, vertDescr);
	this->width = width;
	this->height = height;
}


const StructureDescriptor *DynamicArrayOfStructures2D::getDescr() const {
	return data->getDescr();
}

DynamicArrayOfStructures2D *DynamicArrayOfStructures2D::appendAttribs(const StructureDescriptor *newAttribs) const {
	DynamicArrayOfStructures2D *result = new DynamicArrayOfStructures2D();
	result->width = this->width;
	result->height = this->height;
	result->data = this->data->appendAttribs(newAttribs);
	return result;
}


const MetaClass *DynamicArrayOfStructures2D::getDefaultViewer()
{
	return GeoXImageView::getClass();
}

const MetaClass *DynamicArrayOfStructures2D::getDefaultViewerDefaultCanvas()
{
	return SingleWidgetCanvas::getClass();
}


DynamicArrayOfStructures2D::~DynamicArrayOfStructures2D() {
	delete data;
}

mpcard DynamicArrayOfStructures2D::getWidth() const
{
	return width;
}

mpcard DynamicArrayOfStructures2D::getHeight() const
{
	return height;
}

const DynamicArrayOfStructures *DynamicArrayOfStructures2D::getData() const
{
	return data;
}
