//---------------------------------------------------------------------------
#include "precompiledHeaders.h"
//---------------------------------------------------------------------------
#include "MeshImporterExperiment.h"
//---------------------------------------------------------------------------
#include "Properties.h"
#include "GeoXOutput.h"   
#include "MeshImporterSMFOBJ.h"
#include "SimpleGLMeshMaterial.h"
#include "TriangleMesh.h"
#include "ObjectViewsTable.h"
#include "ObjectListProperty.h"
#include "Raytracer.h"
#include "RaytracingMaterial.h"
#include "DynamicArrayOfStructures.hpp"
#include "StringHelper.h"
#include <QFileDialog>
#include "OpenGLCanvas3D.h"
#include "GLFixedFunctionMeshView.h"
//---------------------------------------------------------------------------

IMPLEMENT_GEOX_CLASS( MeshImporterExperiment ,0)
{
   BEGIN_CLASS_INIT( MeshImporterExperiment );
   ADD_NOARGS_METHOD(MeshImporterExperiment::importMesh)
}

MeshImporterExperiment::MeshImporterExperiment()
{
}

MeshImporterExperiment::~MeshImporterExperiment()
{
}



void MeshImporterExperiment::importMesh()
{
   QString filename = QFileDialog::getOpenFileName(NULL, "Open OBJ File...", QString(), "3D Objects (*.obj *.smf)", 0, 0 );
   if (filename != "") {
      string sfilename = qString2STLString(filename);
      MeshImporterSMFOBJ importer;
      importer.performImport(sfilename);
      TriangleMesh *mesh = importer.createTriangleMesh();
      if (mesh) {
         string name = removePathFromFilename(sfilename);
         /// add data object to workspace
         addData(mesh, name);
         /// default name for new canvas
         string canvasName = "Canvas_from_mesh_import";
         /// add canvas - name might change
         createCanvas(OpenGLCanvas3D::getClass(), canvasName);
         /// default name for new view (might change)
         string viewName = "FixedFunctionRenderer_from_mesh_import";
         /// add view to canvas
         createView(name, GLFixedFunctionMeshView::getClass(), viewName, canvasName);
      }
      /// make sure we see everything
      updateViews();
   }
}

