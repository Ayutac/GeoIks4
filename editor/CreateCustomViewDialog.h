#ifndef CREATECUSTOMVIEWDIALOG_H
#define CREATECUSTOMVIEWDIALOG_H

#include <QDialog>

namespace Ui { class CreateCustomViewDialog; }

class GeoXWorkSpace;
class GeoXCanvas;
class MDICanvasContainer;

class CreateCustomViewDialog : public QDialog {
	Q_OBJECT
 private:
	Ui::CreateCustomViewDialog *ui;
	QWidget *canvasParent;
	GeoXWorkSpace *tmpWorkSpace;

	void checkView();

 private slots:
	void on_pbCreate_clicked();
	void on_pbCancel_clicked();
	void on_cbData_currentIndexChanged(int);
	void on_cbExistingCanvas_currentIndexChanged(int);
	void on_cbNewCanvas_currentIndexChanged(int);
	void on_cbView_currentIndexChanged(int);

 protected:
	void rebuild(GeoXWorkSpace *workSpace);

 public:
	explicit CreateCustomViewDialog(QWidget *parent, QWidget *canvasParent);

	/// this function returns a new cavas and its container if it was created internally
	/// (for wiring it up in the editor)
	virtual void showDialog(GeoXWorkSpace *workSpace, GeoXCanvas *&newCanvas, MDICanvasContainer *&newWin);

	~CreateCustomViewDialog();

};

#endif // CREATECUSTOMVIEWDIALOG_H
