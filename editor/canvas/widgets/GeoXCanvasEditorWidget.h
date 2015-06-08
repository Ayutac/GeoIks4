#ifndef GEOXCANVASEDITORWIDGET_H
#define GEOXCANVASEDITORWIDGET_H

#include <QWidget>

namespace Ui {
class GeoXCanvasEditorWidget;
}

class GeoXCanvasEditorWidget : public QWidget {
   Q_OBJECT

public:
   explicit GeoXCanvasEditorWidget(QWidget *parent = 0);
   ~GeoXCanvasEditorWidget();

private:
   Ui::GeoXCanvasEditorWidget *ui;
};

#endif // GEOXCANVASEDITORWIDGET_H
