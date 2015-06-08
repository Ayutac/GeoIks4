#include "precompiledHeaders.h"
#include "StructureAttribDescriptor.h"
#include "StructureAttribDescriptor.hpp"
#include "StringHelper.h"

card32 StructureAttribDescriptor::getAttribElementSize() const {
   return dataFormat & 0xF;
}

card32 StructureAttribDescriptor::getSize() const {
   return getAttribElementSize()*numComponents;
}

string StructureAttribDescriptor::getDescription() const {
   string c;
   if (dataFormat == DATA_FORMAT_FLOAT32) {
      c = "f";
   } else if (dataFormat == DATA_FORMAT_CARD8) {
      c = "ub";
   } else if (dataFormat == DATA_FORMAT_INT32) {
      c = "i";
   } else if (dataFormat == DATA_FORMAT_CARD32) {
      c = "u";
   } else {
      c = "?";
   }
   //string suffix = string("(") + string(_itoa(numComponents, tmp, 10)) + c + string(")");
   string suffix = string("(") + intToStr(numComponents) + c + string(")");
   return name+suffix;
}

void StructureAttribDescriptor::write(OutputObjectStream *s) const {
   s->writeString(name);
   s->write(numComponents);
   s->write(dataFormat);
}

void StructureAttribDescriptor::read(InputObjectStream *s) {
   s->readString(name);
   s->read(numComponents);
   s->read(dataFormat);
}



