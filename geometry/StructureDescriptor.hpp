//---------------------------------------------------------------------------
#ifndef StructureDescriptorInlineH
#define StructureDescriptorInlineH
//---------------------------------------------------------------------------
#include "StructureDescriptor.h"
//---------------------------------------------------------------------------
#include <string>
#include "AttributeAccessToken.hpp"
#include "StructureAttribDescriptor.hpp"
//---------------------------------------------------------------------------

inline StructureAttribDescriptor::StructureAttribDescriptor() {
	this->name = ""; this->numComponents = 0;
	this->dataFormat = 0;
}


inline StructureAttribDescriptor StructureDescriptor::getAttribute(card32 i) const {
   return attributes[i];
}

inline StructureAttribDescriptor StructureDescriptor::getAttribute(string name) const {
   return getAttribute(getIndex(name));
}

inline card32 StructureDescriptor::getAttributeOffset(card32 i) const {
   return getToken(i).getOffset();
}

inline mpcard StructureDescriptor::computeOverallDimension() const {
   mpcard result = 0;
   const size_t n = attributes.size();
   for (size_t i=0; i<n; i++) {
      result += attributes[i].numComponents;
   }
   return result;
}


#endif
