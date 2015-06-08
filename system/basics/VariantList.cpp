#include "precompiledHeaders.h"

#include "VariantList.h"

#include <StringHelper.h>
#include <Tools.h>
#include "VariableArrayClassProperty.h"
#include "EmbeddedObjectClassProperty.h"
#include <QString>
#include <QStringList>



IMPLEMENT_GEOX_CLASS(VariantList, 0) {
   BEGIN_CLASS_INIT(VariantList)
	ADD_VAR_ARRAY_PROP( VariantList, variables, 0, new EmbeddedObjectClassProperty( "variables" , 0, 0, Variant::getClass()) )
}


void VariantList::assign( const Object* obj )
{
   const VariantList *other = dynamic_cast<const VariantList*>(obj);
   if (!other) throw ETypeMismatch("VariantList::assign - type mismatch.");
   variables = other->variables;
}

void VariantList::write( OutputObjectStream *s ) const
{
   s->writeMaxPlatformCard(variables.size());
   for (size_t i=0; i<variables.size(); i++) {
     variables[i].write(s);
   }
}

void VariantList::read( InputObjectStream *s )
{
   mpcard num;
   s->readMaxPlatformCard(num);
   variables.resize(num);
   for (size_t i=0; i<num; i++) {
     variables[i].read(s);
   }
}

void VariantList::addVariable( Variant var )
{
   variables.push_back(var);
}

mpcard VariantList::getNumVariables() const
{
   return variables.size();
}

Variant VariantList::getVariable( mpcard index )
{
   pAssert(index < variables.size());
   return variables[index];
}

void VariantList::setVariable( mpcard index, const Variant &val )
{
   pAssert(index < variables.size());
   variables[index] = val;
}



// ----


IMPLEMENT_GEOX_CLASS(VariantTypedList, 0) {
   BEGIN_CLASS_INIT(VariantTypedList)
	ADD_CARD32_VAR_ARRAY_PROP( VariantTypedList, types, 0 )
	ADD_CARD32_VAR_ARRAY_PROP( VariantTypedList, flags, 0 )
	ADD_STRING_VAR_ARRAY_PROP( VariantTypedList, names, 0 )
}


void VariantTypedList::assign( const Object* obj )
{
   const VariantTypedList *other = dynamic_cast<const VariantTypedList*>(obj);
   if (!other) throw ETypeMismatch("VariantList::assign - type mismatch.");
   types = other->types;
   flags = other->flags;
   names = other->names;
}

void VariantTypedList::write( OutputObjectStream *s ) const
{
   s->writeMaxPlatformCard(types.size());
   for (size_t i=0; i<types.size(); i++) {
     s->write<card32>(types[i]);
     s->write<int32>(flags[i]);
     s->writeString(names[i]);
   }
}

void VariantTypedList::read( InputObjectStream *s )
{
   mpcard num;
   s->readMaxPlatformCard(num);
   types.resize(num);
   flags.resize(num);
   names.resize(num);
   for (size_t i=0; i<num; i++) {
     s->read<card32>(types[i]);
     s->read<int32>(flags[i]);
     s->readString(names[i]);
   }
}

void VariantTypedList::addType( Variant::Type type, std::string name, bool reference )
{
   types.push_back(type);
   names.push_back(name);
   flags.push_back(reference?1:0);
}

mpcard VariantTypedList::getNumTypes() const
{
   return types.size();
}

Variant::Type VariantTypedList::getType( mpcard index )
{
   pAssert(index < types.size());
   return (Variant::Type)types[index];
}

bool VariantTypedList::typeIsReference( mpcard index )
{
   pAssert(index < flags.size());
   return (flags[index] & 1) == 1;
}

std::string VariantTypedList::getName( mpcard index )
{
   pAssert(index < flags.size());
   return names[index];
}

void VariantTypedList::setType( mpcard index, const Variant::Type &val, std::string name, bool reference )
{
   pAssert(index < types.size());
   types[index] = val;
   names[index] = name;
   flags[index] = reference?1:0;
}

VariantTypedList::VariantTypedList( std::string types, std::string names )
{
   QString namesConv(names.c_str());
   QStringList namesList = namesConv.trimmed().split(",");
   QString conv(types.c_str());
   conv = conv.trimmed().toUpper();
   QStringList list = conv.split(",");
   for (int i=0; i<list.size(); i++) {
     Variant::Type type = Variant::XT_VOID;
     bool reference = false;
     QString current = list[i];
     if (current.size() == 0) throw ETypeMismatch("VariantTypeList::VariantTypeList(std::string) - parse error");
     if (current[current.size()-1] == '&') {
       reference=true;
     }
     current = current.left(current.size()-1);

     if (current == "void")                   {type = Variant::XT_VOID;}
     else if (current == "card8")             {type = Variant::XT_CARD8;}
     else if (current == "int8")              {type = Variant::XT_INT8;}
     else if (current == "card16")            {type = Variant::XT_CARD16;}
     else if (current == "int16")             {type = Variant::XT_INT16;}
     else if (current == "card32")            {type = Variant::XT_CARD32;}
     else if (current == "int32")             {type = Variant::XT_INT32;}
     else if (current == "card64")            {type = Variant::XT_CARD64;}
     else if (current == "int64")             {type = Variant::XT_INT64;}
     else if (current == "mpcard")            {type = Variant::XT_MPCARD;}
     else if (current == "mpint")             {type = Variant::XT_MPINT;}
     else if (current == "max_platform_card") {type = Variant::XT_MPCARD;}
     else if (current == "max_platform_int")  {type = Variant::XT_MPINT;}
     else if (current == "float32")           {type = Variant::XT_FLOAT32;}
     else if (current == "float64")           {type = Variant::XT_FLOAT64;}
     else if (current == "Object*")           {type = Variant::XT_OBJECT;}
     else {throw ETypeMismatch("VariantTypeList::VariantTypeList(std::string) - parse error");}

     types.push_back(type);
     flags.push_back(reference?1:0);
     if (namesList.size() > i) {
       this->names.push_back(qString2STLString(namesList[i]));
     } else {
       this->names.push_back(std::string(""));
     }
   }
}

