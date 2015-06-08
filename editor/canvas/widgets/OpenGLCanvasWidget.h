#ifndef OPENGLCANVASWIDGET_H
#define OPENGLCANVASWIDGET_H

#include "GeoXCanvasWidget.h"
#include "ui_OpenGLCanvas.h"

#include <QWidget>

class OpenGLCanvas;

class OpenGLCanvasWidget : public GeoXCanvasWidget
{
   Q_OBJECT
 private:
   Ui_OpenGLCanvas ui;
 private slots:
   void on_btnResetCamera_pressed();
   void on_btnOrtho_clicked(bool b);
   void on_btnDrawAxes_clicked(bool b);
   void on_btnYAxis_clicked(bool b);
 friend class OpenGLCanvas;

 public:
   explicit OpenGLCanvasWidget(GeoXCanvas *canvas, QWidget *parent = 0, Qt::WindowFlags f = 0);

   int getGLWidgetWidth() {return ui.glFrame->width();}
   int getGLWidgetHeight() {return ui.glFrame->height();}
   bool getDrawAxis() {return ui.btnDrawAxes->isChecked();}
   void repaint() {ui.glFrame->repaint();}
   void set2DInterface(bool mode2D);
	QGLWidget *getGLWidget() {return ui.glFrame;}

   ~OpenGLCanvasWidget();

};

#endif // OPENGLCANVASWIDGET_H
