#ifndef GEOXCANVAS_H
#define GEOXCANVAS_H

#include "Persistent.h"
#include "ObjectListProperty.h"
#include "GeoXView.h"
#include <QWidget>

class GeoXView;
class NamedObjectList;
class GeoXCanvasWidget;


/// This is an object that can display multiple views.
/// The canvas is the container that provides the context for the view objects.
/// A typical example are 3D rendering areas or 2D SVG viewers.
/// A canvas could also be an audio player, or something else quite exotic.
///
/// The canvas consists of two parts: A GeoXObject and a QT widget, which manages the ui.
/// The "makeWidget" method has to be called in order to create the second part and link the two
///
class GeoXCanvas : public Persistent {
   GEOX_ABSTRACT_CLASS(GeoXCanvas)
 private:
   bool deleting;
 protected:
   NamedObjectList *views; // of type GeoXView
   GeoXCanvasWidget *widget;

   /// override this method to define which widget to use
   virtual GeoXCanvasWidget *createWidget(QWidget *parent = 0, Qt::WindowFlags f = 0) = 0;

 public:
   GeoXCanvas();

   /// This method creates the widget that is used in the user interface
   virtual void setup(QWidget *parent = 0, Qt::WindowFlags f = 0);

   /// Adds an view of an object. Types are checked internally.
   virtual void addView(GeoXView *view, string &name);
   /// Removes the view again.
   virtual void deleteView(GeoXView *view);
   /// Removes the view again (by name).
   virtual void deleteView(string name);
   /// Repaints everything (or what else might be required for display - such as flush buffers)
   virtual void update() {}

   /// retrieve a list of all views associated with this canvas (getter method).
   NamedObjectList *getViews();

   /// retrieve the widget linked to the canvas
   GeoXCanvasWidget *getWidget() {return widget;}

   ~GeoXCanvas();
 friend class GeoXCanvasWidget;
};


#endif // GEOXCANVAS

