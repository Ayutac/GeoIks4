#-------------------------------------------------
#
# Project created by QtCreator 2015-03-31T00:02:15
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GeoX4
TEMPLATE = app

PRECOMPILED_HEADER = precompiledHeaders.h

DEFINES += USING_PCH

gcc:QMAKE_CXXFLAGS += -Wno-unused-variable \
                      -Wno-unused-parameter

unix {
   QMAKE_LFLAGS += -lGL -lGLU -lglut
   LIBS += -lGL -lglut -lGLU
}

gcc:CONFIG(debug, debug|release) {
   QMAKE_CXXFLAGS += -DDEBUG
}

CONFIG += exceptions

*msvc* {
   QMAKE_CXXFLAGS_EXCEPTIONS_ON = /EHa
   QMAKE_CXXFLAGS_STL_ON = /EHa
}


SOURCES += main.cpp \
    experiments/Experiment.cpp \
    experiments/MeshImporterExperiment.cpp \
    experiments/SimpleGLMeshMaterial.cpp \
    geometry/AttributeAccessToken.cpp \
    geometry/DynamicArrayOfStructures.cpp \
    geometry/MeshImporterSMFOBJ.cpp \
    geometry/PointCloud.cpp \
    geometry/StructureAttribDescriptor.cpp \
    geometry/StructureDescriptor.cpp \
    geometry/TriangleMesh.cpp \
    math/BoundingBox.cpp \
    math/DynamicLinearAlgebra.cpp \
    math/DynamicLinearAlgebraTools.cpp \
    math/Frustum.cpp \
    math/HalfSpace.cpp \
    math/IterativeSolvers.cpp \
    math/LinearAlgebra.cpp \
    math/MarchingCubes3D.cpp \
    math/PolarCoordinates.cpp \
    math/SparseLinearAlgebra.cpp \
    math/ViewFrustum.cpp \
    raytracing/KDTree.cpp \
    raytracing/MathTools.cpp \
    raytracing/Raytracer.cpp \
    raytracing/RaytracingMaterial.cpp \
    raytracing/RaytracingMath.cpp \
    system/basics/ClassCreator.cpp \
    system/basics/ClassEditor.cpp \
    system/basics/ClassMethods.cpp \
    system/basics/ClassProperty.cpp \
    system/basics/CopyObjectProperties.cpp \
    system/basics/ExceptionHandling.cpp \
    system/basics/HashTables.cpp \
    system/basics/MetaClass.cpp \
    system/basics/Object.cpp \
    system/basics/Persistent.cpp \
    system/gui/ClassSelectionDialog.cpp \
    system/gui/FileDialogs.cpp \
    system/gui/GeneralKey.cpp \
    system/gui/GeoXOutput.cpp \
    system/gui/MemberBrowser.cpp \
    system/gui/MethodBrowser.cpp \
    system/gui/Modifiers.cpp \
    system/gui/ObjectBrowserWidget.cpp \
    system/gui/ObjectCreationDialog.cpp \
    system/gui/ObjectViewsTable.cpp \
    system/gui/PopupWindow.cpp \
    system/gui/PropertyBrowser.cpp \
    system/gui/VerticalScrollView.cpp \
    system/misc/ClassInstanceSet.cpp \
    system/misc/qtHelpers.cpp \
    system/misc/StringHelper.cpp \
    system/misc/Timer.cpp \
    system/misc/Tools.cpp \
    system/properties/BooleanClassProperty.cpp \
    system/properties/BooleanElementEditor.cpp \
    system/properties/BooleanPropertyEditor.cpp \
    system/properties/ClassTypeClassProperty.cpp \
    system/properties/ClassTypeClassPropertyEditorWidget.cpp \
    system/properties/ClassTypePropertyEditor.cpp \
    system/properties/CompoundClassProperty.cpp \
    system/properties/ElementEditor.cpp \
    system/properties/EmbeddedObjectClassProperty.cpp \
    system/properties/EmbeddedObjectClassPropertyEditor.cpp \
    system/properties/EmbeddedObjectElementEditor.cpp \
    system/properties/EnumerationElementEditor.cpp \
    system/properties/EnumerationPropertyEditor.cpp \
    system/properties/FileNameElementEditor.cpp \
    system/properties/FileNamePropertyEditor.cpp \
    system/properties/FixedArrayClassProperty.cpp \
    system/properties/FixedArrayElementEditor.cpp \
    system/properties/FixedArrayPropertyEditor.cpp \
    system/properties/NumericalClassProperty.cpp \
    system/properties/ObjectClassProperty.cpp \
    system/properties/ObjectElementEditor.cpp \
    system/properties/ObjectListBase.cpp \
    system/properties/ObjectListElementEditor.cpp \
    system/properties/ObjectListProperty.cpp \
    system/properties/ObjectListPropertyEditor.cpp \
    system/properties/ObjectPropertyEditor.cpp \
    system/properties/PropertyEditor.cpp \
    system/properties/SeparatorClassProperty.cpp \
    system/properties/SeparatorPropertyEditor.cpp \
    system/properties/SingleClassProperty.cpp \
    system/properties/StringClassProperty.cpp \
    system/properties/StringElementEditor.cpp \
    system/properties/StringPropertyEditor.cpp \
    system/properties/StructClassProperty.cpp \
    system/properties/VariableArrayClassProperty.cpp \
    system/properties/VariableArrayClassPropertyEditor.cpp \
    system/properties/VariableArrayElementEditor.cpp \
    system/streaming/BinaryObjectStreams.cpp \
    system/streaming/LegacyClassNameTranslation.cpp \
    system/streaming/ObjectStreams.cpp \
    system/streaming/StreamClassTable.cpp \
    InitGeoX.cpp \
	 precompiledHeaders.cpp \
    experiments/ExampleExperiment.cpp \
    editor/ObjectEditor4.cpp \
    editor/GeoXWorkSpace.cpp \
    system/misc/NamedObjectList.cpp \
    editor/NamedObjectListEditor.cpp \
    geometry/GeoXData.cpp \
    editor/GeoXCanvas.cpp \
    editor/GeoXView.cpp \
    editor/NewViewDialog.cpp \
    system/basics/DynamicObject.cpp \
    system/basics/Variant.cpp \
    system/basics/VariantList.cpp \
    editor/canvas/cameraControl/Camera.cpp \
    editor/canvas/cameraControl/CameraController.cpp \
    editor/canvas/cameraControl/ExaminerCameraController.cpp \
    editor/canvas/cameraControl/WalkthroughCameraController.cpp \
    editor/MDICanvasContainer.cpp \
    editor/view/GeoXOpenGLView.cpp \
    editor/canvas/LogoCanvas.cpp \
    editor/canvas/OpenGLCanvas.cpp \
    editor/canvas/widgets/GeoXCanvasWidget.cpp \
    editor/canvas/widgets/OpenGLCanvasWidget.cpp \
    editor/canvas/widgets/GeoXOpenGLWidget.cpp \
    editor/canvas/widgets/LogoCanvasWidget.cpp \
    editor/view/OpenGLPlotter2D.cpp \
    editor/canvas/cameraControl/PanZoomCameraController2D.cpp \
    editor/canvas/OpenGLCanvas3D.cpp \
    editor/canvas/OpenGLCanvas2D.cpp \
    editor/view/GLFixedFunctionMeshView.cpp \
    editor/view/GLFixedFunctionPointCloudView.cpp \
    editor/canvas/SingleWidgetCanvas.cpp \
    editor/view/SingleWidgetView.cpp \
    editor/view/GeoXTextView.cpp \
    editor/view/widgets/GeoXTextViewWidget.cpp \
    editor/view/widgets/GeoXSingleWidgetViewWidget.cpp \
    editor/view/GeoXImageView.cpp \
    editor/view/widgets/GeoXImageViewWidget.cpp \
    geometry/DynamicArrayOfStructures2D.cpp \
    system/gui/TimerMethodWidget.cpp \
    editor/CreateCustomViewDialog.cpp

