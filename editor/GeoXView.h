#ifndef GEOXVIEW_H
#define GEOXVIEW_H

#include "Persistent.h"
#include <string>

class GeoXCanvas;
class GeoXWorkSpace;

/// This class provides a (typically) visual representation of an arbitrary GeoX object (called "data" here;
/// the terminology in the MVC paradigm would be "model").
/// In order to avoid dangeling reference problems, data objects are referenced by _names_ rather than
/// direct pointers.
/// This means, that every view object is aware of a _context_ (GeoXWorkspace) in which the viewed data lives.
///
/// Updates to the view are always initiated via its canvas. Each type of canvas defines its own protocoll for refreshing views
/// when the model ("data") changed.
///
/// Typically, two different models will be used:
/// - Immediate mode Canvas: The canvas will call some kind of drawing method each time the view has to be refreshed.
/// - Retained mode Canvas: The canvas will call some kind of update method, along with a storage identifier
///                         in order to update the content of the buffer it stores to represent the data object.
///
/// Other modes of operation might be suitable, too; therefore, this protocol is not prescribed in the base class.
///
class GeoXView : public Persistent {
   GEOX_CLASS(GeoXView)
 private:
   GeoXWorkSpace *context;
   GeoXCanvas *container;
   string data;

 public:
   GeoXView();
   /// This method binds the view to the object to be viewed. It must be called first thing after the constructor.
   /// Re-setup is not supported (exception).
   virtual void setup(GeoXWorkSpace *context, GeoXCanvas *container, string data);

   // --- property access (getters/setters)
   GeoXWorkSpace *getContext() const    {return context;}
   GeoXCanvas *getContainer() const     {return container;}
   void setContainer(GeoXCanvas *value) {container = value;}
   string getData() const               {return data;}
   void setData(const string &value)    {data = value;}

	virtual bool canHandleCanvas(const MetaClass *canvasType) = 0;
	virtual bool canHandleData(const MetaClass *dataType) = 0;
	virtual const MetaClass *preferredCanvasType() = 0;
};


#endif // GEOXVIEW



