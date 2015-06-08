#ifndef ObjectBrowserViewMDISubWindowH
#define ObjectBrowserViewMDISubWindowH

#include <QMdiSubWindow>
#include <string>
#include <QCloseEvent>



class GeoXCanvas;
class GeoXCanvasWidget;

/// This class represents the MDI windows that contain GeoXCanvases
class MDICanvasContainer : public QMdiSubWindow {
   Q_OBJECT

 private:
   GeoXCanvasWidget *canvasWidget;
   std::string name;

 protected:
   void closeEvent(QCloseEvent *event);

 signals:
   void canvasClosed(QString name, QWidget *sender);

 public:
   /// constructor, and this one does the setup in one go
   MDICanvasContainer(QWidget *parent, Qt::WindowFlags f = 0);
   /// connect to canvas (call after constructor! integration in constructor is not possible for technical reasons.)
   void setup(GeoXCanvasWidget *canvasWidget, std::string name, std::string windowTitle);
};




#endif
