//---------------------------------------------------------------------------
#include "precompiledHeaders.h"
//---------------------------------------------------------------------------
#include "ObjectEditor4.h"
//---------------------------------------------------------------------------
#include "ExceptionHandling.h"
#include "StringHelper.h"
#include "Experiment.h"
#include "FileDialogs.h"
#include "BinaryObjectStreams.h"
#include "GeoXWorkSpace.h"
#include "NamedObjectList.h"
#include "NamedObjectListEditor.h"
#include "MDICanvasContainer.h"
#include "LogoCanvas.h"
#include "OpenGLCanvas.h"
#include "GeoXData.h"
#include "CreateCustomViewDialog.h"
//---------------------------------------------------------------------------
#include <qevent.h>
#include <QMessageBox>
#include <QtCore/QTimer>
#include <QFrame>
#include <QVBoxLayout>
#include <iostream>
#include "GeoXOutput.h"
//---------------------------------------------------------------------------

ObjectEditor4::ObjectEditor4(QWidget * parent, Qt::WindowFlags f)
:QMainWindow(parent, f)
{
   workSpace = NULL;
	customViewsDialog = NULL;
   ui.setupUi(this);	
   output.addListener(this);
	move(0,0);
	ui.dockOutput->setMaximumHeight(100);
   ui.dwWorkspace->hide();
   ui.dockObjectView->hide();

   setWindowTitle( "GeoX WorkSpace" );
	setAttribute(Qt::WA_DeleteOnClose);

   experimentsEditor = NULL;
   experimentsEditorWidget = NULL;
   currentExperiment = NULL;

   dataEditor = NULL;
   dataEditorWidget = NULL;

   canvasesEditor = NULL;
   canvasesEditorWidget = NULL;

   viewsEditor = NULL;
   viewsEditorWidget = NULL;

   QVBoxLayout* tv=new QVBoxLayout;
   tv->setMargin(0);
   tv->setSpacing(0);
   ui.experimentObjectViewFrame->setLayout(tv);
   experimentView = new VerticalScrollView( ui.experimentObjectViewFrame );
   ui.experimentObjectViewFrame->layout()->addWidget( experimentView );

   ui.experimentSelector->collectClasses("Experiment", false);
   QWidget::connect(ui.experimentSelector, SIGNAL(currentIndexChanged(QString)), this , SLOT(sltExperimentChanged(QString)));

   updateTimer = new QTimer(this);
   updateTimer->setSingleShot(true);
   updateTimer->stop();
   connect(updateTimer, SIGNAL(timeout()), this, SLOT(lazyUpdateSlot()));

	customViewsDialog = new CreateCustomViewDialog(this, ui.mdiArea);
	customViewsDialog->hide();
}

ObjectEditor4::~ObjectEditor4()
{
   output.removeListener(this);
}


void ObjectEditor4::closeEvent(QCloseEvent *event) {
   if (workSpace) {
      if (canCloseWorkSpace()) {
         event->setAccepted(true);
      } else {
         event->setAccepted(false);
      }
   } else {
      event->setAccepted(true);
   }
}

void ObjectEditor4::outputString(const std::string &s) {
   ui.outputWindow->moveCursor( QTextCursor::End );
   QString qs = s.c_str();
   qs.replace(QRegExp("a[\n]"), "\r\n");
   ui.outputWindow->insertPlainText(qs);
}

void ObjectEditor4::update()
{
   if (workSpace) workSpace->update();
   if (dataEditor) dataEditor->update();
   if (canvasesEditor) canvasesEditor->update();
}

void ObjectEditor4::updateLazy()
{
   if (!updateTimer->isActive()) updateTimer->start(10);
}

MDICanvasContainer *ObjectEditor4::createNewCanvasContainer(GeoXCanvas *canvas, string &name, bool addToWorkSpace)
{
   if (addToWorkSpace) {
      if (workSpace) {
         workSpace->getCanvases()->addObject(canvas, name);
      }
   }
   MDICanvasContainer *win = new MDICanvasContainer(ui.mdiArea);
   canvas->setup(win);
   string windowTitle = name;
   win->setup(canvas->getWidget(), name, windowTitle);
   win->show();
   updateLazy();
   return win;
}


void ObjectEditor4::on_actionExit_triggered()
{
   close();
}

void ObjectEditor4::sltExperimentChanged(const QString &newExperimentName) {
   STD_GLOBAL_EXCEPTION_HANDLER_START
   std::string  s = qString2STLString(newExperimentName);
   const MetaClass *newExperimentClass = MetaClass::getClassByName(s);
   if (newExperimentClass == NULL ) return;
   changeExperiment( newExperimentClass );
   STD_GLOBAL_EXCEPTION_HANDLER_END
}

