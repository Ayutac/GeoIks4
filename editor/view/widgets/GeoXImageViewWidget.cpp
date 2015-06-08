#include "precompiledHeaders.h"
#include "GeoXImageViewWidget.h"
#include "DynamicArrayOfStructures2D.hpp"
#include "ui_GeoXImageViewWidget.h"
#include "FileDialogs.h"
#include "GeoXWorkSpace.h"
#include "NamedObjectList.h"



ImagePopup::ImagePopup(QWidget *parent) : PopupWindow(parent) {
	ui.setupUi(this);
}
void ImagePopup::on_doubleSpinBoxZoom_valueChanged(double) {
	emit changed();
}



MappingPopup::MappingPopup(QWidget *parent) : PopupWindow(parent) {
	ui.setupUi(this);
	noSignals = false;
}
void MappingPopup::on_dsbScale_valueChanged(double) {
	if (noSignals) return;
	emit changed();
}
void MappingPopup::on_dsbOffset_valueChanged(double) {
	if (noSignals) return;
	emit changed();
}
void MappingPopup::on_dsbGamma_valueChanged(double) {
	if (noSignals) return;
	emit changed();
}
void MappingPopup::on_cbAuto_stateChanged(int){
	if (noSignals) return;
	if (ui.cbAuto->isChecked()) {
		ui.dsbOffset->setEnabled(false);
		ui.dsbScale->setEnabled(false);
	} else {
		ui.dsbOffset->setEnabled(true);
		ui.dsbScale->setEnabled(true);
	}
	emit changed();
}


FileIOPopup::FileIOPopup(QWidget *parent) : PopupWindow(parent) {
	ui.setupUi(this);
}

void FileIOPopup::on_pbSaveNow_clicked() {
	emit saveNow();
}


// --


GeoXImageViewWidget::GeoXImageViewWidget(QWidget *parent, Qt::WindowFlags f, GeoXWorkSpace *context, GeoXCanvas *container, string data) :
   GeoXSingleWidgetViewWidget(parent, f, context, container, data),
	ui(new Ui::GeoXImageViewWidget)
{
	ui->setupUi(this);
	ui->scrollArea->setWidget(ui->labelImage);
	imgPopup = new ImagePopup(this);
	mapPopup = new MappingPopup(this);
	filePopup = new FileIOPopup(this);
	currentChannel = "color";
	deactivateChannelChoice = false;
	QObject::connect(imgPopup, SIGNAL(changed()), this, SLOT(changed()));
	QObject::connect(mapPopup, SIGNAL(changed()), this, SLOT(changed()));
	QObject::connect(filePopup, SIGNAL(saveNow()), this, SLOT(saveNow()));
	changed();
}

void GeoXImageViewWidget::changed() {
	DAOS2D *aos2dImage = (DAOS2D*)context->getData()->getObject(data);
	if (aos2dImage) displayDAOS2D(aos2dImage);
}


void GeoXImageViewWidget::on_pushButtonImage_clicked() {
	imgPopup->popup(ui->pushButtonImage);
}

void GeoXImageViewWidget::on_pushButtonMapping_clicked() {
	mapPopup->popup(ui->pushButtonMapping);
}

void GeoXImageViewWidget::on_pushButtonFileIO_clicked() {
	filePopup->popup(ui->pushButtonFileIO);
}

void GeoXImageViewWidget::on_cbChannel_currentIndexChanged(int) {
	if (deactivateChannelChoice) return;
	currentChannel = qString2STLString(ui->cbChannel->currentText());
	changed();
}
void GeoXImageViewWidget::on_cbChannel_editTextChanged(const QString &text) {
	if (deactivateChannelChoice) return;
	currentChannel = qString2STLString(text);
	changed();
}

//void GeoXImageViewWidget::rethinkLayout() {
//	int h = ui->labelImage->height();
//	if (h > maxHeight) h = maxHeight;
//	ui->scrollArea->setMinimumHeight(h);
//	ui->scrollArea->setMaximumHeight(h);
//}


