//---------------------------------------------------------------------------
#ifndef AttributeAccessTokenH
#define AttributeAccessTokenH
//---------------------------------------------------------------------------
#include <string>
#include "PTypes.h"
//---------------------------------------------------------------------------


class DynamicArrayOfStructures;
class StructureAttribDescriptor;
class StructureDescriptor;


/// offset to a channel within a dynamic array of structures
class  AttributeAccessToken {
 private:                                                     
   card32 offset;
   #ifdef _DEBUG
   card32 size;
   #endif

 protected:
   inline AttributeAccessToken(card32 size, card32 offset);

 public:
   inline AttributeAccessToken();

   inline bool operator==(const AttributeAccessToken &aat) const;
   inline bool operator!=(const AttributeAccessToken &aat) const;

   inline card32 getOffset() const;

 friend class DynamicArrayOfStructures;
 friend class StructureAttribDescriptor;
 friend class StructureDescriptor;
};


typedef AttributeAccessToken AAT;

/// undefined AAT (e.g. to mark non-present attribute channels)
extern const AttributeAccessToken NULL_AAT;



#endif