void ObjectEditor4::changeExperiment(const MetaClass * newExperimentClass)
{
   STD_GLOBAL_EXCEPTION_HANDLER_START
   string s = newExperimentClass->getClassName();

   if (newExperimentClass == NULL ) return;
   ui.experimentSelector->setSelection(s);

   if (currentExperiment != NULL) {
      currentExperiment->experimentDeactivated();
      oldExperiments.insertObject(currentExperiment);
      currentExperiment = NULL;
   }

   if (experimentsEditor != NULL) {
      delete experimentsEditor;
      experimentsEditor = NULL;
   }

   if (experimentsEditorWidget != NULL) {
      delete experimentsEditorWidget;
   }
   experimentsEditorWidget = NULL;

   if (newExperimentClass == NULL || newExperimentClass->isAbstract()) return;

   if (!oldExperiments.containsObjectOfClass(newExperimentClass))
   {
      currentExperiment = (Experiment*)(newExperimentClass->newInstance());
      currentExperiment->bindExperiment(workSpace, this);
      oldExperiments.insertObject(currentExperiment);
   } else {
      currentExperiment = (Experiment*)(oldExperiments.getInstance(newExperimentClass));
   }
   /*setDisplayWidget(currentExperiment->createViewer());*/
   currentExperiment->experimentActivated();
   const MetaClass *newExperimentEditorClass = newExperimentClass->getGUIDescriptor()->getDefaultClassEditor();

   if (newExperimentEditorClass == NULL || newExperimentEditorClass->isAbstract()) return;

   experimentsEditor = (ClassEditor*)(newExperimentEditorClass->newInstance());
   experimentsEditorWidget = experimentsEditor->makeEditor(experimentView->viewport() , currentExperiment);
   experimentView->setWidget(experimentsEditorWidget);
   experimentsEditorWidget->show();

   STD_GLOBAL_EXCEPTION_HANDLER_END
}

void ObjectEditor4::bindExperiments(GeoXWorkSpace *workSpace)
{
   card32 n = oldExperiments.getNumInstances();
   for (card32 i=0; i<n; i++) {
      Experiment *e = (Experiment*)oldExperiments.getInstanceByIndex(i);
      e->bindExperiment(workSpace, this);
   }
   if (currentExperiment) {
      currentExperiment->bindExperiment(workSpace, this);
   }
}

void ObjectEditor4::closeWorkSpace()
{
   ui.dwWorkspace->hide();
   ui.dockObjectView->hide();
   bindExperiments(NULL);
   delete dataEditor;
   delete dataEditorWidget;
   delete canvasesEditor;
   delete canvasesEditorWidget;
   delete viewsEditor;
   delete viewsEditorWidget;

   dataEditor = NULL;
   dataEditorWidget = NULL;
   canvasesEditor = NULL;
   canvasesEditorWidget = NULL;
   viewsEditor = NULL;
   viewsEditorWidget = NULL;

   delete workSpace;
   workSpace = NULL;

   ui.actionSave->setEnabled(false);
   ui.actionClose->setEnabled(false);
   ui.actionSaveAs->setEnabled(false);
}

void ObjectEditor4::wireUpNewWorkSpace()
{
   STD_GLOBAL_EXCEPTION_HANDLER_START
   bindExperiments(workSpace);

   NamedObjectList *data = workSpace->getData();
   delete dataEditor;
   dataEditor = NULL;
   delete dataEditorWidget;
   dataEditorWidget = NULL;
   dataEditor = new NamedObjectListEditor();
   dataEditorWidget = (NamedObjectListEditorWidget*)dataEditor->makeEditor(ui.dwWorkspace, data);
   QWidget::connect(dataEditorWidget, SIGNAL(selectionChanged()), this, SLOT(dataSelected()));

   ui.layoutData->addWidget(dataEditorWidget);
   ui.layoutData->setMargin(0);
   ui.layoutData->setSpacing(0);
   dataEditorWidget->show();

   NamedObjectList *canvases = workSpace->getCanvases();
   delete canvasesEditor;
   canvasesEditor = NULL;
   delete canvasesEditorWidget;
   canvasesEditorWidget = NULL;
   canvasesEditor = new NamedObjectListEditor();
   canvasesEditorWidget = (NamedObjectListEditorWidget*)canvasesEditor->makeEditor(ui.dwWorkspace, canvases);
   QWidget::connect(canvasesEditorWidget, SIGNAL(newItemAdded(QString)), this, SLOT(newCanvasAdded(QString)));
   QWidget::connect(canvasesEditorWidget, SIGNAL(selectionChanged()), this, SLOT(newViewSelected()));

   ui.layoutCanvases->addWidget(canvasesEditorWidget);
   ui.layoutCanvases->setMargin(0);
   ui.layoutCanvases->setSpacing(0);
   canvasesEditorWidget->show();

   rebuildViewsEditor();

   ui.dwWorkspace->show();
   ui.dockObjectView->show();
   ui.actionSave->setEnabled(true);
   ui.actionClose->setEnabled(true);
   ui.actionSaveAs->setEnabled(true);
   STD_GLOBAL_EXCEPTION_HANDLER_END
}

