#include "precompiledHeaders.h"
#include "OpenGLCanvasWidget.h"
#include "OpenGLCanvas.h"
#include "OpenGLCanvas2D.h"

OpenGLCanvasWidget::OpenGLCanvasWidget(GeoXCanvas *canvas, QWidget *parent, Qt::WindowFlags f)
   : GeoXCanvasWidget(canvas, parent, f)
{
   ui.setupUi(this);
   ui.glFrame->setup((OpenGLCanvas*)canvas);
	// evil hack
	if (dynamic_cast<OpenGLCanvas2D*>(canvas)) ui.btnDrawAxes->setChecked(false);
}



OpenGLCanvasWidget::~OpenGLCanvasWidget()
{

}

void OpenGLCanvasWidget::on_btnResetCamera_pressed()
{
   ((OpenGLCanvas*)canvas)->on_btnResetCamera_pressed();
}

void OpenGLCanvasWidget::on_btnOrtho_clicked(bool b)
{
   ((OpenGLCanvas*)canvas)->on_btnOrtho_clicked(b);
}

void OpenGLCanvasWidget::on_btnDrawAxes_clicked(bool b)
{
   ((OpenGLCanvas*)canvas)->on_btnDrawAxes_clicked(b);
}

void OpenGLCanvasWidget::on_btnYAxis_clicked(bool b)
{
   ((OpenGLCanvas*)canvas)->on_btnYAxis_clicked(b);
}

void OpenGLCanvasWidget::set2DInterface(bool mode2D)
{
   ui.btnOrtho->setVisible(!mode2D);
   ui.btnOrtho->setEnabled(!mode2D);
   ui.btnYAxis->setVisible(!mode2D);
   ui.btnYAxis->setEnabled(!mode2D);
}