Vector3i GeoXImageViewWidget::mapColor(Vector3f col, float32 scale, float32 offset, float32 gamma) {
	col *= scale;
	col += makeVector3f(offset,offset,offset);
	if (gamma != 1.0f && gamma > 0) {
		col[0] = pow(col[0], gamma);
		col[1] = pow(col[1], gamma);
		col[2] = pow(col[2], gamma);
	}

	Vector3i icol = makeVector3i((int32)(col[0] * 255.0f), (int32)(col[1] * 255.0f), (int32)(col[2] * 255.0f));

	if (icol[0] < 0) icol[0] = 0; if (icol[0] > 255) icol[0] = 255;
	if (icol[1] < 0) icol[1] = 0; if (icol[1] > 255) icol[1] = 255;
	if (icol[2] < 0) icol[2] = 0; if (icol[2] > 255) icol[2] = 255;

	return icol;
}

bool GeoXImageViewWidget::createImage(const DynamicArrayOfStructures2D *aos2d, QImage &img, double &minv, double &maxv) {
	if (!aos2d->providesAttribute(currentChannel)) {
		return false;
	}

	const float32 scaleParam = mapPopup->ui.dsbScale->value() / 100.0f;
	const float32 offsetParam = mapPopup->ui.dsbOffset->value();
	const float32 gammaParam = 1.0f/max(mapPopup->ui.dsbGamma->value(), 0.01);

	SAD SAD = aos2d->getDescr()->getAttribute(currentChannel);
	AAT aat = aos2d->getAAT(currentChannel);
	int offset = aat.getOffset();
	const int numDispChan = min((card32)3, SAD.numComponents);
	const int w = aos2d->getWidth();
	const int h = aos2d->getHeight();
	img = QImage(w,h, QImage::Format_ARGB32);
	minv = 1e40;
	maxv = -1e40;
	for (int y=0; y<h; y++) {
		for (int x=0; x<w; x++) {
			Vector3f col = makeVector3f(0,0,0);
			switch (SAD.dataFormat) {
				case SAD::DATA_FORMAT_CARD8: {
					card8 *pos = (card8*)(aos2d->getDataPointer(x,y) + offset);
					for (int j=0; j<numDispChan; j++) {
						col[j] = pos[j] / 255.0f;
						if ((double)col[j] < minv) minv = col[j];
						if ((double)col[j] > maxv) maxv = col[j];
					}
					break;
				}
				case SAD::DATA_FORMAT_INT32: {
					int32 *pos = (int32*)(aos2d->getDataPointer(x,y) + offset);
					for (int j=0; j<numDispChan; j++) {
						col[j] = pos[j] / 255.0f;
						if ((double)col[j] < minv) minv = col[j];
						if ((double)col[j] > maxv) maxv = col[j];
					}
					break;
				}
				case SAD::DATA_FORMAT_FLOAT32: {
					float32 *pos = (float32*)(aos2d->getDataPointer(x,y) + offset);
					for (int j=0; j<numDispChan; j++) {
						col[j] = pos[j];
						if ((double)col[j] < minv) minv = col[j];
						if ((double)col[j] > maxv) maxv = col[j];
					}
					break;
				}
			}
			Vector3i iCol = mapColor(col, scaleParam, offsetParam, gammaParam);
			img.setPixel(x,y, qRgb(iCol[0], iCol[1], iCol[2]));
		}
	}
	return true;
}

