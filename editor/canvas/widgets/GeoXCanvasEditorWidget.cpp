#include "GeoXCanvasEditorWidget.h"
#include "ui_GeoXCanvasEditorWidget.h"

GeoXCanvasEditorWidget::GeoXCanvasEditorWidget(QWidget *parent) :
   QWidget(parent),
   ui(new Ui::GeoXCanvasEditorWidget)
{
   ui->setupUi(this);
}

GeoXCanvasEditorWidget::~GeoXCanvasEditorWidget()
{
   delete ui;
}
