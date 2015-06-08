//---------------------------------------------------------------------------
#ifndef MeshImporterExperimentH
#define MeshImporterExperimentH
//---------------------------------------------------------------------------
#include "Experiment.h"
//---------------------------------------------------------------------------



///
/// This is an example experiment that demonstrates importing and rendering triangle meshes.
///
class MeshImporterExperiment : public Experiment {
   GEOX_CLASS(MeshImporterExperiment)
 public:
   MeshImporterExperiment();

   void importMesh();

   ~MeshImporterExperiment();
};


#endif                                         
