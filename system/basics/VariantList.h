#ifndef VariantListH
#define VariantListH

#include "Persistent.h"
#include "Variant.h"
#include "VariableArrayClassProperty.h"



/// a list of variants
class VariantList : public Persistent {
   GEOX_CLASS(VariantList)
 private:
   std::vector<Variant> variables;
   IMPLEMENT_VAR_ARRAY_METHODS_STL(variables)

 public:
   VariantList() {};

   virtual void assign(const Object* obj);
   virtual void write(OutputObjectStream *s) const;
   virtual void read(InputObjectStream *s);

   void addVariable(Variant var);
   mpcard getNumVariables() const;
   Variant getVariable(mpcard index);
   void setVariable(mpcard index, const Variant &val);
};


/// a "template" for a variant list - only listing allowed types
class VariantTypedList : public Persistent {
   GEOX_CLASS(VariantTypedList)
 private:
   std::vector<card32> types;
   IMPLEMENT_VAR_ARRAY_METHODS_STL(types)
   std::vector<int32> flags;
   IMPLEMENT_VAR_ARRAY_METHODS_STL(flags)
   std::vector<std::string> names;
   IMPLEMENT_VAR_ARRAY_METHODS_STL(names)

 public:
   VariantTypedList() {};
   /// types and parameter names are specified separately
   ///   format for types: for example "card32, int32, Object*, int16 &"
   ///   use PTypes type or Object*, followed by & for references
   ///   separated by commas
   ///   white spaces are ignored
   /// format for names: comma separated list ("blah,foo,test,ref")
   VariantTypedList(std::string types, std::string names = "");

   virtual void assign(const Object* obj);
   virtual void write(OutputObjectStream *s) const;
   virtual void read(InputObjectStream *s);

   void addType(Variant::Type type, std::string name = "", bool reference = false);
   mpcard getNumTypes() const;
   Variant::Type getType(mpcard index);
   bool typeIsReference(mpcard index);
   std::string getName(mpcard index);
   void setType(mpcard index, const Variant::Type &val, std::string name = "", bool reference = false);
};




#endif
