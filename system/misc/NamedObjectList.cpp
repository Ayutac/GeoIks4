#include "NamedObjectList.h"
#include "StringHelper.h"
#include "ClassTypeClassProperty.h"
#include "StringClassProperty.h"
#include "ObjectClassProperty.h"

// helper class - internal
IMPLEMENT_GEOX_CLASS(NamedObjectListEntry, 0)
{
   BEGIN_CLASS_INIT(NamedObjectListEntry)
   ADD_STRING_PROP(name, 0)
   ADD_OBJECT_PROP(obj, 0, Persistent::getClass(), true)
}


IMPLEMENT_GEOX_CLASS(NamedObjectList, 0)
{
   BEGIN_CLASS_INIT(NamedObjectList);
	ADD_OBJECT_LIST_PROP(NamedObjectList, entries, 0, Persistent::getClass())
   ADD_METACLASS_PROP(baseType, 0, Persistent::getClass())
}




NamedObjectList::NamedObjectList()
{
   baseType = Persistent::getClass();
}

void NamedObjectList::addObject(Persistent *obj, string &name)
{
    int suffix = 0;
   string actualName = name;
   while (getObject(actualName)) {
      suffix++;
      actualName = name + intToStr(suffix);
   }
   NamedObjectListEntry *e = new NamedObjectListEntry;
   e->name = actualName;
   e->obj = obj;
   entries.push_back(e);
   name = actualName;
}

void NamedObjectList::addOrChangeObject(Persistent *obj, string &name)
{
   for (vector<NamedObjectListEntry*>::iterator pos = entries.begin();
        pos != entries.end(); ++pos) {
      if ((*pos)->name == name) {
         delete (*pos)->obj;
         (*pos)->obj = obj;
         return;
      }
   }
   addObject(obj, name);
}


void NamedObjectList::deleteObject(const string &name)
{
   for (vector<NamedObjectListEntry*>::iterator pos = entries.begin();
        pos != entries.end(); ++pos) {
      if ((*pos)->name == name) {
         delete (*pos)->obj;
         delete (*pos);
         entries.erase(pos);
         return;
      }
   }

}

Persistent *NamedObjectList::getObject(const string &name)
{
   for (size_t i=0; i<entries.size(); i++) {
      if (entries[i]->name == name) return entries[i]->obj;
   }
   return NULL;
}

/// same as above
const Persistent *NamedObjectList::getObject(const string &name) const
{
   return const_cast<NamedObjectList*>(this)->getObject(name);
}

void NamedObjectList::clear()
{
   for (size_t i=0; i<entries.size(); i++) {
      delete entries[i]->obj;
      delete entries[i];
   }
   entries.clear();
}

const MetaClass *NamedObjectList::getBaseType() const
{
    return baseType;
}

void NamedObjectList::setBaseType(const MetaClass *value)
{
   baseType = value;
}

void NamedObjectList::removeEntry(mpcard index)
{
   if (index >= entries.size()) {
      throw ERangeCheck("NamedObjectList::removeEntry -- index out of range.");
   }
   delete entries[index];
   entries.erase(entries.begin()+index);
}

NamedObjectList::~NamedObjectList()
{
   clear();
}

