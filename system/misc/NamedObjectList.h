#ifndef NAMEDOBJECTLIST_H
#define NAMEDOBJECTLIST_H

#include "Persistent.h"
#include "ObjectListProperty.h"

/// Internal use only - ignore
class NamedObjectListEntry : public Persistent {
   GEOX_CLASS(NamedObjectListEntry)
 public:
   string name;
   Persistent *obj;
};

/// This class provides a list of objects identified by a unique name
class NamedObjectList : public Persistent {
   GEOX_CLASS(NamedObjectList)
 private:
   vector<NamedObjectListEntry*> entries;
   IMPLEMENT_OBJECT_LIST_ACCESS_STL(entries, NamedObjectListEntry)

   /// Base type of all types allowed in this container. Default: Persistent.
   const MetaClass *baseType;

 public:
   NamedObjectList();

   /// Add an object to the list. The proposed name might change to make it unique (numbers appended).
   /// This object takes the ownership of obj (will be deleted in destructor).
   ///
   /// @param obj -- object to be inserted
   /// @param name -- sugested name (will be modified if not unique)
   ///
   void addObject(Persistent *obj, string &name);

   /// Updates an existing object, or adds a new one if the name does not exist yet.
   /// This object takes the ownership of obj in any case (will be deleted in destructor).
   ///
   /// @param obj -- object to be inserted
   /// @param name -- sugested name (if in use already, the entry will be overwritten and the old object will be deleted)
   ///
   void addOrChangeObject(Persistent *obj, string &name);

   /// Deletes an object from the list.
   ///
   /// @param name -- name of the object. No effect if the name is not found.
   ///
   void deleteObject(const string &name);

   /// Retrieve an object from the list.
   ///
   /// @param name -- name of the object.
   /// \returns the pointer to the object, or NULL if the name is found.
   Persistent *getObject(const string &name);

   /// Retrieve an object from the list (const version).
   ///
   /// @param name -- name of the object.
   /// \returns the pointer to the object, or NULL if the name is found.
   const Persistent *getObject(const string &name) const;

   /// Get base type allowed in this list (default: Persistent).
   const MetaClass *getBaseType() const;
   /// Set base type allowed in this list (default: Persistent).
   void setBaseType(const MetaClass *value);

   /// Get number of entries in the list
   mpcard getSize() {return entries.size();}

   /// low-level: get entry by index (mostly for internal use - avoid in applications)
   Persistent *getEntry(mpcard index) {return entries[index]->obj;}

   /// low-level: remove entry by index
   void removeEntry(mpcard index);

   /// low-level: get entry by index (mostly for internal use - avoid in applications)
   string getName(mpcard index) {return entries[index]->name;}

   /// Delete all objects.
   void clear();

   ~NamedObjectList();
};

#endif // NAMEDOBJECTLIST_H
