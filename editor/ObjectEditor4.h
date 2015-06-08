//---------------------------------------------------------------------------
#ifndef ObjectEditor4H
#define ObjectEditor4H
//---------------------------------------------------------------------------
#include "PTypes.h"
#include "ClassInstanceSet.h"
#include "VerticalScrollView.h"
#include "StringListener.h"
#include "GeoXWorkSpace.h"
#include "ExperimentContainer.h"
//---------------------------------------------------------------------------
#include "ui_ObjectEditor4.h"
//---------------------------------------------------------------------------
#include <string>
//---------------------------------------------------------------------------

class QCloseEvent;
class QTimer;
class Experiment;
class ClassEditor;
class ClassInstanceSet;
class VerticalScrollView;
class GeoXWorkSpace;
class NamedObjectListEditor;
class NamedObjectListEditorWidget;
class MDICanvasContainer;
class CreateCustomViewDialog;



class ObjectEditor4 : public QMainWindow, public StringListener, public ExperimentContainer {
	Q_OBJECT

 private:
   /// the editor edits a GeoXWorkSpace object
   GeoXWorkSpace *workSpace;
   /// filename under which the object is stored (initially empty)
   string workSpaceFilename;

   Ui_ObjectEditor4 ui;

   // experiments editor panel
   ClassEditor *experimentsEditor;
   QWidget *experimentsEditorWidget;
   Experiment *currentExperiment;
   ClassInstanceSet oldExperiments;
   VerticalScrollView *experimentView;

   // work space editor panel
   NamedObjectListEditor *dataEditor;
   NamedObjectListEditorWidget *dataEditorWidget;
   NamedObjectListEditor *canvasesEditor;
   NamedObjectListEditorWidget *canvasesEditorWidget;
   NamedObjectListEditor *viewsEditor;
   NamedObjectListEditorWidget *viewsEditorWidget;

   // for lazy updates
   QTimer *updateTimer;

	// dialog for creating views interactively
	CreateCustomViewDialog *customViewsDialog;

   void changeExperiment(const MetaClass *newExperimentClass);
   void bindExperiments(GeoXWorkSpace *workSpace);

 protected:
   /// queries the user to save
   bool canCloseWorkSpace();
   /// deletes work space (no questions asked)
   void closeWorkSpace();
   /// A new work space has been created/loaded; now, do the cleanup.
   void wireUpNewWorkSpace();
   /// Create an editor for the selected view (including no selection yet).
   /// Canvas editor must exist already.
   void rebuildViewsEditor();

 private slots:
   void on_actionExit_triggered();
   void sltExperimentChanged(const QString &newExperimentName);
   void on_actionOpen_triggered();
   void on_actionSave_triggered();
   void on_actionSaveAs_triggered();
   void on_actionNew_triggered();
   void on_actionClose_triggered();
   void dataSelected();
   void lazyUpdateSlot();
   void on_actionAboutGeoX_triggered();
   void on_pbCreateView_clicked();
   void on_actionShowInTabs_toggled(bool checked);
   void on_actionCascadeWindows_triggered();
   void on_actionTileWindows_triggered();
	void on_pbCreateCustomView_clicked();
   void canvasClosed(QString name, QWidget *sender);
   void newCanvasAdded(QString canvas);
   void newViewSelected();

 protected:
   void closeEvent(QCloseEvent *event);
   bool trySaveWorkSpace();

 public:
   ObjectEditor4(QWidget * parent=NULL, Qt::WindowFlags f=0);

   virtual void outputString(const string &s);
   /// redraw all views, do it immediately
   virtual void update();
   /// redraw all views, do it after gathering requests for a few milliseconds (or longer if the event loop does not run yet)
   virtual void updateLazy();
   /// create a new MDI window and add a canvas
   virtual MDICanvasContainer *createNewCanvasContainer(GeoXCanvas *canvas, string &name, bool addToWorkSpace = true);
   /// integrates an existing (presumably just newly created) canvas object into the environment
   virtual void addCanvas(GeoXCanvas *canvas, string &name, bool addToWorkSpace = true);

   ~ObjectEditor4();
};





#endif

