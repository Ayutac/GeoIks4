#ifndef DYNAMICOBJECT_H
#define DYNAMICOBJECT_H

#include "Persistent.h"
#include <map>
#include "Variant.h"

/// This class represents objects where properties (and, in the future, methods) can be added at runtime.
/// This is useful for scripting and annotations (replacement for the traditional "attachments").
/// The goal is flexibility at run time and ease of use; performance is not an objective.
/// Do not use in critical inner loops :-)
///
class DynamicObject : public Persistent {
   typedef map<string, Variant> PropsMap;
   typedef PropsMap::iterator PropsIterator;
   typedef PropsMap::const_iterator PropsConstIterator;
   PropsMap properties;
 public:
   DynamicObject();

   /// Set a property. Object properties are owned (deleted by this object)
   void setProperty(const string &name, Variant prop);
   /// Get a property. An exception is raised if the property does not exist.
   Variant getProperty(const string &name);
   /// Remove a property. An exception is raised if the property does not exist.
   void deleteProperty(const string &name);

   virtual void assign(const Object* obj);
   virtual void write(OutputObjectStream *s) const;
   virtual void read(InputObjectStream *s);

   ~DynamicObject();
};

#endif // DYNAMICOBJECT_H
