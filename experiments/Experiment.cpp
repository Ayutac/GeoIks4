//---------------------------------------------------------------------------
#include "precompiledHeaders.h"
//---------------------------------------------------------------------------
#include "Experiment.h"
#include "GeoXWorkSpace.h"
#include "NamedObjectList.h"
#include "GeoXData.h"
#include "ExperimentContainer.h"
//---------------------------------------------------------------------------
#include "Properties.h"
#include "LogoCanvas.h"
#include "GeoXOutput.h"
//---------------------------------------------------------------------------



IMPLEMENT_GEOX_CLASS( Experiment, 0 )
{
   BEGIN_CLASS_INIT( Experiment );
}



void Experiment::createNewData(const MetaClass *type, string &name)
{
   if (!workSpace) {
      error("No workspace bound to experiment.");
      return;
   }
   Object *newObj = type->newInstance();
   GeoXData *casted = dynamic_cast<GeoXData*>(newObj);
   if (casted) {
      workSpace->getData()->addObject(casted, name);
   } else {
      delete newObj;
      error("Experiment::createNewData() -- the type provided is not a GeoXData object. Operation not performed.");
   }
}

void Experiment::addData(GeoXData *data, string &name)
{
   if (!workSpace) {
      error("No workspace bound to experiment.");
      return;
   }
   workSpace->getData()->addObject(data, name);
}

GeoXData *Experiment::getData(const string &name)
{
   if (!workSpace) {
      error("No workspace bound to experiment.");
      return NULL;
   }
   return dynamic_cast<GeoXData*>(workSpace->getData()->getObject(name));
}

void Experiment::deleteData(const string &name)
{
   if (!workSpace) {
      error("No workspace bound to experiment.");
      return;
   }
   workSpace->getData()->deleteObject(name);
}

void Experiment::createCanvas(const MetaClass *type, string &name)
{
   if (!workSpace) {
      error("No workspace bound to experiment.");
      return;
   }
   if (!container) {
      error("No container (object editor) set.");
      return;
   }

   Object *newObj = type->newInstance();
   GeoXCanvas *canvas = dynamic_cast<GeoXCanvas*>(newObj);
   if (!canvas) {
      error("Experiment::createCanvas() - given type does not describe a GeoXCanvas. Aborted.");
      delete newObj;
      return;
   }
   container->addCanvas(canvas, name, true);
}

void Experiment::deleteCanvas(const string &name)
{
   if (!workSpace) {
      error("No workspace bound to experiment.");
      return;
   }
   workSpace->getCanvases()->deleteObject(name);
}

bool Experiment::createView(const string &dataName, const MetaClass *viewType, string &viewName, const string canvasName)
{
   if (!workSpace) {
      error("No workspace bound to experiment.");
      viewName ="";
      return false;
   }
   GeoXData *data = (GeoXData*)workSpace->getData()->getObject(dataName);
   if (!data) {
      error("Experiment::createView() - The specified data object was not found.");
      viewName ="";
      return false;
   }
   GeoXCanvas *canvas = (GeoXCanvas*)workSpace->getCanvases()->getObject(canvasName);
   if (!canvas) {
      error("Experiment::createView() - canvasName not found.");
      viewName ="";
      return false;
   }
   Object *newObj = viewType->newInstance();
   GeoXView *view = dynamic_cast<GeoXView*>(newObj);
   if (!view) {
      error("Experiment::createView() - given type does not describe a GeoXView. Aborted.");
      delete newObj;
      viewName ="";
      return false;
   }
   if (!view->canHandleData(data->getInstanceClass())) {
      error("Experiment::createView() - the view type cannot handle the specified data object.");
      delete view;
      viewName ="";
      return false;
   }
   if (!view->canHandleCanvas(canvas->getInstanceClass())) {
      error("Experiment::createView() - the view type cannot be inserted in the specified canvas according to its type.");
      delete view;
      viewName ="";
      return false;
   }

   view->setup(workSpace, canvas, dataName);
   canvas->addView(view, viewName);
   return true;
}

void Experiment::deleteView(const string canvasName, const string &viewName)
{
   if (!workSpace) {
      error("No workspace bound to experiment.");
      return;
   }
   GeoXCanvas *canvas = (GeoXCanvas*)workSpace->getCanvases()->getObject(canvasName);
   if (!canvas) {
      error("Experiment::createView() - canvasName not found.");
      return;
   }
   canvas->deleteView(viewName);
}

void Experiment::updateViews()
{
   if (!workSpace) {
      error("No workspace bound to experiment.");
      return;
   }
   container->update();
}

void Experiment::updateViewsLazy()
{
   if (!workSpace) {
      error("No workspace bound to experiment.");
      return;
   }
   container->updateLazy();
}

Experiment::Experiment()
{
   container = NULL;
   workSpace = NULL;
}

void Experiment::bindExperiment(GeoXWorkSpace *workSpace, ExperimentContainer *container)
{
   this->workSpace = workSpace;
   this->container = container;
}
