#ifndef GEOXWORKSPACE_H
#define GEOXWORKSPACE_H

#include "Persistent.h"
#include "GeoXCanvas.h"
#include "GeoXView.h"
#include "ObjectListProperty.h"


class NamedObjectList;


/// This class representes a whole work space - an object is edited, and multiple tools, views, etc. are provided to edit the object.
///
/// The member methods mimic the typical operations in the "File" menue of the application.
/// Remember that read and write are inherited via Persistent - so opening and saving is available, too.
///
class GeoXWorkSpace : public Persistent {
   GEOX_CLASS(GeoXWorkSpace)
 private:
   /// the object that is edited
   NamedObjectList *data;
   /// The canvases on which its views are displayed.
   /// The type of the list is GeoXCanvas* - templating ommitted for simplicity.
   NamedObjectList *canvases;

 public:
   /// creates an empty work space (no object)
   GeoXWorkSpace();

   /// Return the main object that is being edited.
   NamedObjectList *getData();

   /// Delete the current object being edited, clean up all the views / canvases.
   virtual void close();

   /// Sets a new object to be edited. setNewObject(NULL) is equivalent to closeObject().
   /// Setting the same object again has no effect. Otherwise, views are deleted.
   virtual void setNewData(NamedObjectList *data);

   /// Deletes the object and all canvases (along with their views).
   ~GeoXWorkSpace();


   // --- Technical tools; usually, you can ignore these.

   /// (technical) retrieve the pointer to the list of canvases
   NamedObjectList *getCanvases() {return canvases;}

   /// updates all the views (usually called via editor)
   virtual void update();
};


#endif // GEOXWORKSPACE_H
