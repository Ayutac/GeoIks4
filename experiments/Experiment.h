//---------------------------------------------------------------------------
#ifndef ExperimentH
#define ExperimentH
//---------------------------------------------------------------------------
#include "Persistent.h"
//---------------------------------------------------------------------------

class QWidget;
class GeoXData;
class GeoXWorkSpace;
class ExperimentContainer;


///
/// Base class for experiments.
///
/// Implement and register a subclass to define a new experiment.
/// You can use GeoX properties and methods to define parameters and actions (buttons) for your experiment.
/// See "ExampleExperiment" for details.
///
class Experiment : public Persistent {
   GEOX_CLASS(Experiment)

 // -- internal representation - this could change
 private:
   /// reference to the work space this experiment operates on
   GeoXWorkSpace *workSpace;
   /// reference to the editor window this experiments operates on
   ExperimentContainer *container;

 // -- this is the interface available inside a new experiment class
 // -- it should be used to let your experiment communicate with the editor / environment
 protected:

   // --- creating new data objects
   //
   // objects store data to be processed & visualized

   /// This method creates a new object to work on.
   ///
   /// @param name:
   /// Objects are always identified by their name.
   /// Because names must be unique, the method might decide to change the name a bit to make it unique.
   /// The name that is finally used by the system is returned in the parameter name (this is why this is a
   /// reference parameter).
   ///
   /// @param type:
   /// To keep the system easily extensible, the type is specified as a class (MetaClass representes classes).\n
   /// The type must be a decendant of GeoXData. \n
   /// The standard version of the system always provides the following choices: \n
   ///
   /// type=TriangleMesh::getClass() -- create an empty triangle mesh \n
   /// type=PointCloud::getClass() -- create an empty point cloud (which is a triangle mesh w/o triangles, i.e., vertices only) \n
   /// type=DynamicArrayOfStructures::getClass() -- creates a simple array of structures (fill it with your data ad lib) \n
   ///
   virtual void createNewData(const MetaClass *type, string &name);

   /// This method adds a data object to the work space that already exists.
   ///
   /// You also have to give it a name.
   ///
   /// @param data:
   /// The data object - ownership is transferred to the workspace.
   ///
   /// @param name:
   /// Symbolic name for the new object. If the name already exists, a modified version is returned that is unique.
   virtual void addData(GeoXData *data, string &name);

   /// This method retrieves an object from the workspace.
   /// It returns a generic pointer to a Persistent (serializable) object.
   /// It is your task to double check that it has the correct type!
   ///
   virtual GeoXData *getData(const string &name);

   /// This method deletes an object from the work space (good-bye!).
   /// If the name is not found, nothing happens.
   ///
   /// @param name -- Name of the object to be deleted.
   virtual void deleteData(const string &name);


   // --- creating new canvas objects
   //
   // Cavas objects (base class: GeoXCanvas) provide an output area for visualization.
   // The canvas itself does not show any data; it is a container for views that visualize data objects or parts of them.

   /// This method creates a new canvas. A canvas is an output window in which data can be visualized.
   /// Usually, these appear in a new subwindow. Again, for modularity, the type is specified using a class type
   /// (MetaClass). Each canvans obtains a unique name; the paramter returns a modified name if the initially proposed
   /// name is already taken.
   ///
   /// @param type -- class of the canvas to be created \n
   ///   The following standard types are always supported: \n
   ///   OpenGLCanvas::getClass()   -- an OpenGL output area \n
   ///   BitmapCanvas::getClass()   -- displays bitmap images \n
   ///   OpenGLCanvas2D::getClass() -- displays vector graphics (e.g.: function plots, charts,...);
   ///                                 special case of OpenGL canvas adapted to 2D visualization. \n
   ///
   /// @param name -- proposed name (might be changed to make it unique)
   virtual void createCanvas(const MetaClass *type, string &name);

   /// This method deletes a canvas from the user interface.
   /// All views contained in that canvas are deleted, too (hasta la vista!).
   /// If the name is not found, nothing happens.
   ///
   /// @param name -- Name of the canvas to be deleted.
   virtual void deleteCanvas(const string &name);

   // --- creating new views
   //
   // View objects (base class: GeoXView) visualize a concrete object or a part of it.
   // Please note that not all views work with all types of objects.
   // There will be an error code if the view does not understand the data.

