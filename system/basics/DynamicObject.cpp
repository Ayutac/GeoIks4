#include "precompiledHeaders.h"
#include "DynamicObject.h"
#include "BinaryObjectStreams.h"


DynamicObject::DynamicObject() {}

void DynamicObject::setProperty(const string &name, Variant prop)
{
   PropsIterator pos = properties.find(name);
   if (pos != properties.end()) {
      Variant val = pos->second;
      if (val.getType() == Variant::XT_OBJECT) {
         delete val.getObject();
      }
      properties.erase(pos);
   }
   properties.insert(pair<string,Variant>(name, prop));
}

Variant DynamicObject::getProperty(const string &name)
{
   PropsIterator pos = properties.find(name);
   if (pos == properties.end()) {
      throw ERangeCheck("DynamicObject::setProperty - property not found.");
   } else {
      return pos->second;
   }
}

void DynamicObject::deleteProperty(const string &name)
{
   PropsIterator pos = properties.find(name);
   if (pos != properties.end()) {
      Variant val = pos->second;
      if (val.getType() == Variant::XT_OBJECT) {
         delete val.getObject();
      }
      properties.erase(pos);
   } else {
      throw ERangeCheck("DynamicObject::setProperty - property not found.");
   }
}

void DynamicObject::assign(const Object *obj)
{
   const DynamicObject *other = dynamic_cast<const DynamicObject*>(obj);
   if (other) {
      properties = other->properties;
   } else {
      throw ETypeMismatch("DynamicObject::assign - other object is not a DynamicObject.");
   }
}

void DynamicObject::write(OutputObjectStream *s) const
{
   mpcard n = properties.size();
   s->writeMaxPlatformCard(n);
   for (PropsConstIterator pos = properties.begin(); pos != properties.end(); ++pos) {
      s->writeString(pos->first);
      pos->second.write(s);
   }
}

void DynamicObject::read(InputObjectStream *s)
{
   mpcard n;
   s->readMaxPlatformCard(n);
   for (mpcard i=0; i<n; i++) {
      string name;
      s->readString(name);
      Variant val;
      val.read(s);
      properties.insert(pair<string, Variant>(name, val));
   }
}

DynamicObject::~DynamicObject()
{
   for (PropsIterator pos=properties.begin(); pos != properties.end(); ++pos) {
      Variant val = pos->second;
      if (val.getType() == Variant::XT_OBJECT) {
         delete val.getObject();
      }
   }
}