void ObjectEditor4::rebuildViewsEditor()
{
   NamedObjectList *views = NULL;

   if (canvasesEditorWidget) {
      string selectedCanvas = canvasesEditorWidget->getSelectedObject();
      if (selectedCanvas != "") {
         GeoXCanvas *selCanvasPtr = dynamic_cast<GeoXCanvas*>(workSpace->getCanvases()->getObject(selectedCanvas));
         if (selCanvasPtr) {
            views = selCanvasPtr->getViews();
         }
      }
   }

   delete viewsEditor;
   viewsEditor = NULL;
   delete viewsEditorWidget;
   viewsEditorWidget = NULL;

   viewsEditor = new NamedObjectListEditor();
   viewsEditorWidget = (NamedObjectListEditorWidget*)viewsEditor->makeEditor(ui.dwWorkspace, views);
   viewsEditorWidget->setProvideAddDeleteButtons(false);
   if (!views) viewsEditorWidget->setEnabled(false);

   ui.layoutViews->addWidget(viewsEditorWidget);
   ui.layoutViews->setMargin(0);
   ui.layoutViews->setSpacing(0);
   viewsEditorWidget->show();
}

bool ObjectEditor4::canCloseWorkSpace()
{
   STD_GLOBAL_EXCEPTION_HANDLER_START
   int answer =  QMessageBox::question(this,
      "Close current work space?",
      "Do you want to save the current work space before closing?",
      QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
      QMessageBox::Cancel);
   if (answer == QMessageBox::Yes) {
      return trySaveWorkSpace();
   } else if (answer == QMessageBox::Cancel) {
      return false;
   }
   return true;
   STD_GLOBAL_EXCEPTION_HANDLER_END
   return true;
}

void ObjectEditor4::on_actionOpen_triggered()
{
   STD_GLOBAL_EXCEPTION_HANDLER_START
   if (workSpace != NULL) {
      if (!canCloseWorkSpace()) return;
   }
   string filename = FileDialogs::getOpenFileName(this, "Open Work Space...",
      "GeoX Objects (*.object);;All Files (*.*)");
   if (!filename.empty()) {
      Persistent *p = NULL;
      try {
         BinaryInputObjectStream in(filename.c_str());
         in.readAnyObject(p);
      } catch (...) {
         error("Something went wrong reading the file. Operation aborted.");
         return;
      }
      if (dynamic_cast<GeoXWorkSpace*>(p) == NULL) {
         delete p;
         error("File is not a work space object.");
         return;
      }
      closeWorkSpace();
      workSpace = (GeoXWorkSpace*)p;
      workSpaceFilename = filename;
      wireUpNewWorkSpace();
   }
   STD_GLOBAL_EXCEPTION_HANDLER_END
}

void ObjectEditor4::on_actionSave_triggered()
{
   STD_GLOBAL_EXCEPTION_HANDLER_START
   trySaveWorkSpace();
   STD_GLOBAL_EXCEPTION_HANDLER_END
}

void ObjectEditor4::on_actionSaveAs_triggered()
{
   STD_GLOBAL_EXCEPTION_HANDLER_START
   if (workSpaceFilename.empty()) {
      workSpaceFilename = FileDialogs::getSaveFileName(this, "Save Work Space...",
         "GeoX Objects (*.object);;All Files (*.*)");
      if (workSpaceFilename.empty()) return;
   }
   try {
      BinaryOutputObjectStream out(workSpaceFilename.c_str());
      out.writeObject(workSpace);
   } catch (...) {
      error("cannot save file - something went wrong.");
   }
   STD_GLOBAL_EXCEPTION_HANDLER_END
}

bool ObjectEditor4::trySaveWorkSpace() {
   STD_GLOBAL_EXCEPTION_HANDLER_START
   if (workSpace == NULL) {
      warning("no active work space, cannot save");
      return true;
   }
   if (workSpaceFilename.empty()) {
      workSpaceFilename = FileDialogs::getSaveFileName(this, "Save Work Space...",
         "GeoX Objects (*.object);;All Files (*.*)");
      if (workSpaceFilename.empty()) return false;
   }
   try {
      BinaryOutputObjectStream out(workSpaceFilename.c_str());
      out.writeObject(workSpace);
   } catch (...) {
      error("cannot save file - something went wrong.");
      return false;
   }
   return true;
   STD_GLOBAL_EXCEPTION_HANDLER_END
   return true;
}

