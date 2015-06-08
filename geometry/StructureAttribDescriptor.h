//---------------------------------------------------------------------------
#ifndef StructureAttribDescriptorH
#define StructureAttribDescriptorH
//---------------------------------------------------------------------------
#include "Persistent.h"
#include "PAssert.h"
#include "LinearAlgebra.h"
#include "DynamicLinearAlgebra.h"
#include "DynamicLinearAlgebra.hpp"
//---------------------------------------------------------------------------
#include <string>


/// descriptor of a single channel within an array of structures
class  StructureAttribDescriptor {
 public:
   string name;
   card32 numComponents;
   card32 dataFormat;

   // scheme -- do not use in applications; this might change!
   // first 4 bits = number of bytes
   // 16 = floating point or integer?
   // 32 = unsigned or signed int? (set = signed)
   static const card32 DATA_FORMAT_CARD8   =      1;
   static const card32 DATA_FORMAT_CARD16  =      2;
   static const card32 DATA_FORMAT_CARD32  =      4;
   static const card32 DATA_FORMAT_CARD64  =      8;
   static const card32 DATA_FORMAT_FLOAT32 = 16 | 4;
   static const card32 DATA_FORMAT_FLOAT64 = 16 | 8;
   static const card32 DATA_FORMAT_INT8    = 32 | 1;
   static const card32 DATA_FORMAT_INT16   = 32 | 2;
   static const card32 DATA_FORMAT_INT32   = 32 | 4;
   static const card32 DATA_FORMAT_INT64   = 32 | 8;

 public:
   inline StructureAttribDescriptor();

 protected:
   card32 getAttribElementSize() const;

 public:
   inline StructureAttribDescriptor(const StructureAttribDescriptor &other);
   inline StructureAttribDescriptor(string name, card32 numComponents, card32 dataFormat);

   inline void operator=(const StructureAttribDescriptor &other);
   inline bool operator==(const StructureAttribDescriptor &other);

   card32 getSize() const;
   string getDescription() const;
   void write(OutputObjectStream *s) const;
   void read(InputObjectStream *s);

};

inline bool operator==(const StructureAttribDescriptor &v, const StructureAttribDescriptor &other);
inline StructureAttribDescriptor mSAD(string name, card32 numComponents, card32 dataFormat);


typedef StructureAttribDescriptor SAD;


#endif