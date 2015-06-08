#ifndef EXPERIMENTCONTAINER_H
#define EXPERIMENTCONTAINER_H

class MDICanvasContainer;

/// This interface class defines a protocol how experiments can interact with their environment.
/// Usually, there is no need to worry about this - this is handled internally by the GeoX framework.
///
class ExperimentContainer {
 public:
   /// redraw all views, do it immediately
   virtual void update() = 0;

   /// redraw all views, do it after gathering requests for a few milliseconds (or longer if the event loop does not run yet)
   virtual void updateLazy() = 0;

   /// creates a new container for a canvas (at this point, assuming that these are always MDI windows for simplicity)
   /// The name of the canvas needs to be specified, too.
   /// It will be added to the work space under this name.
   virtual MDICanvasContainer *createNewCanvasContainer(GeoXCanvas *canvas, string &name, bool addToWorkSpace = false) = 0;

   /// integrates an existing canvas into the environment. Adds to the environment's workspace, if desired.
   virtual void addCanvas(GeoXCanvas *canvas, string &name, bool addToWorkSpace) = 0;


   /// makes sure that the interface is used on proper identity-type objects (technical requirement)
   virtual ~ExperimentContainer() {}
};


#endif // EXPERIMENTCONTAINER_H

