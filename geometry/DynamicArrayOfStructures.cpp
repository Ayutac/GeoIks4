//---------------------------------------------------------------------------
#include "precompiledHeaders.h"
//---------------------------------------------------------------------------
#include "DynamicArrayOfStructures.h"
#include "DynamicArrayOfStructures.hpp"
//---------------------------------------------------------------------------
#include "LinearAlgebra.hpp"
#include "Tools.h"
#include "Exceptions.h"
#include "NumericalClassProperty.h"
#include "StringClassProperty.h"
#include "ClassMethods.h"
#include "GeoXOutput.h"
#include "OpenGLPlotter2D.h"
#include "OpenGLCanvas2D.h"
//#include "SingleWidgetCanvas.h"
//#include "GeoXTextView.h"
#include "ObjectClassProperty.h"
//---------------------------------------------------------------------------
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------



IMPLEMENT_GEOX_CLASS(DynamicArrayOfStructures, 0) {
   BEGIN_CLASS_INIT(DynamicArrayOfStructures)
   ADD_STRING_PROP_METHOD( description, 0, &DynamicArrayOfStructures::getDescrString, NULL )
	( (MetaClass *)getClass())->actProperty_setEditReadonly( true );
   SET_EDIT_READONLY( true )
   SET_STREAM_PROPERTY( false )
   ADD_MAX_PLATFORM_CARD_PROP( numEntries, 0 )
   ( (MetaClass *)getClass())->actProperty_setEditReadonly( true );
   SET_EDIT_READONLY( true )
   SET_STREAM_PROPERTY( false )
   ADD_MAX_PLATFORM_CARD_PROP( elementSize, 0 )
   ( (MetaClass *)getClass())->actProperty_setEditReadonly( true );
   SET_EDIT_READONLY( true )
   SET_STREAM_PROPERTY( false )
   ADD_MAX_PLATFORM_CARD_PROP( allocatedNumEntries, 0 )
   ( (MetaClass *)getClass())->actProperty_setEditReadonly( true );
   SET_EDIT_READONLY( true )
   SET_STREAM_PROPERTY( false )
    ADD_OBJECT_PROP( sDescr, 0, StructureDescriptor::getClass(), true )
   ( (MetaClass *)getClass())->actProperty_setEditReadonly( true );
   SET_EDIT_READONLY( true )
   SET_STREAM_PROPERTY( false )

    ADD_NOARGS_METHOD(DynamicArrayOfStructures::createExampleDataSet)
}

// ----

string DynamicArrayOfStructures::getDescrString() const {
	return sDescr->getDescription();
}

/// MSVC fix
#ifndef M_PI
const double M_PI = 3.1415926535897932384626433832795;
#endif

void DynamicArrayOfStructures::createExampleDataSet()
{
	static const mpcard NUM_PTS = 32;
	StructureDescriptor sd;
	sd.pushAttrib(mSAD("position", 3, SAD::DATA_FORMAT_FLOAT32));
	sd.pushAttrib(mSAD("color", 3, SAD::DATA_FORMAT_FLOAT32));
	clearAndSetup(NUM_PTS, &sd);
	AAT positionAAT = getAAT("position");
	AAT colorAAT = getAAT("color");
	for (mpcard i=0; i<NUM_PTS; i++) {
		float32 x = (float32)i/(float32)(NUM_PTS-1) * 2 * (float32)M_PI;
		float32 c = (float32)i/(float32)(NUM_PTS-1);
		set3f(i, positionAAT, makeVector3f(sin(x), cos(x),x));
		set3f(i, colorAAT, makeVector3f(1,1,0)*c+makeVector3f(0,1,1)*(1-c));
	}
}

DynamicArrayOfStructures::DynamicArrayOfStructures() {
	sDescr = new StructureDescriptor();
   data = NULL;
   numEntries = 0;
   elementSize = 0;
   allocatedNumEntries=0;
}

void DynamicArrayOfStructures::assign(const Object* obj) {
   const DynamicArrayOfStructures *other = dynamic_cast<const DynamicArrayOfStructures*>(obj);
   pAssert(other != NULL);
   free(data);
	delete sDescr;
   *this = *other;

   const mpcard numBytes = other->elementSize * other->numEntries;
   this->data = (char*)malloc(numBytes);
   if (this->data == NULL && numBytes > 0) throw PException("PointSet::assign() - cannot allocate memory.");
   memcpy(this->data, other->data, numBytes);
   this->allocatedNumEntries = numEntries;

	this->sDescr = (StructureDescriptor*)(other->sDescr->copy());
}

