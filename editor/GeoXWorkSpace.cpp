#include "precompiledHeaders.h"
#include "GeoXWorkSpace.h"
#include "ObjectClassProperty.h"
#include "NamedObjectList.h"
#include "GeoXData.h"

IMPLEMENT_GEOX_CLASS( GeoXWorkSpace, 0 )
{
   BEGIN_CLASS_INIT( GeoXWorkSpace );
   ADD_OBJECT_PROP( data, 0, Persistent::getClass(), true )
}

GeoXWorkSpace::GeoXWorkSpace()
{
   data = new NamedObjectList();
   data->setBaseType(GeoXData::getClass());
   canvases = new NamedObjectList();
   canvases->setBaseType(GeoXCanvas::getClass());
}

NamedObjectList *GeoXWorkSpace::getData() {
   return data;
}

void GeoXWorkSpace::close()
{
   setNewData(NULL);
}

void GeoXWorkSpace::setNewData(NamedObjectList *data)
{
   if (data != this->data) {
      canvases->clear();
      delete data;
      data = NULL;
   }
   this->data = data;
}

/// clean up - being careful to do it in the right order!
GeoXWorkSpace::~GeoXWorkSpace()
{
   canvases->clear();
   delete data;
}

void GeoXWorkSpace::update()
{
   mpcard n = canvases->getSize();
   for (mpcard i=0; i<n; i++) {
      ((GeoXCanvas*)(canvases->getEntry(i)))->update();
   }
}