HEADERS += \
    experiments/ExampleExperiment.h \
    experiments/Experiment.h \
    experiments/MeshImporterExperiment.h \
    experiments/SimpleGLMeshMaterial.h \
    geometry/AttributeAccessToken.h \
    geometry/AttributeAccessToken.hpp \
    geometry/DynamicArrayOfStructures.h \
    geometry/DynamicArrayOfStructures.hpp \
    geometry/MeshImporterSMFOBJ.h \
    geometry/PointCloud.h \
    geometry/StructureAttribDescriptor.h \
    geometry/StructureAttribDescriptor.hpp \
    geometry/StructureDescriptor.h \
    geometry/StructureDescriptor.hpp \
    geometry/TriangleMesh.h \
    math/BoundingBox.h \
    math/BoundingBox.hpp \
    math/BoundingVolumeIntersection.h \
    math/DynamicLinearAlgebra.h \
    math/DynamicLinearAlgebra.hpp \
    math/DynamicLinearAlgebraTools.h \
    math/DynamicLinearAlgebraTools.hpp \
    math/Frustum.h \
    math/HalfSpace.h \
    math/HalfSpace.hpp \
    math/IterativeSolvers.h \
    math/IterativeSolvers.hpp \
    math/LinearAlgebra.h \
    math/LinearAlgebra.hpp \
    math/MarchingCubes3D.h \
    math/PolarCoordinates.h \
    math/Random.h \
    math/SparseLinearAlgebra.h \
    math/SparseLinearAlgebra.hpp \
    math/Sphere.h \
    math/ViewFrustum.h \
    raytracing/KDTree.h \
    raytracing/KDTree.hpp \
    raytracing/MathTools.h \
    raytracing/MathTools.hpp \
    raytracing/Raytracer.h \
    raytracing/RaytracingMaterial.h \
    raytracing/RaytracingMath.h \
    raytracing/RaytracingMath.hpp \
    system/basics/ClassCreator.h \
    system/basics/ClassEditor.h \
    system/basics/ClassMethods.h \
    system/basics/ClassProperty.h \
    system/basics/CopyObjectProperties.h \
    system/basics/Delegate.h \
    system/basics/ExceptionHandling.h \
    system/basics/Exceptions.h \
    system/basics/HashTables.h \
    system/basics/HashTables.hpp \
    system/basics/MetaClass.h \
    system/basics/Object.h \
    system/basics/PAssert.h \
    system/basics/Persistent.h \
    system/basics/PTypes.h \
    system/gui/ClassSelectionDialog.h \
    system/gui/Editor.h \
    system/gui/FileDialogs.h \
    system/gui/GeneralKey.h \
    system/gui/GeoXOutput.h \
    system/gui/KeyboardController.h \
    system/gui/MemberBrowser.h \
    system/gui/MethodBrowser.h \
    system/gui/Modifiers.h \
    system/gui/MouseButtons.h \
    system/gui/MouseController.h \
    system/gui/ObjectBrowserWidget.h \
    system/gui/ObjectCreationDialog.h \
    system/gui/ObjectViewsTable.h \
    system/gui/PopupWindow.h \
    system/gui/PropertyBrowser.h \
    system/gui/VerticalScrollView.h \
    system/misc/ClassInstanceSet.h \
    system/misc/qtHelpers.h \
    system/misc/StringHelper.h \
    system/misc/Timer.h \
    system/misc/Tools.h \
    system/misc/values_vs.h \
    system/properties/BooleanClassProperty.h \
    system/properties/BooleanElementEditor.h \
    system/properties/BooleanPropertyEditor.h \
    system/properties/ClassTypeClassProperty.h \
    system/properties/ClassTypeClassPropertyEditorWidget.h \
    system/properties/ClassTypePropertyEditor.h \
    system/properties/CompoundClassProperty.h \
    system/properties/ElementEditor.h \
    system/properties/EmbeddedObjectClassProperty.h \
    system/properties/EmbeddedObjectClassPropertyEditor.h \
    system/properties/EmbeddedObjectElementEditor.h \
    system/properties/EnumerationElementEditor.h \
    system/properties/EnumerationPropertyEditor.h \
    system/properties/FileNameElementEditor.h \
    system/properties/FileNamePropertyEditor.h \
    system/properties/FixedArrayClassProperty.h \
    system/properties/FixedArrayElementEditor.h \
    system/properties/FixedArrayPropertyEditor.h \
    system/properties/NumericalClassProperty.h \
    system/properties/ObjectClassProperty.h \
    system/properties/ObjectElementEditor.h \
    system/properties/ObjectListBase.h \
    system/properties/ObjectListElementEditor.h \
    system/properties/ObjectListProperty.h \
    system/properties/ObjectListPropertyEditor.h \
    system/properties/ObjectPropertyEditor.h \
    system/properties/Properties.h \
    system/properties/PropertyEditor.h \
    system/properties/SeparatorClassProperty.h \
    system/properties/SeparatorPropertyEditor.h \
    system/properties/SingleClassProperty.h \
    system/properties/StringClassProperty.h \
    system/properties/StringElementEditor.h \
    system/properties/StringPropertyEditor.h \
    system/properties/StructClassProperty.h \
    system/properties/VariableArrayClassProperty.h \
    system/properties/VariableArrayClassPropertyEditor.h \
    system/properties/VariableArrayElementEditor.h \
    system/streaming/BinaryObjectStreams.h \
    system/streaming/LegacyClassNameTranslation.h \
    system/streaming/ObjectStreams.h \
    system/streaming/StreamClassTable.h \
    InitGeoX.h \
	 precompiledHeaders.h \
    editor/ObjectEditor4.h \
    system/gui/StringListener.h \
    editor/GeoXView.h \
    editor/GeoXCanvas.h \
    editor/GeoXWorkSpace.h \
    editor/ExperimentContainer.h \
    system/misc/NamedObjectList.h \
    editor/NamedObjectListEditor.h \
    geometry/GeoXData.h \
    editor/NewViewDialog.h \
    system/basics/DynamicObject.h \
    system/basics/Variant.h \
    system/basics/VariantList.h \
    editor/canvas/cameraControl/Camera.h \
    editor/canvas/cameraControl/CameraController.h \
    editor/canvas/cameraControl/ExaminerCameraController.h \
    editor/canvas/cameraControl/WalkthroughCameraController.h \
    editor/MDICanvasContainer.h \
    editor/view/GeoXOpenGLView.h \
    editor/canvas/LogoCanvas.h \
    editor/canvas/OpenGLCanvas.h \
    editor/canvas/widgets/GeoXOpenGLWidget.h \
    editor/canvas/widgets/OpenGLCanvasWidget.h \
    editor/canvas/widgets/GeoXCanvasWidget.h \
    editor/canvas/widgets/LogoCanvasWidget.h \
    editor/canvas/widgets/ViewerImgUpdateCallBack.h \
    editor/view/OpenGLPlotter2D.h \
    editor/canvas/cameraControl/PanZoomCameraController2D.h \
    editor/canvas/OpenGLCanvas3D.h \
    editor/canvas/OpenGLCanvas2D.h \
    editor/view/GLFixedFunctionMeshView.h \
    editor/view/GLFixedFunctionPointCloudView.h \
    editor/canvas/SingleWidgetCanvas.h \
    editor/view/SingleWidgetView.h \
    editor/view/GeoXTextView.h \
    editor/view/widgets/GeoXTextViewWidget.h \
    editor/view/widgets/GeoXSingleWidgetViewWidget.h \
    editor/view/GeoXImageView.h \
    editor/view/widgets/GeoXImageViewWidget.h \
    geometry/DynamicArrayOfStructures2D.h \
    geometry/DynamicArrayOfStructures2D.hpp \
    system/gui/TimerMethodWidget.h \
    editor/CreateCustomViewDialog.h

