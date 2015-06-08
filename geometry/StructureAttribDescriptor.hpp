//---------------------------------------------------------------------------
#ifndef StructureAttribDescriptorInlineH
#define StructureAttribDescriptorInlineH
//---------------------------------------------------------------------------
#include "StructureAttribDescriptor.h"
//---------------------------------------------------------------------------
#include <string>
//---------------------------------------------------------------------------



inline StructureAttribDescriptor::StructureAttribDescriptor(string name, card32 numComponents, card32 dataFormat) {
   this->name = name;
   this->numComponents = numComponents;
   this->dataFormat = dataFormat;
}

inline StructureAttribDescriptor::StructureAttribDescriptor(const StructureAttribDescriptor &other) {
   name = other.name;
   numComponents = other.numComponents;
   dataFormat = other.dataFormat;
}

inline void StructureAttribDescriptor::operator=(const StructureAttribDescriptor &other) {
   name = other.name;
   numComponents = other.numComponents;
   dataFormat = other.dataFormat;
}

inline bool StructureAttribDescriptor::operator==(const StructureAttribDescriptor &other) {
   return name == other.name
      && numComponents == other.numComponents
      && dataFormat == other.dataFormat;
}

inline bool operator==(const StructureAttribDescriptor &v, const StructureAttribDescriptor &other) {
   return v.name == other.name && v.numComponents == other.numComponents && v.dataFormat == other.dataFormat;
}

inline StructureAttribDescriptor mSAD(string name, card32 numComponents, card32 dataFormat) {
   return StructureAttribDescriptor(name, numComponents, dataFormat);
}


#endif
