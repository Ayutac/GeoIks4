#ifndef GEOXDATA_H
#define GEOXDATA_H

#include "Persistent.h"

class MetaClass;

/// This class groups all objects that can be visualized within the GeoXWorkspace environment
///
class GeoXData : public Persistent {
   GEOX_ABSTRACT_CLASS(GeoXData)
 public:
   /// This method speficies the default viewer that should be used for displaying this type of data.
   virtual const MetaClass *getDefaultViewer() = 0;
   /// This method speficies the canvas that the default viewer requires.
   virtual const MetaClass *getDefaultViewerDefaultCanvas() = 0;
};

#endif // GEOXDATA_H
