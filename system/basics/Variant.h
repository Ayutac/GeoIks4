#ifndef VariantH
#define VariantH

#include "PTypes.h"
#include "Exceptions.h"
#include "Persistent.h"


/// this class can take a value of all standard GeoX types
class Variant : public Persistent {
   GEOX_CLASS(Variant)
 public:
   enum Type
   {
      XT_VOID    = 0,
      XT_CARD8   = 1, XT_INT8   = 2,
      XT_CARD16  = 3, XT_INT16  = 4,
      XT_CARD32  = 5, XT_INT32  = 6,
      XT_CARD64  = 7, XT_INT64  = 8,
      XT_MPCARD  = 9, XT_MPINT  = 10,
      XT_FLOAT32 = 11, XT_FLOAT64 = 12,
      XT_OBJECT  = 13,
      XT_STRING  = 14, XT_MAX_TYPE_INDEX = 14
   };
 private:
   Type type;
   int64 intVal;
   float64 floatVal;
   Object *obj;
   std::string str;

   inline void init() {
      type = XT_VOID;
      intVal = 0;
      floatVal = 0;
      obj = NULL;
   }


 public:
   Variant() {init();}

   virtual ~Variant() {}

   void assign(const Object* obj);

   void write(OutputObjectStream *s) const;
   void read(InputObjectStream *s);

   inline Type getType() const { return type; }
   inline void setVoid() { type = XT_VOID; }

   /// use this method to read any scalar type
   template<typename ScalarType>
   ScalarType getScalar() const
   {
      if (type < XT_CARD8 ||  type > XT_FLOAT64)
         throw EInvalidState("Variant access via getScalarValue to non scalar type.");

      if (type <= XT_MPINT)
         return (ScalarType)(intVal);

      return (ScalarType)(floatVal);
   }

   /// use this method to set a scalar type
   template<typename ScalarType>
   void setScalar(Type newType, const ScalarType &value)
   {
      if (newType < XT_CARD8 ||  newType > XT_FLOAT64)
         throw EInvalidState("Variant write via setScalarValue and non scalar type.");

      intVal = (int64)(value);
      floatVal = (float64)(value);

      type = newType;
   }

   /// If the variant contains an object, then retrieve it
   Object *getObject() const
   {
      if (type != XT_OBJECT)
         throw EInvalidState("Variant access via getObjectValue to non-object type.");
      return obj;
   }

   /// set object type and object pointer
   inline void setObject(Object *obj)
   {
      type = XT_OBJECT;
      this->obj = obj;
   }

   /// If the variant contains a std::string, then return std::string
   inline const std::string& getString() const
   {
      if (type != XT_STRING)
         throw EInvalidState("Variant access via getString to non-object type.");
      return str;
   }
   inline void setString(const std::string& str)
   {
      type = XT_STRING;
      this->str = str;
   }

   explicit Variant(const float32& val)
   {
      init(); setScalar(XT_FLOAT32, val);
   }
   explicit Variant(const float64& val)
   {
      init(); setScalar(XT_FLOAT64, val);
   }
   explicit Variant(const card8& val)
   {
      init(); setScalar(XT_CARD8, val);
   }
   explicit Variant(const card16& val)
   {
      init(); setScalar(XT_CARD16, val);
   }
   explicit Variant(const card32& val)
   {
      init(); setScalar(XT_CARD32, val);
   }
   explicit Variant(const card64& val)
   {
      init(); setScalar(XT_CARD64, val);
   }
   explicit Variant(const int8& val)
   {
      init(); setScalar(XT_INT8, val);
   }
   explicit Variant(const int16& val)
   {
      init(); setScalar(XT_INT16, val);
   }
   explicit Variant(const int32& val)
   {
      init(); setScalar(XT_INT32, val);
   }
   explicit Variant(const int64& val)
   {
      init(); setScalar(XT_INT64, val);
   }
   explicit Variant(Object* obj)
   {
      init(); setObject(obj);
   }
   explicit Variant(const string &str)
   {
      init(); setString(str);
   }

};





#endif
