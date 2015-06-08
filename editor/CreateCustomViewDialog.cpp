#include "precompiledHeaders.h"
#include "CreateCustomViewDialog.h"
#include "GeoXWorkSpace.h"
#include "GeoXCanvas.h"
#include "GeoXView.h"
#include "GeoXData.h"
#include "NamedObjectList.h"
#include "MDICanvasContainer.h"
#include "ExceptionHandling.h"
#include "GeoXOutput.h"

#include "ui_CreateCustomViewDialog.h"

void CreateCustomViewDialog::on_pbCreate_clicked()
{
	accept();
}

void CreateCustomViewDialog::on_pbCancel_clicked()
{
	reject();
}

void CreateCustomViewDialog::on_cbData_currentIndexChanged(int)
{
	int dataIndex = ui->cbData->currentIndex();
	if (dataIndex > -1) {
		string dataName = qString2STLString(ui->cbData->itemText(dataIndex));
		GeoXData *data = (GeoXData*)tmpWorkSpace->getData()->getObject(dataName);
		const MetaClass *mcv = data->getDefaultViewer();
		const MetaClass *mcc = data->getDefaultViewerDefaultCanvas();
		ui->cbView->setSelection(mcv);
		ui->cbNewCanvas->setSelection(mcc);
	}
	checkView();
}

void CreateCustomViewDialog::on_cbExistingCanvas_currentIndexChanged(int)
{
	checkView();
}

void CreateCustomViewDialog::on_cbNewCanvas_currentIndexChanged(int)
{
	checkView();
}

void CreateCustomViewDialog::on_cbView_currentIndexChanged(int)
{
	const MetaClass *mc = ui->cbView->getSelected();
	if (mc) {
		GeoXView *testView = (GeoXView*)mc->newInstance();
		const MetaClass *canvasType = testView->preferredCanvasType();
		delete testView;
		ui->cbNewCanvas->setSelection(canvasType);
	}
	checkView();
}

void CreateCustomViewDialog::checkView()
{
	STD_GLOBAL_EXCEPTION_HANDLER_START
	if (!tmpWorkSpace) return;
	const MetaClass *newViewMetaClass = ui->cbView->getSelected();
	GeoXView *testView = NULL;
	bool dataMatches = true;
	bool canvasMatches = true;
	if (newViewMetaClass) {
		testView = dynamic_cast<GeoXView*>(newViewMetaClass->newInstance());

		int dataIndex = ui->cbData->currentIndex();
		if (dataIndex > -1) {
			string dataName = qString2STLString(ui->cbData->itemText(dataIndex));
			GeoXData *data = (GeoXData*)tmpWorkSpace->getData()->getObject(dataName);
			dataMatches = testView->canHandleData(data->getInstanceClass());
		}
		if (ui->cbExisting->isChecked()) {
			int newCanvasIndex = ui->cbNewCanvas->currentIndex();
			if (newCanvasIndex != -1) {
				string canvasName = qString2STLString(ui->cbData->itemText(newCanvasIndex));
				GeoXCanvas *canvas = (GeoXCanvas*)tmpWorkSpace->getCanvases()->getObject(canvasName);
				canvasMatches = testView->canHandleCanvas(canvas->getInstanceClass());
			}
		} else {
			const MetaClass *newCanvasMetaClass = ui->cbNewCanvas->getSelected();
			if (newCanvasMetaClass) {
				canvasMatches = testView->canHandleCanvas(newCanvasMetaClass);
			}
		}
		delete testView;
	}
	if (!dataMatches && canvasMatches) {
		ui->lbHint->setText("Data incompatible with view.");
		ui->lbHintFront->setText("Error: ");
	}
	if (dataMatches && !canvasMatches) {
		ui->lbHint->setText("Canvas incompatible with view.");
		ui->lbHintFront->setText("Error: ");
	}
	if (!dataMatches && !canvasMatches) {
		ui->lbHint->setText("Data and canvas incompatible with view.");
		ui->lbHintFront->setText("Error: ");
	}
	if (dataMatches && canvasMatches) {
		ui->lbHint->setText("Data, cavas, view combination match.");
		ui->lbHintFront->setText("OK: ");
	}
	if (!dataMatches || !canvasMatches) {
		ui->pbCreate->setEnabled(false);
	} else {
		ui->pbCreate->setEnabled(true);
	}
	STD_GLOBAL_EXCEPTION_HANDLER_END
}

