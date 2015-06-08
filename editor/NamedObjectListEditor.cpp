#include "precompiledHeaders.h"
#include "NamedObjectListEditor.h"
#include "ClassSelectionDialog.h"
#include "ClassCreator.h"
#include "NamedObjectList.h"
#include "StringHelper.h"
#include "ObjectBrowserWidget.h"
#include "ExceptionHandling.h"
#include <QMessageBox>
#include "GeoXOutput.h"

#include "ui_NamedObjectListEditorWidget.h"


IMPLEMENT_CLASS_EDITOR(NamedObjectList)

NamedObjectListEditorWidget::NamedObjectListEditorWidget(QWidget *parent) :
   QWidget(parent),
   ui(new Ui::NamedObjectListEditorWidget)
{
   ui->setupUi(this);
   classSelectionDialog = NULL;
   list = NULL;
}

void NamedObjectListEditorWidget::on_pbAdd_clicked()
{
   STD_GLOBAL_EXCEPTION_HANDLER_START
   if (!list || !classSelectionDialog) return;
   QPoint pos = ui->pbAdd->mapToGlobal(QPoint(0,ui->pbAdd->height()));
   classSelectionDialog->move(pos);
   if( classSelectionDialog->exec() == QDialog::Accepted )
   {
      const MetaClass * mc = classSelectionDialog->getSelectedClass();
      const MetaClass * ccmc = mc->getGUIDescriptor()->getDefaultClassCreator();
      Persistent * obj = NULL;
      if (ccmc == NULL) {
         obj = (Persistent*)mc->newInstance();
      } else {
         ClassCreator *cc = (ClassCreator*)ccmc->newInstance();
         obj = (Persistent*)ClassCreator::interactivelyCreateObject(cc); // result may be NULL
         delete cc;
      }
      if (obj != NULL) {
         string name = string("new-")+mc->getClassName();
         list->addObject(obj, name);
         updateEditor();
         emit newItemAdded(name.c_str());
      }
   }
   STD_GLOBAL_EXCEPTION_HANDLER_END
}

void NamedObjectListEditorWidget::on_pbDelete_clicked()
{
   STD_GLOBAL_EXCEPTION_HANDLER_START
   if (!list) return;
   if( !ui->lwObjects->selectedItems().empty() ) {
      QString qname = ui->lwObjects->currentItem()->text();
      if( QMessageBox::question(this, "", QString("Do you want to delete \'") + qname + "\' ?",
         QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes )
      {
         list->deleteObject( qString2STLString(qname) );
         updateEditor();
      }
   }
   STD_GLOBAL_EXCEPTION_HANDLER_END
}

void NamedObjectListEditorWidget::on_lwObjects_itemSelectionChanged()
{
   STD_GLOBAL_EXCEPTION_HANDLER_START
   if (!list) return;
   if( !ui->lwObjects->selectedItems().empty() ) {
      ui->pbDelete->setEnabled(true);
   } else {
      ui->pbDelete->setEnabled(false);
   }
   emit selectionChanged();
   STD_GLOBAL_EXCEPTION_HANDLER_END
}


void NamedObjectListEditorWidget::on_lwObjects_itemDoubleClicked(QListWidgetItem *item)
{
   STD_GLOBAL_EXCEPTION_HANDLER_START
   if (!list) return;
   if(item != NULL)	{
      string name = qString2STLString(item->text());
      Persistent *obj = list->getObject(name);
      if (obj) {
         const GUIDescriptor * guiDesc = obj->getInstanceClass()->getGUIDescriptor();
         const MetaClass *mc = guiDesc->getDefaultClassEditor();

			if (mc) {

				ClassEditor * ce = (ClassEditor*)mc->newInstance();
				ObjectBrowserWidget *newBrowser = new ObjectBrowserWidget(obj, ce, this, 0, true);
				QWidget *w = ce->makeEditor(newBrowser, obj);
				newBrowser->setupEditor(w);
				//connect( w, SIGNAL( destroyed( QObject* ) ), SLOT( sltDestroyEditorWidget( QObject* ) ) );
				newBrowser->move( QCursor::pos() );
				newBrowser->show();
				ObjectViewsTable::bindView( obj, ce );
				QWidget::connect(this, SIGNAL(sigModified()), newBrowser, SLOT(deleteLater()));
			} else {
				warning("no default editor class found (please check that your class has been properly registered in initGeoX.cpp)");
			}
      }
   }
   STD_GLOBAL_EXCEPTION_HANDLER_END
}

void NamedObjectListEditorWidget::updateEditor()
{
   STD_GLOBAL_EXCEPTION_HANDLER_START
   ui->lwObjects->clear();
   if (list) {
      for(mpcard i=0; i<list->getSize(); i++) {
         string objectName = list->getName(i);
         ui->lwObjects->addItem(objectName.c_str());
      }
   }
   emit sigModified();
   STD_GLOBAL_EXCEPTION_HANDLER_END
}

void NamedObjectListEditorWidget::setup(NamedObjectList *list)
{
   this->list = list;
   delete classSelectionDialog;
   if (list) {
      classSelectionDialog = new ClassSelectionDialog(this);
      classSelectionDialog->init( list->getBaseType() );
   } else {
      delete classSelectionDialog;
      classSelectionDialog = NULL;
   }
   updateEditor();
}

string NamedObjectListEditorWidget::getSelectedObject()
{
   QList<QListWidgetItem*> selection = ui->lwObjects->selectedItems();
   if( !selection.empty() ) {
      QString qname = selection[0]->text();
      return qString2STLString(qname);
   } else {
      return "";
   }
}

void NamedObjectListEditorWidget::setProvideAddDeleteButtons(bool buttons)
{
   ui->frameBtns->setVisible(buttons);
}


NamedObjectListEditorWidget::~NamedObjectListEditorWidget()
{
   delete ui;
}
