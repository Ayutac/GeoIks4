//---------------------------------------------------------------------------
#ifndef SeparatorClassPropertyH
#define SeparatorClassPropertyH
//---------------------------------------------------------------------------
#include "SingleClassProperty.h"
#include "Tools.h"
//---------------------------------------------------------------------------

#ifdef _MSC_VER
   #pragma warning(disable:4100)
#endif


/**
	this is a dummy class property - it does not do anything; 
	it just causes a named separator bar to be displayed in automatically generated user interfaces
*/
class  SeparatorClassProperty : public SingleClassProperty
{
	GEOX_CLASS( SeparatorClassProperty )

 private:
	SeparatorClassProperty() {}
	
 public:
   SeparatorClassProperty(const string name);

   virtual void writeProperty(const Object* obj, pointer_diff offset, OutputObjectStream *out) const {UNUSED(obj);UNUSED(offset);UNUSED(out);}
   virtual void readProperty(Object* obj, pointer_diff offset, InputObjectStream *in) const {UNUSED(obj);UNUSED(offset);UNUSED(in);}
   virtual string toString(const Object* obj, pointer_diff offset) const {UNUSED(obj);UNUSED(offset); return getCName();}
   virtual void fromString(Object* obj, pointer_diff offset, const string str) const {UNUSED(obj);UNUSED(offset);UNUSED(str);}
   virtual const string getTypeName() const {return "SEPARATOR";}
   void assignProperty(const Object *from, Object *to, pointer_diff offset) const {UNUSED(from);UNUSED(offset);UNUSED(to);}
   virtual ClassProperty* copyClassProperty();
};

/// add a separator to the class' property list - name must be enclosed in ""-quotes
#define ADD_SEPARATOR( name ) \
	((MetaClass*)getClass())->addProperty( new SeparatorClassProperty( name ) );



#endif
