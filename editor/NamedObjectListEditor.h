#ifndef NAMEDOBJECTLISTEDITOR_H
#define NAMEDOBJECTLISTEDITOR_H

#include "ClassEditor.h"

#include <QWidget>
#include <QListWidgetItem>


// --- forwards

namespace Ui { class NamedObjectListEditorWidget; }
class ClassSelectionDialog;
class NamedObjectList;
class ClassEditor;
class PopupWindow;



/// Class editor (widget part) for named object lists
class NamedObjectListEditorWidget : public QWidget {
   Q_OBJECT

 private:
   Ui::NamedObjectListEditorWidget *ui;
   ClassSelectionDialog *classSelectionDialog ;
   NamedObjectList *list;

 private slots:
   /// internal
   void on_pbAdd_clicked();
   void on_pbDelete_clicked();
   void on_lwObjects_itemSelectionChanged();
   void on_lwObjects_itemDoubleClicked(QListWidgetItem *item);

 public:
   /// yes - make one
   explicit NamedObjectListEditorWidget(QWidget *parent = 0);

   /// write changes to editor from object
   virtual void updateEditor();

   /// internal - connect editor to instance
   /// this editor supports disconnections (list == NULL)
   /// and reconnections (calling setup multiple times)
   void setup(NamedObjectList *list);

   /// returns empty string if nothing is selected
   string getSelectedObject();

   /// this method can be used to hide the buttons to add/delete objects
   void setProvideAddDeleteButtons(bool buttons);

   ~NamedObjectListEditorWidget();

 signals:
   /// internal - for shutting down dependent editors
   void sigModified();
   void selectionChanged();
   void newItemAdded(QString name);

};

// automatically create the connection code to makes this a class editor
MAKE_CLASS_EDITOR( , NamedObjectList )


#endif // NAMEDOBJECTLISTEDITOR_H
