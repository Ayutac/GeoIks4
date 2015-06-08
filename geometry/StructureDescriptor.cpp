//---------------------------------------------------------------------------
#include "precompiledHeaders.h"
//---------------------------------------------------------------------------
#include "StructureDescriptor.h"
#include "StructureDescriptor.hpp"
//---------------------------------------------------------------------------
#include "AttributeAccessToken.h"
#include "AttributeAccessToken.hpp"
#include "StructureAttribDescriptor.h"
#include "StructureAttribDescriptor.hpp"
//---------------------------------------------------------------------------
#include "Tools.h"
#include "Exceptions.h"
#include "NumericalClassProperty.h"
#include "StringClassProperty.h"
#include "DynamicArrayOfStructures.h"
#include "DynamicArrayOfStructures.hpp"
#include "LinearAlgebra.hpp"
#include "StringHelper.h"
//---------------------------------------------------------------------------
#include <sstream>
//---------------------------------------------------------------------------


IMPLEMENT_GEOX_CLASS(StructureDescriptor, 0) {
	BEGIN_CLASS_INIT(StructureDescriptor)
}



StructureDescriptor::StructureDescriptor() {
   offsets.push_back(0);
}

void StructureDescriptor::assign(const Object* obj) {
   const StructureDescriptor *other = dynamic_cast<const StructureDescriptor*>(obj);
   pAssert(other != NULL);
   this->attributes = other->attributes;
   this->offsets = other->offsets;
}

bool StructureDescriptor::operator==(const StructureDescriptor *vd) const {
   return equal(vd);
}

bool StructureDescriptor::operator!=(const StructureDescriptor *vd) const {
   return !equal(vd);
}


void StructureDescriptor::write(OutputObjectStream *s) const {
   const card32 n = (card32)attributes.size();
   s->write(n);
   for (card32 i=0; i<n; i++) {
      attributes[i].write(s);
   }
}

void StructureDescriptor::read(InputObjectStream *s) {
   card32 n;
   s->read(n);
   attributes.resize(n);
   offsets.resize(n+1);
   card32 offset = 0;
   for (card32 i=0; i<n; i++) {
      attributes[i].read(s);
      offsets[i] = offset;
      offset += attributes[i].getSize();
   }
   offsets[n] = offset;
   pAssert(offsets.size() == attributes.size() + 1);
}

void StructureDescriptor::pushAttrib(StructureAttribDescriptor a) {
   attributes.push_back(a);
   card32 offset = a.getSize();
   offset += offsets[attributes.size()-1];
   offsets.push_back(offset);
   pAssert(offsets.size() == attributes.size() + 1);
}

void StructureDescriptor::clear()
{
   attributes.clear();
   offsets.clear();
   offsets.push_back(0);
}

mpcard StructureDescriptor::getNumAttributes() const {
   return attributes.size();
}

card32 StructureDescriptor::getIndex(string name) const {
   const size_t n = attributes.size();
   for (card32 i=0; i<n; i++) {
      if (attributes[i].name == name) {
         return i;
      }
   }
   throw PException("StructureDescriptor::getIndex() - name not found.");
};

bool StructureDescriptor::providesAttribute(string name) const {
	// mb - added a way to access single components of a vector-attribute directly
	// example: "color:2" corresponds to the third color component
	string number = name;
	name = extractToChar(number,':');

   const size_t n = attributes.size();
   for (card32 i=0; i<n; i++) {
      if (attributes[i].name == name) {
         return true;
      }
   }
   return false;
}

bool StructureDescriptor::providesAttribute(string name, card32 numComponents) const {
   const size_t n = attributes.size();
   for (card32 i=0; i<n; i++) {
      if (attributes[i].name == name && attributes[i].numComponents == numComponents) {
         return true;
      }
   }
   return false;
}

bool StructureDescriptor::providesAttribute(string name, card32 numComponents, card32 dataFormat) const {
	// mb - added a way to access single components of a vector-attribute directly
	// example: "color:2" corresponds to the third color component
	string number = name;
	name = extractToChar(number,':');
	if( number.length() > 0)
	{
		const size_t n = attributes.size();
		for (card32 i=0; i<n; i++)
			if (attributes[i].name == name && attributes[i].dataFormat == dataFormat )
			{
				int num;
				if( strToInt(number, num))
				{
					return (int)(num + numComponents) <= (int)(attributes[i].numComponents);
				}
				return false;
			}
		return false;
	}
	else
	{
		const size_t n = attributes.size();
		for (card32 i=0; i<n; i++) {
			if (attributes[i].name == name && attributes[i].numComponents == numComponents && attributes[i].dataFormat == dataFormat) {
				return true;
			}
		}
		return false;
	}
}

bool StructureDescriptor::providesAttribute(SAD att) const {
   const size_t n = attributes.size();
   for (card32 i=0; i<n; i++) {
      if (attributes[i] == att) {
         return true;
      }
   }
   return false;
}