   /// This method creates a new view of an object.
   ///
   /// At creation time, the view links the data object (objectName), and the newly created view.
   /// The method also checks if the view is principally able to visualize the specified object. If that is not
   /// the case, the method returns false and an empty string in viewName.
   /// It also checks whether the specified canvas is able to display the view objects of the specified type.
   /// If that fails, the result is also false (and empty viewName).
   /// The canvasName specifies the canvas to be used; if left empty, a new canvas is created automatically.
   ///
   /// Important: please note that this method only creates the view itself; further parameters have to be set via
   /// the view object itself (use getView() ).
   ///
   /// @param dataName -- Name of the object to be displayed by the view.
   /// @param viewType -- type (class of the view, see below for examples) \n
   ///        The following standard types are supported: \n
   ///
   ///        GLFixedFunctionMeshView::getClass() -- A viewer that renders the mesh in some simple shading using the OpenGL fixed function pipeline.
   ///                                               requires a CanvasGL3D and a TriangleMesh object.\n
   ///        GLFixedFunctionPointCloudView::getClass() -- A viewer that renders the mesh in some simple shading using the OpenGL fixed function pipeline.
   ///                                               requires a CanvasGL3D and a TriangleMesh or PointCloud object.\n
   ///        OpenGLPlotter2D::getClass()     -- A viewer that plots data from a DynamicArrayOfStructures. Requires an OpenGLCanvas2D.\n
   ///        Bitmap2DView::getClass()        -- A viewer that displays a single bitmap image.\n
   ///
   /// @param viewName -- name of the new view, returns a unique name after checking/modifying if necessary
   /// @param canvasName -- name of the canvas where the view should appear. make sure it has the right type!
   /// \returns The function returns false and an empty viewName if anything went wrong (diagnostic output is written
   ///          to the console). Otherwise the result is true and viewName contains the (possibly changed)
   ///          unique name of the view. Use getView(viewName) to get an object reference to the view.
   virtual bool createView(const string &dataName, const MetaClass *viewType, string &viewName, const string canvasNamem);

   /// This method deletes a view from the user interface (Adiós!).
   /// If the name is not found, nothing happens.
   ///
   /// @param canvasName -- The name of the canvas from which the view should be removed. Must be given!
   /// @param viewName -- Name of the view to be deleted.
   virtual void deleteView(const string canvasName, const string &viewName);


   // -- repainting, animation
   //
   // views are only guaranteed to change if demanded explicitly

   /// This method repaints all views immediately. Useful for animations. See also updateViewsLazy() below.
   ///
   /// Rule of thumb:
   /// - Use updateViews for animations (your loop continues, more updates will be shown in the same process).
   /// - Use update for static changes (you've changed some data and want to show the result, no animation, or QTimer event based animations)
   virtual void updateViews();

   /// This method sends an event to the application to update all views within the next 10ms. See also updateViews() above.
   /// This allows to process multiple updates in one go (calling updateView() 10 times causes 10 repains;
   /// calling updateViewsLazy() 10 times directly after annother causes only one repaint).
   /// This is more efficient but the effect is only visible after the program returns from event processing.
   ///
   /// Rule of thumb:
   /// - Use updateViews for animations (your loop continues, more updates will be shown in the same process).
   /// - Use update for static changes (you've changed some data and want to show the result, no animation, or QTimer event based animations)
   virtual void updateViewsLazy();


 public:

   /// constructor does nothing special (initialize members to NULL)
   Experiment();

   /// this method is called when the experiment is started
   /// override if necessary
   virtual void experimentActivated() {}

   /// this method is called when the experiment is suspended (other user choice)
   /// override if necessary
   virtual void experimentDeactivated() {}

   /// This method will be called from external side if the experiment itself has been changed from outside.
   /// Usually, you do not need to worry about this one.
   virtual void updateExperiment() {}

   /// This method is called internally (by the container) to attach the experiment to a new work space.
   /// Usually, there is no need to override / call it explicitly.
   /// The work space parameter can be NULL. The container parameter must not be NULL.
   virtual void bindExperiment(GeoXWorkSpace *workSpace, ExperimentContainer *container);
};





#endif