void GeoXImageViewWidget::displayDAOS2D(const DynamicArrayOfStructures2D *aos, std::string channel) {
	if (channel != "") {
		currentChannel = channel;
	}
	if (aos == NULL) return;
	if (aos->getData() == NULL) return;

	deactivateChannelChoice = true;
	ui->cbChannel->clear();
	const StructureDescriptor *vd = aos->getDescr();
	for (card32 i=0; i<vd->getNumAttributes(); i++) {
		ui->cbChannel->addItem(QString(vd->getAttribute(i).name.c_str()));
	}
	int pos = ui->cbChannel->findText(currentChannel.c_str());
	if (pos != -1) {
		ui->cbChannel->setCurrentIndex(pos);
	} else {
		ui->cbChannel->addItem(currentChannel.c_str());
		ui->cbChannel->setCurrentIndex(ui->cbChannel->count()-1);
	}
	deactivateChannelChoice = false;

	QImage img;
	double minv;
	double maxv;
	bool success = createImage(aos, img, minv, maxv);
	if (mapPopup->ui.cbAuto->isChecked()) {
		mapPopup->noSignals = true;
		double delta = maxv - minv;
		if (delta < 1e-37) delta = 1e-37;
		mapPopup->ui.dsbScale->setValue(100.0 / delta);
		mapPopup->ui.dsbOffset->setValue(-minv/delta);
		success = createImage(aos, img, minv, maxv);
		mapPopup->noSignals = false;
	}
	if (success) {
		QPixmap pix = QPixmap::fromImage(img);
		ui->labelImage->setPixmap(pix);
		int w = (int)(aos->getWidth() * imgPopup->ui.doubleSpinBoxZoom->value() / 100.0);
		int h = (int)(aos->getHeight() * imgPopup->ui.doubleSpinBoxZoom->value() / 100.0);
		if (w < 1) w = 1;
		if (h < 1) h = 1;
		if (w > 10000) w = 10000;
		if (h > 10000) h = 10000;
		ui->labelImage->setScaledContents(true);
		ui->labelImage->resize(w,h);
	} else {
		ui->labelImage->setText(" - channel not found - ");
		ui->labelImage->resize(181, 41);
	}
}

//void GeoXImageViewWidget::displayQImage(const QImage *img) {
//	if (img == NULL) return;
//	if (img->width() <= 0 || img->height() <= 0) {
//		return;
//	}
//	DynamicArrayOfStructures2D aos;
//	StructureDescriptor vd;
//	vd.pushAttrib(SAD("color", 3, SAD::DATA_FORMAT_CARD8));
//	aos.clearAndSetup(img->width(), img->height(), &vd);
//	AAT COLAAT = vd.getToken("color");
//	const int w = img->width();
//	const int h = img->height();
//	for (int y=0; y<h; y++) {
//		for (int x=0; x<w; x++) {
//			QRgb pix = img->pixel(x,y);
//			Vector3ub c = makeVector3ub(qRed(pix), qGreen(pix), qBlue(pix));
//			aos.set<card8, 3>(x,y,COLAAT, c);
//		}
//	}
//	displayDAOS2D(&aos);
//}

card32 GeoXImageViewWidget::getLoggingFormat() {
	if (filePopup->ui.rbJPG->isChecked()) {
		return JPG;
	} else {
		return PNG;
	}
}

QPixmap GeoXImageViewWidget::getCurrentImage() {
	return *(ui->labelImage->pixmap());
}

void GeoXImageViewWidget::saveNow() {
	std::string selectedFilter;
	std::string filename = FileDialogs::getSaveFileName(this, "Save current image as...",
		"*.png;;*.jpg", &selectedFilter, "", FileDialogs::EXPORT_DIR);
	if (filename != "") {
		QPixmap pix = getCurrentImage();
		pix.save(filename.c_str(), (selectedFilter=="*.png")?"PNG":"JPG");
	}
}

void GeoXImageViewWidget::changeZoom(double factor) {
	imgPopup->ui.doubleSpinBoxZoom->setValue(factor*100);
	changed();
}

void GeoXImageViewWidget::changeMapping(double offset, double scale, double gamma) {
	mapPopup->ui.dsbOffset->setValue(offset);
	mapPopup->ui.dsbScale->setValue(scale);
	mapPopup->ui.dsbGamma->setValue(gamma);
	changed();
}

void GeoXImageViewWidget::on_pbZoomDouble_clicked() {
	imgPopup->ui.doubleSpinBoxZoom->setValue(imgPopup->ui.doubleSpinBoxZoom->value()*2.0);
	changed();
}

void GeoXImageViewWidget::on_pbZoomHalf_clicked() {
	imgPopup->ui.doubleSpinBoxZoom->setValue(max(imgPopup->ui.doubleSpinBoxZoom->value()/2.0,1.0));
	changed();
}


GeoXImageViewWidget::~GeoXImageViewWidget() {}























