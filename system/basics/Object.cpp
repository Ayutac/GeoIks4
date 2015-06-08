//---------------------------------------------------------------------------
#include "precompiledHeaders.h"
//---------------------------------------------------------------------------
#include "Object.h"
//---------------------------------------------------------------------------
#include "Tools.h"
#include "PAssert.h"
#include "CopyObjectProperties.h"
//---------------------------------------------------------------------------
#include "MemberBrowser.h"
//---------------------------------------------------------------------------


MetaClass* Object::objectMetaClass = NULL;

const MetaClass* Object::getInstanceClass() const {
   return getClass();
}

const MetaClass* Object::getClass() {
   if (objectMetaClass==NULL) {
      objectMetaClass = new MetaClass("Object", "Base Class for all reflective classes.",
                                      NULL, sizeof(Object));
   }
   return objectMetaClass;
}

void Object::init(const MetaClass *parentClass) {
   ((MetaClass*)getClass())->init( parentClass);
   objectMetaClass->getGUIDescriptor()->addClassEditor(MemberBrowser::getClass());

}

void Object::shutdown() {
   delete objectMetaClass;
}


// ----



void Object::assign(const Object* obj) {
   if (obj->getInstanceClass() != this->getInstanceClass()) {
      throw ECannotAssign("Object::assign does not implement assignment between different types.");
   } else {
      // memcpy(this, obj, getInstanceClass()->getInstanceSize());
      copyObjectProperties(obj, this);
   }
}

Object* Object::copy() const {
   Object* result = getInstanceClass()->newInstance();
	result->assign(this);
   return result;
}

bool Object::equal(const Object* obj) const {
   if (obj == NULL) return false;

   const MetaClass *thisMetaClass = getInstanceClass();
   const MetaClass *objMetaClass = obj->getInstanceClass();
   if (thisMetaClass != objMetaClass) return false;

   card32 instanceSize = thisMetaClass->getInstanceSize();
   char *thisPos = (char*)this;
   char *objPos = (char*)obj;

   while (instanceSize > 0) {
      if (*thisPos != *objPos) return false;
      ++thisPos;
      ++objPos;
      --instanceSize;
   }
   return true;
}