INCLUDEPATH += \
	 ./ \
	 experiments \
	 geometry \
	 geoX \
	 math \
	 raytracing \
	 system \
	 system/basics \
	 system/gui \
	 system/misc \
	 system/properties\
	 system/streaming\
	 viewers/cameraControl \
    viewers/widgets \
    editor \
    editor/canvas \
    editor/canvas/cameraControl \
    editor/canvas/widgets \
	 editor/view \
	 editor/view/widgets


RESOURCES += \
	 images/geoX.qrc

FORMS += \
    system/gui/ObjectBrowserWidgetDesigner.ui \
	 system/gui/ObjectCreationDialogDesigner.ui \
    editor/ObjectEditor4.ui \
    editor/NamedObjectListEditorWidget.ui \
    editor/NewViewDialog.ui \
    editor/canvas/widgets/LogoCanvas.ui \
    editor/canvas/widgets/OpenGLCanvas.ui \
    editor/view/widgets/GeoXTextViewWidget.ui \
    editor/view/widgets/GeoXImageViewWidget.ui \
    editor/view/widgets/IDWFileIOPanel.ui \
    editor/view/widgets/IDWImagePanel.ui \
    editor/view/widgets/IDWMappingPanel.ui \
    system/gui/TimerMethodWidget.ui \
    editor/CreateCustomViewDialog.ui