void DynamicArrayOfStructures::write(OutputObjectStream *s) const {
	s->writeObject(sDescr);
   // write data
   s->writeMaxPlatformCard(numEntries);
   s->writeMaxPlatformCard(elementSize); // for checking only!
   s->writeBuffer(data, numEntries*elementSize);
}

void DynamicArrayOfStructures::read(InputObjectStream *s) {
	delete sDescr;
	s->readObject((Persistent*&)sDescr);
	if(sDescr)
		elementSize = sDescr->getSize();
   else
      elementSize = 0;

   // retrieve extends
   s->readMaxPlatformCard(numEntries);
   mpcard elementSizeTest;

   s->readMaxPlatformCard(elementSizeTest);
   pAssert(elementSize == elementSizeTest);
   if (elementSize != elementSizeTest) error("element size does not match upon reading a DAOS.");

   // read data
   free(data);
   data = (char*)malloc(numEntries * elementSize);
   if (data == NULL && numEntries * elementSize > 0) throw PException("PointSet::read() - cannot allocate memory.");
   s->readBuffer(data, numEntries*elementSize);

   this->allocatedNumEntries = numEntries;
}

void DynamicArrayOfStructures::clear() {
   numEntries = 0;
   free(data);
   data = NULL;
   allocatedNumEntries = numEntries;
}

void DynamicArrayOfStructures::clearAndSetup(mpcard numEntries, const StructureDescriptor *vertDescr) {
   this->numEntries = numEntries;
	if (this->sDescr != vertDescr) {
		delete this->sDescr;
		this->sDescr = (StructureDescriptor*)(vertDescr->copy());
   }
   this->elementSize = vertDescr->getSize();
   free(data);
   data = (char*)malloc(numEntries * elementSize);
   if (data == NULL && numEntries * elementSize > 0) throw PException("PointSet::clearAndSetup() - cannot allocate memory.");
   memset(data, 0, numEntries * elementSize);

   this->allocatedNumEntries = numEntries;
}


const StructureDescriptor *DynamicArrayOfStructures::getDescr() const {
	return sDescr;
}

DynamicArrayOfStructures *DynamicArrayOfStructures::appendAttribs(const StructureDescriptor *newAttribs) const {
   DynamicArrayOfStructures *result = new DynamicArrayOfStructures();
	StructureDescriptor *newDescr = (StructureDescriptor*)(sDescr->copy());
	newDescr->mergeAttributes(newAttribs);
   result->clearAndSetup(numEntries, newDescr);

	const mpint sourceSize = sDescr->getSize();
   const mpint destSize = newDescr->getSize();
	#ifdef _DEBUG
	const mpint addDestOffset = destSize - sourceSize;
   pAssert(addDestOffset >= 0);
	#endif
   char *dataDest = result->data;
   char *dataSource = data;
   for (mpcard vPos=0; vPos<numEntries; vPos++){
      for (mpint b=0; b<sourceSize; b++) {
         *dataDest = *dataSource;
         dataDest++;
         dataSource++;
      }
      for (mpint b=sourceSize; b<destSize; b++) {
         *dataDest = 0;
         dataDest++;
      }
      // dataDest += addDestOffset;
   }

   delete newDescr;
	return result;
}

string DynamicArrayOfStructures::toString()
{
	ostringstream out;
	out << "# This is a dump of a DynamicArrayOfStructures. Here is short description of the format:\n"
		 << "# Any line beginning with '#' is a comment and will be ignored.\n"
		 << "# The first non-comment contains the structure descriptor:\n"
		 << "# The format is <name_1> <dim_1> <type_1> <name_2> <dim_2> <type_2> ... <name_n> <dim_n> <type_n>\n"
		 << "# For example:\n"
		 << "# position 3 float32 color 4 card8 flags 1 int32\n"
		 << "# Each line afterwards just lists the numerical values stored in the attribute channels in the specified order.\n";
	if (sDescr == NULL) {
		out << "#\n# EMPTY - no structure descriptor\n#\n";
	} else {
		out << sDescr->toString();
		mpcard channelDim = sDescr->computeOverallDimension();
		DVectorD dump(channelDim);
		for (mpcard i=0; i<getNumEntries(); i++) {
			getAsDVector(i, dump);
			for (mpcard j=0; j<channelDim; j++) {
				out << dump[j] << "\t";
			}
			out << "\n";
		}
	}
	return out.str();
}

