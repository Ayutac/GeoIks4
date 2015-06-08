//---------------------------------------------------------------------------
#include "precompiledHeaders.h"
//---------------------------------------------------------------------------
#include "InitGeoX.h"
//---------------------------------------------------------------------------
#include "Camera.h"
#include "ViewFrustum.h"
#include "CameraController.h"
#include "ClassCreator.h"
#include "ClassEditor.h"
#include "ExaminerCameraController.h"
#include "Persistent.h"
#include "PropertyBrowser.h"
#include "MethodBrowser.h"
#include "MemberBrowser.h"
#include "PropertyEditor.h"
#include "WalkthroughCameraController.h"
#include "StreamClassTable.h"
#include "LegacyClassNameTranslation.h"

#include "BooleanPropertyEditor.h"
#include "ObjectPropertyEditor.h"
#include "FixedArrayPropertyEditor.h"
#include "ClassTypeClassProperty.h"
#include "ClassTypeClassPropertyEditorWidget.h"

#include "Experiment.h"
#include "ExampleExperiment.h"
#include "MeshImporterExperiment.h"
#include "SimpleGLMeshMaterial.h"
#include "DynamicArrayOfStructures.h"
#include "DynamicArrayOfStructures.hpp"

#include "StructureDescriptor.h"
#include "DynamicArrayOfStructures2D.h"
#include "DynamicArrayOfStructures2D.hpp"
#include "PointCloud.h"
#include "TriangleMesh.h"
#include "Raytracer.h"
#include "RaytracingMaterial.h"

#include "GeoXCanvas.h"
#include "LogoCanvas.h"
#include "OpenGLCanvas.h"
#include "OpenGLCanvas3D.h"
#include "OpenGLCanvas2D.h"
#include "SingleWidgetCanvas.h"
#include "GeoXView.h"
#include "GeoXOpenGLView.h"
#include "GLFixedFunctionMeshView.h"
#include "GLFixedFunctionPointCloudView.h"
#include "OpenGLPlotter2D.h"
#include "SingleWidgetView.h"
#include "GeoXTextView.h"
#include "GeoXImageView.h"

#include "GeoXWorkSpace.h"
#include "NamedObjectList.h"
#include "NamedObjectListEditor.h"


// ...
// include your experiments here
// ...

//---------------------------------------------------------------------------

void init() {
	Object::init(NULL);
	ClassCreator::init(Object::getClass());
	PropertyEditor::init(Object::getClass());
	BooleanPropertyEditor::init(PropertyEditor::getClass());
	ObjectPropertyEditor::init(PropertyEditor::getClass());
	FixedArrayPropertyEditor::init(PropertyEditor::getClass());
	ClassEditor::init(Object::getClass());
   MethodBrowser::init(ClassEditor::getClass());
   PropertyBrowser::init(ClassEditor::getClass());
   MemberBrowser::init(ClassEditor::getClass());
   Persistent::init(Object::getClass());
	StreamClassTable::init(Persistent::getClass());
	Camera::init(Persistent::getClass());
	ViewFrustum::init(Persistent::getClass());
	CameraController::init(Persistent::getClass());
	ExaminerCameraController::init(CameraController::getClass());
	WalkthroughCameraController::init(CameraController::getClass());

	Experiment::init(Persistent::getClass());
	ExampleExperiment::init(Experiment::getClass());
   MeshImporterExperiment::init(Experiment::getClass());
	SimpleGLMeshMaterial::init(Persistent::getClass());

   StructureDescriptor::init(Persistent::getClass());
   GeoXData::init(Persistent::getClass());
   DynamicArrayOfStructures::init(GeoXData::getClass());
	DynamicArrayOfStructures2D::init(GeoXData::getClass());
   PointCloud::init(GeoXData::getClass());
   TriangleMesh::init(PointCloud::getClass());

	Raytracer::init(Persistent::getClass());
	RaytracingMaterial::init(Persistent::getClass());

   GeoXCanvas::init(Persistent::getClass());
   LogoCanvas::init(GeoXCanvas::getClass());
   OpenGLCanvas::init(GeoXCanvas::getClass());
	OpenGLCanvas3D::init(OpenGLCanvas::getClass());
	OpenGLCanvas2D::init(OpenGLCanvas::getClass());
	SingleWidgetCanvas::init(GeoXCanvas::getClass());

   GeoXView::init(Persistent::getClass());
   GeoXOpenGLView::init(GeoXView::getClass());
   GLFixedFunctionMeshView::init(GeoXOpenGLView::getClass());
   GLFixedFunctionPointCloudView::init(GeoXOpenGLView::getClass());
	OpenGLPlotter2D::init(GeoXOpenGLView::getClass());
	GLPlotterCurveStyle::init(Persistent::getClass());
	GLPlotterCurveSelection::init(Persistent::getClass());
	SingleWidgetView::init(GeoXView::getClass());
	GeoXTextView::init(SingleWidgetView::getClass());
	GeoXImageView::init(SingleWidgetView::getClass());

   GeoXWorkSpace::init(Persistent::getClass());
   NamedObjectList::init(Persistent::getClass());
   NamedObjectListEntry::init(Persistent::getClass());
   NamedObjectListEditor::init(ClassEditor::getClass());
   const_cast<MetaClass*>(NamedObjectList::getClass())->getGUIDescriptor()->addClassEditor(NamedObjectListEditor::getClass(), false);

   // ...
   // register your experiments here
   // ...
}

void shutdown() {
	Object::shutdown();
	ClassEditor::shutdown();
	Persistent::shutdown();
	StreamClassTable::shutdown();
	Camera::shutdown();
	ViewFrustum::shutdown();
	CameraController::shutdown();
	ExaminerCameraController::shutdown();
	WalkthroughCameraController::shutdown();
	BooleanPropertyEditor::shutdown();
	ObjectPropertyEditor::shutdown();
	FixedArrayPropertyEditor::shutdown();
   MethodBrowser::shutdown();
   PropertyBrowser::shutdown();
   MemberBrowser::shutdown();

	Experiment::shutdown();
	ExampleExperiment::shutdown();
   MeshImporterExperiment::shutdown();
	SimpleGLMeshMaterial::shutdown();

	StructureDescriptor::shutdown();
   GeoXData::shutdown();

   DynamicArrayOfStructures::shutdown();
	DynamicArrayOfStructures2D::shutdown();
	PointCloud::shutdown();
	TriangleMesh::shutdown();

   GeoXCanvas::shutdown();
   LogoCanvas::shutdown();
   OpenGLCanvas::shutdown();
	OpenGLCanvas3D::shutdown();
	OpenGLCanvas2D::shutdown();
	SingleWidgetCanvas::shutdown();

   GeoXView::shutdown();
   GeoXOpenGLView::shutdown();
   GLFixedFunctionMeshView::shutdown();
   GLFixedFunctionPointCloudView::shutdown();
   OpenGLPlotter2D::shutdown();
	GLPlotterCurveStyle::shutdown();
	GLPlotterCurveSelection::shutdown();
	SingleWidgetView::shutdown();
	GeoXTextView::shutdown();
	GeoXImageView::shutdown();

   GeoXWorkSpace::shutdown();
   NamedObjectList::shutdown();
   NamedObjectListEntry::shutdown();
   NamedObjectListEditor::shutdown();

	Raytracer::shutdown();
	RaytracingMaterial::shutdown();
	// ...
   // unregister your experiments here
   // ...
}