void CreateCustomViewDialog::rebuild(GeoXWorkSpace *workSpace)
{
	QString msg;
	ui->cbData->clear();
	mpcard numData = workSpace->getData()->getSize();
	for (mpcard i=0; i<numData; i++) {
		ui->cbData->addItem(workSpace->getData()->getName(i).c_str());
	}
	if (numData == 0) {
		msg = "No data object found!";
		ui->cbData->setEnabled(false);
		ui->gbExistingCanvas->setEnabled(false);
		ui->gbNewCanvas->setEnabled(false);
		ui->cbView->setEnabled(false);
		ui->pbCreate->setEnabled(false);
	} else {
		ui->cbNew->setChecked(true);
		ui->cbExisting->setChecked(false);
		ui->cbData->setEnabled(true);
      ui->gbExistingCanvas->setEnabled(true);
		ui->gbNewCanvas->setEnabled(true);
		ui->cbView->setEnabled(true);
		ui->pbCreate->setEnabled(true);
	}
	ui->lbHint->setText(msg);
	if (msg != "") {
		ui->lbHintFront->setText("Error: ");
	} else {
		ui->lbHintFront->setText("");
		checkView();
	}
}

CreateCustomViewDialog::CreateCustomViewDialog(QWidget *parent, QWidget *canvasParent) :
	QDialog(parent),
	ui(new Ui::CreateCustomViewDialog)
{
	tmpWorkSpace = NULL;
	ui->setupUi(this);
	this->canvasParent = canvasParent;
	ui->cbNewCanvas->collectClasses("GeoXCanvas", true);
	ui->cbView->collectClasses("GeoXView", true);
}

CreateCustomViewDialog::~CreateCustomViewDialog()
{
	delete ui;
}

void CreateCustomViewDialog::showDialog(GeoXWorkSpace *workSpace, GeoXCanvas *&newCanvas, MDICanvasContainer *&newWin)
{
	STD_GLOBAL_EXCEPTION_HANDLER_START
	newCanvas = NULL;
	newWin = NULL;
	tmpWorkSpace = workSpace;
	rebuild(workSpace);
	try {
		if (exec() == QDialog::Accepted) {
			int dataIndex = ui->cbData->currentIndex();
			if (dataIndex > -1) {
				string dataName = qString2STLString(ui->cbData->itemText(dataIndex));
            //GeoXData *data = (GeoXData*)workSpace->getData()->getObject(dataName);
				GeoXCanvas *canvas = NULL;
				int newCanvasIndex = ui->cbNewCanvas->currentIndex();
				if (newCanvasIndex == -1) {
					warning("no canvas selected - aborting");
					return;
				}
				if (ui->cbExisting->isChecked()) {
					string canvasName = qString2STLString(ui->cbData->itemText(newCanvasIndex));
					canvas = (GeoXCanvas*)workSpace->getCanvases()->getObject(canvasName);
				} else {
					const MetaClass *newCanvasMetaClass = ui->cbNewCanvas->getSelected();
					if (newCanvasMetaClass) {
						canvas = dynamic_cast<GeoXCanvas*>(newCanvasMetaClass->newInstance());
						string canvasName = string("new_")+newCanvasMetaClass->getClassName();
						workSpace->getCanvases()->addObject(canvas, canvasName);
						MDICanvasContainer *win = new MDICanvasContainer(canvasParent);
						canvas->setup(win);
						string windowTitle = canvasName;
						win->setup(canvas->getWidget(), canvasName, windowTitle);
						win->show();
						newCanvas = canvas;
						newWin = win;
					}
				}
				const MetaClass *newViewMetaClass = ui->cbView->getSelected();
				GeoXView *view = NULL;
				if (newViewMetaClass) {
					view = dynamic_cast<GeoXView*>(newViewMetaClass->newInstance());
				}
				if (view == NULL) {
					warning("could not create view - aborted.");
					if (newCanvas) {delete newCanvas; newCanvas = NULL; newWin = NULL;}
					return;
				}
				if (canvas) {
					view->setup(workSpace, canvas, dataName);
					string canvasName = string("new_") + canvas->getInstanceClass()->getClassName();
					string viewName = dataName + string("_[")+ newViewMetaClass->getClassName() + string("]");
					canvas->addView(view, viewName);
				} else {
					warning("Something went very wrong: the canvas was not found.");
					return;
				}
			}
		}
	} catch (...) {
		tmpWorkSpace = NULL;
		throw;
	}
	tmpWorkSpace = NULL;

	STD_GLOBAL_EXCEPTION_HANDLER_END
}