void DynamicArrayOfStructures::fromString(const string &buffer)
{
   UNUSED(buffer);
	notImplemented();
}

const MetaClass *DynamicArrayOfStructures::getDefaultViewer()
{
	return OpenGLPlotter2D::getClass();
	//return GeoXTextView::getClass();
}

const MetaClass *DynamicArrayOfStructures::getDefaultViewerDefaultCanvas()
{
	return OpenGLCanvas2D::getClass();
	//return SingleWidgetCanvas::getClass();
}


void DynamicArrayOfStructures::changeSize(const mpcard newSize) {

   // change ad lib
   mpcard factor = 2;

   if (newSize==0)
   {
      clear();
      return;
   }

	this->elementSize = sDescr->getSize();
   mpcard allocatedMem = allocatedNumEntries*elementSize;
   mpcard actualMem = numEntries*elementSize;
   mpcard neededMem = newSize*elementSize;
   
   // got larger && large enough --> exact reallocation (exactly the number of bytes demanded)
   if ((neededMem > allocatedMem*factor) || (this->allocatedNumEntries==0))
   {
      data = (char*)realloc(data, neededMem);
      if (data == NULL && neededMem > 0) throw PException("PointSet::changeNumEntries() - cannot reallocate memory.");
      pAssert(data);
      allocatedNumEntries = newSize;
   }
   // got only a bit larger --> double memory (but no more than 50MB more than needed)
   else if (neededMem > allocatedMem) {
      mpcard incByFactorMem = allocatedMem*factor;
      pAssert(incByFactorMem >= neededMem);

      if (incByFactorMem - neededMem > 50*1024*1024) {
         incByFactorMem = ((neededMem + 50*1024*1024) / elementSize) * elementSize; // round to proper numEntries (integer div!)
      }
      data = (char*)realloc(data, incByFactorMem);
      if (data == NULL && incByFactorMem > 0) throw PException("PointSet::changeNumEntries() - cannot allocate memory.");

      pAssert(data);

      allocatedNumEntries = incByFactorMem / elementSize;
   }
   // got smaller && small enough --> exact reallocation (exactly the number of bytes demanded)
   else if (neededMem <= allocatedMem/factor)
   {
      data = (char*)realloc(data, neededMem);
      if (data == NULL && neededMem > 0) throw PException("PointSet::changeNumEntries() - cannot reallocate memory.");
      pAssert(data);
      allocatedNumEntries = newSize;
   // got only a bit smaller --> zero remaining area
   }

   // zero new mem
   if (neededMem > actualMem) {
      memset(((char*)data)+actualMem, 0, neededMem - actualMem);
   }

   this->numEntries = newSize;
}


void DynamicArrayOfStructures::addPoint(const char *data) {
   changeSize(numEntries+1);
   if( data)
	setAllAttribs(numEntries-1, data);
}

void DynamicArrayOfStructures::addPointFront(const char *buffer)
{
    // create and copy array into something temporary
	 mpcard allocatedMem = allocatedNumEntries * sDescr->getSize();

    char* tmp = new char[allocatedMem];
    memcpy(tmp, getDataPointer(), allocatedMem);

    // change the whole size of the array
    changeSize(numEntries+1);

    // copy the content of previous memory to the place just right after the first element
	 memcpy(getDataPointer() + sDescr->getSize(), tmp, allocatedMem);
    delete [] tmp;

    // set point data
	 setAllAttribs(0, buffer);
}


void DynamicArrayOfStructures::deletePoint(const mpcard index) {
   pAssert(numEntries > 0);
   pAssert(index < numEntries);

   if (index < numEntries-1) {
		setAllAttribs(index, getDataPointer(numEntries-1));
   }
   changeSize(numEntries-1);
}

DynamicArrayOfStructures *DynamicArrayOfStructures::subset(const vector<mpcard>& indices)
{
   DynamicArrayOfStructures *result = new DynamicArrayOfStructures();
   const size_t numEntries = indices.size();
   result->clearAndSetup(numEntries, getDescr());
   for (size_t i=0; i<numEntries; i++)
		result->setAllAttribs(i, getDataPointer(indices[i]));
   return result;
}


DynamicArrayOfStructures::~DynamicArrayOfStructures() {
   free(data);
	delete sDescr;
}
