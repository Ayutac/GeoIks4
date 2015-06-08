#include "precompiledHeaders.h"

#include "Variant.h"

#include "GeoXOutput.h"
#include "NumericalClassProperty.h"
#include "ObjectStreams.h"
#include "ObjectClassProperty.h"
#include "StringClassProperty.h"
#include "Object.h"
#include "Exceptions.h"


static std::string descrType(card32 v) {
   switch (v) {
     case Variant::XT_VOID    : return "XT_VOID";
     case Variant::XT_CARD8   : return "XT_CARD8";
     case Variant::XT_INT8    : return "XT_INT8";
     case Variant::XT_CARD16  : return "XT_CARD16";
     case Variant::XT_INT16   : return "XT_INT16";
     case Variant::XT_CARD32  : return "XT_CARD32";
     case Variant::XT_INT32   : return "XT_INT32";
     case Variant::XT_CARD64  : return "XT_CARD64";
     case Variant::XT_INT64   : return "XT_INT64";
     case Variant::XT_MPCARD  : return "XT_MPCARD";
     case Variant::XT_MPINT   : return "XT_MPINT";
     case Variant::XT_FLOAT32 : return "XT_FLOAT32";
     case Variant::XT_FLOAT64 : return "XT_FLOAT64";
     case Variant::XT_OBJECT  : return "XT_OBJECT";
     case Variant::XT_STRING  : return "XT_STRING";
     default: return "unkown";
   }
}


// ------------------------------------------------------------------------------
IMPLEMENT_GEOX_CLASS(Variant, 0)
{
   BEGIN_CLASS_INIT(Variant)
   ADD_CARD32_PROP_UPDATE_DESCR(type, 0, NULL, descrType, 32)
   ADD_INT64_PROP(intVal, 0)
   ADD_FLOAT64_PROP(floatVal, 0)
   ADD_OBJECT_PROP(obj, 0, Object::getClass(), false);
   ADD_STRING_PROP(str, 0);
}


// ------------------------------------------------------------------------------
void Variant::assign( const Object* obj )
{
   const Variant *other = dynamic_cast<const Variant*>(obj);
   if (other == NULL) throw ETypeMismatch("Variant::assign - incompatible type.");
   this->type = other->type;
   this->intVal = other->intVal;
   this->floatVal = other->floatVal;
   this->obj = other->obj;
   this->str = other->str;
}

// ------------------------------------------------------------------------------
void Variant::write( OutputObjectStream *s ) const
{
   s->write(type);
   if (type > XT_VOID || type <= XT_MAX_TYPE_INDEX)
   {
     if (type <= XT_MPINT) {
       s->write<int64>(intVal);
     } else if (type <= XT_FLOAT64) {
       s->write<float64>(floatVal);
     } else if (type == XT_OBJECT) {
       Persistent *per = dynamic_cast<Persistent*>(obj);
       if ((Object*)per != obj) error("Variant::write - trying to write non-persistent object, exchanged with NULL-pointer.");
       s->writeObject(per);
     } else if (type == XT_STRING) {
       s->writeString(str);
     }else {
       pAssert(false);
     }
   }
}

// ------------------------------------------------------------------------------
void Variant::read( InputObjectStream *s )
{
   s->read(type);
   if (type > XT_VOID || type <= XT_MAX_TYPE_INDEX)
   {
     if (type <= XT_MPINT) {
       s->read<int64>(intVal);
     } else if (type <= XT_FLOAT64) {
       s->read<float64>(floatVal);
     } else if (type == XT_OBJECT) {
       Persistent *per = NULL;
       s->readObject(per);
       obj = per;
     } else if (type == XT_STRING) {
       s->readString(str);
     } else {
       pAssert(false);
     }
   }
}