void ObjectEditor4::on_actionNew_triggered()
{
   STD_GLOBAL_EXCEPTION_HANDLER_START
   if (workSpace != NULL) {
      if (!canCloseWorkSpace()) return;
   }
   bindExperiments(NULL);
   delete workSpace;
   workSpace = new GeoXWorkSpace();
   wireUpNewWorkSpace();
   STD_GLOBAL_EXCEPTION_HANDLER_END
}

void ObjectEditor4::on_actionClose_triggered()
{
   if (canCloseWorkSpace()) {
      closeWorkSpace();
   }
}

void ObjectEditor4::dataSelected()
{
   ui.pbCreateView->setEnabled(dataEditorWidget->getSelectedObject() != "");
}

void ObjectEditor4::lazyUpdateSlot()
{
   updateTimer->stop();
   update();
}

void ObjectEditor4::on_actionAboutGeoX_triggered()
{
   LogoCanvas *lc = new LogoCanvas();
   string name = "aboutGeoX4";
   addCanvas(lc, name);
}

void ObjectEditor4::on_pbCreateView_clicked()
{
	STD_GLOBAL_EXCEPTION_HANDLER_START
   string sel = dataEditorWidget->getSelectedObject();
   if (sel != "") {
      GeoXData *data = dynamic_cast<GeoXData*>(workSpace->getData()->getObject(sel));
      if (data) {
         const MetaClass *mcViewer = data->getDefaultViewer();
         const MetaClass *mcCanvas = data->getDefaultViewerDefaultCanvas();
         GeoXCanvas *canvas = dynamic_cast<GeoXCanvas*>(mcCanvas->newInstance());
         GeoXView *view = dynamic_cast<GeoXView*>(mcViewer->newInstance());
         if (canvas && view) {
            view->setup(workSpace, canvas, sel);
            string canvasName = string("new_") + mcCanvas->getClassName();
            addCanvas(canvas, canvasName);
            string viewName = sel + string("_[")+ mcViewer->getClassName() + string("]");
            canvas->addView(view, viewName);
         } else {
            warning("ObjectEditor4::on_pbCreateView_clicked() - something went very wrong: the default view or canvas instance could not be created.");
         }
      } else {
         warning("ObjectEditor4::on_pbCreateView_clicked() - This should not happen (data not found).");
      }
   } else {
      warning("ObjectEditor4::on_pbCreateView_clicked() - This should not happen (selection empty).");
   }
	STD_GLOBAL_EXCEPTION_HANDLER_END
}

void ObjectEditor4::addCanvas(GeoXCanvas *canvas, string &name, bool addToWorkSpace) {
   MDICanvasContainer *win = createNewCanvasContainer(canvas, name, addToWorkSpace);
   QWidget::connect(win, SIGNAL(canvasClosed(QString,QWidget*)), this, SLOT(canvasClosed(QString,QWidget*)));
   win->showMaximized();
   updateLazy();
}

void ObjectEditor4::on_actionShowInTabs_toggled(bool checked)
{
   if (checked) {
      ui.mdiArea->setViewMode(QMdiArea::TabbedView);
   } else {
      ui.mdiArea->setViewMode(QMdiArea::SubWindowView);
   }
}

void ObjectEditor4::on_actionCascadeWindows_triggered()
{
   ui.mdiArea->cascadeSubWindows();
}

void ObjectEditor4::on_actionTileWindows_triggered()
{
	ui.mdiArea->tileSubWindows();
}

void ObjectEditor4::on_pbCreateCustomView_clicked()
{
	GeoXCanvas *newCanvas = NULL;
	MDICanvasContainer *win = NULL;
	customViewsDialog->showDialog(workSpace, newCanvas, win);
	if (newCanvas) {
		QWidget::connect(win, SIGNAL(canvasClosed(QString,QWidget*)), this, SLOT(canvasClosed(QString,QWidget*)));
		win->showMaximized();
	}
	updateLazy();
}

void ObjectEditor4::canvasClosed(QString name, QWidget *sender)
{
   string sname = qString2STLString(name);
   workSpace->getCanvases()->deleteObject(sname);
   sender->deleteLater();
   updateLazy();
}

void ObjectEditor4::newCanvasAdded(QString canvas)
{
   string name = qString2STLString(canvas);
   GeoXCanvas *c = dynamic_cast<GeoXCanvas*>(workSpace->getCanvases()->getObject(name));
   addCanvas(c, name, false);
}

void ObjectEditor4::newViewSelected()
{
   rebuildViewsEditor();
}
