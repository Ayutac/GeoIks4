//---------------------------------------------------------------------------
#ifndef AttributeAccessTokenInlineH
#define AttributeAccessTokenInlineH
//---------------------------------------------------------------------------
#include "AttributeAccessToken.h"
#include "Tools.h"
//---------------------------------------------------------------------------


AttributeAccessToken::AttributeAccessToken(card32 size, card32 offset) {
   #ifdef _DEBUG
   this->size = size;
	#else
	UNUSED(size);
   #endif
   this->offset = offset;
}

AttributeAccessToken::AttributeAccessToken() {
   #ifdef _DEBUG
   size = 0xFFFFFFFF; // invalid ("NULL_AAT") token by default
   #endif
   offset = 0xFFFFFFFF; // invalid ("NULL_AAT") token by default
}

bool AttributeAccessToken::operator==(const AttributeAccessToken &aat) const {
   bool result = (offset == aat.offset);
   #ifdef _DEBUG
      result &= (size == aat.size);
   #endif
   return result;
}

bool AttributeAccessToken::operator!=(const AttributeAccessToken &aat) const {
   bool result = (offset != aat.offset);
   return result;
}

card32 AttributeAccessToken::getOffset() const {return offset;}


#endif