card32 StructureDescriptor::getSize() const {
   pAssert(attributes.size() + 1 == offsets.size());
   return offsets[attributes.size()];
}

string StructureDescriptor::getDescription() const {
   const size_t n = attributes.size();
   if (n == 0) {
      return "(no attributes defined)";
   }
   string result;
   for (card32 i=0; i<n; i++) {
      result += attributes[i].getDescription();
      if (i != n-1) {
         result += string("; ");
      }
   }
   return result;
}


void StructureDescriptor::mergeAttributes(const StructureDescriptor *newAttribs) {
   const size_t nd = attributes.size();
   const size_t ns = newAttribs->attributes.size();
   for (card32 s=0; s<ns; s++) {
      const string currentName = newAttribs->attributes[s].name;
      bool attribFound = false;
      for (card32 d=0; d<nd; d++) {
         if (attributes[d].name == currentName) {
            if (!(   attributes[d].dataFormat    == newAttribs->attributes[s].dataFormat
                  && attributes[d].numComponents == newAttribs->attributes[s].numComponents) ) {
               throw PException("StructureDescriptor::mergeAttributes - type mismatch in duplicate field.");
            } else {
               attribFound = true;
               break;
            }
         }
      }
      if (!attribFound) {
         pushAttrib(newAttribs->attributes[s]);
      }
   }
}


AttributeAccessToken StructureDescriptor::getToken(string name) const {
	if (!providesAttribute(name)) {
		throw PException(string("StructureDescriptor::getToken() - Couldn't find attribute ") + name + ".");
	}
	// mb - added a way to access single components of a vector-attribute directly
	// example: "color:2" corresponds to the third color component
	string number = name;
	name = extractToChar(number,':');
   const card32 i = getIndex(name);
	if( number.length() > 0)
	{
		int num;
		if( strToInt(number, num))
		{
			unsigned numBytes = attributes[i].dataFormat & 15;
			if( num < (int)attributes[i].numComponents )
				return AAT( attributes[i].getSize(), offsets[i] + numBytes*num );
		}
		throw PException(string("StructureDescriptor::getToken() - invalid attribute access! Trying to access component #") + number + ".");
	}
	else
		return getToken(i);
}

AttributeAccessToken StructureDescriptor::getToken(string name, card32 numComponents, card32 dataFormat) const {
	// get result and and the same time check for name
	AAT result = getToken(name);
	if (!providesAttribute(name, numComponents, dataFormat)) {
		throw PException("StructureDescriptor::getToken() - data format does not match.");
	}
	return result;
}											


AttributeAccessToken StructureDescriptor::getToken(card32 i) const {
   pAssert(i < offsets.size());
   pAssert(offsets.size() == attributes.size() + 1);
	return AttributeAccessToken(attributes[i].getSize(), offsets[i]);
}

string StructureDescriptor::toString()
{
	ostringstream out;
	for (card32 i=0; i<getNumAttributes(); i++) {
		SAD d = getAttribute(i);
		string fString;
		switch (d.dataFormat) {
			case SAD::DATA_FORMAT_CARD8  : fString = "card8"; break;
			case SAD::DATA_FORMAT_CARD16 : fString = "card16"; break;
			case SAD::DATA_FORMAT_CARD32 : fString = "card32"; break;
			case SAD::DATA_FORMAT_CARD64 : fString = "card64"; break;
			case SAD::DATA_FORMAT_FLOAT32: fString = "float32"; break;
			case SAD::DATA_FORMAT_FLOAT64: fString = "float64"; break;
			case SAD::DATA_FORMAT_INT8   : fString = "int8"; break;
			case SAD::DATA_FORMAT_INT16  : fString = "int16"; break;
			case SAD::DATA_FORMAT_INT32  : fString = "int32"; break;
			case SAD::DATA_FORMAT_INT64  : fString = "int64"; break;
		}

		out << d.name << " " << d.numComponents << " " << fString << "\t";
	}
	out << "\n";
	return out.str();
}

void StructureDescriptor::fromString(const string &descr)
{
   UNUSED(descr);
	notImplemented();
}


bool StructureDescriptor::equal(const Object* obj) const {


   pAssert(dynamic_cast<const StructureDescriptor*>(obj));
   StructureDescriptor* vd2 = (StructureDescriptor*)obj;

   if (getNumAttributes() != vd2->getNumAttributes()) return false;

   for( card32 i=0;i<getNumAttributes();i++ )
   {
      SAD tmp = getAttribute(i);
      SAD tmp2 = vd2->getAttribute(i);

      if (!((tmp.name==tmp2.name) &&
           (tmp.numComponents==tmp2.numComponents) &&
           (tmp.dataFormat==tmp2.dataFormat))) {
          return false;
       }
   }

   return true;

};

