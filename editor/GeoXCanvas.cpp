#include "precompiledHeaders.h"
#include "GeoXCanvas.h"
#include "GeoXView.h"
#include "NamedObjectList.h"
#include "GeoXCanvasWidget.h"
#include "ObjectClassProperty.h"
#include "GeoXOutput.h"

IMPLEMENT_GEOX_ABSTRACT_CLASS(GeoXCanvas, 0) {
   BEGIN_CLASS_INIT(GeoXCanvas)
   ADD_OBJECT_PROP(views, 0, NamedObjectList::getClass(), true)
   SET_USER_EDIT(false)
   SET_EDIT_READONLY(true)
   SET_STREAM_PROPERTY(false)
}


GeoXCanvas::GeoXCanvas()
{
   deleting = false;
   views = new NamedObjectList();
   views->setBaseType(GeoXView::getClass());
}

void GeoXCanvas::setup(QWidget *parent, Qt::WindowFlags f)
{
   widget = createWidget(parent, f);
}

void GeoXCanvas::addView(GeoXView *view, string &name)
{
   for (mpcard i=0; i<views->getSize(); i++) {
      if (views->getEntry(i) == view) {
         error("GeoXCanvas::addView -- view added twice;");
         return;
      }
   }
   views->addObject(view, name);
}

void GeoXCanvas::deleteView(GeoXView *view)
{
   for (mpcard i=0; i<views->getSize(); i++) {
      if (views->getEntry(i) == view) {
         views->removeEntry(i);
         return;
      }
   }
   throw ERangeCheck("GeoXCanvas::addView -- view not found.");
}

void GeoXCanvas::deleteView(string name)
{
   views->deleteObject(name);
}

NamedObjectList *GeoXCanvas::getViews()
{
   return views;
}

GeoXCanvas::~GeoXCanvas()
{
   deleting = true;
   delete views;
   if (!widget->deleting) delete widget;
}
