#ifndef GEOXIMAGEVIEWERWIDGET_H
#define GEOXIMAGEVIEWERWIDGET_H

#include "GeoXSingleWidgetViewWidget.h"
#include "DynamicArrayOfStructures2D.hpp"
#include "PopupWindow.h"

#include <QWidget>

#include "ui_IDWImagePanel.h"
#include "ui_IDWMappingPanel.h"
#include "ui_IDWFileIOPanel.h"


// automatically generated as forward declartion to designed UI
namespace Ui { class GeoXImageViewWidget; }



class ImagePopup : public PopupWindow {
	Q_OBJECT
 private slots:
	void on_doubleSpinBoxZoom_valueChanged(double);
 public:
	Ui_IDWImagePanel ui;
	ImagePopup(QWidget *parent);
 signals:
	void changed();
};

// --

class MappingPopup : public PopupWindow {
	Q_OBJECT
 private slots:
	void on_dsbScale_valueChanged(double);
	void on_dsbOffset_valueChanged(double);
	void on_dsbGamma_valueChanged(double);
	void on_cbAuto_stateChanged(int);

 public:
	Ui_IDWMappingPanel ui;
	bool noSignals;

	MappingPopup(QWidget *parent);

 signals:
	void changed();
};

// --

class FileIOPopup : public PopupWindow {
	Q_OBJECT

 private slots:
	void on_pbSaveNow_clicked();

 public:
	Ui_IDWFileIOPanel ui;
	FileIOPopup(QWidget *parent);

 signals:
	void saveNow();
};


// --




/// This is the QT widget of the GeoXImageView.
///
/// The code has been ported from GeoXL 35. So there is a lot of stuff going on...
class GeoXImageViewWidget : public GeoXSingleWidgetViewWidget {
	Q_OBJECT
 private:
	Ui::GeoXImageViewWidget *ui;
	ImagePopup *imgPopup;
	MappingPopup *mapPopup;
	FileIOPopup *filePopup;

	std::string currentChannel;
	bool deactivateChannelChoice;

	Vector3i mapColor(Vector3f col, float32 scale, float32 offset, float32 gamma);
	bool createImage(const DynamicArrayOfStructures2D *aos2d, QImage &img, double &minv, double &maxv);

 private slots:
	void on_pushButtonImage_clicked();
	void on_pushButtonMapping_clicked();
	void on_pushButtonFileIO_clicked();
	void changed();
	void on_cbChannel_currentIndexChanged(int);
	void on_cbChannel_editTextChanged(const QString &text);
	void saveNow();
	void on_pbZoomDouble_clicked();
	void on_pbZoomHalf_clicked();

 public:
   explicit GeoXImageViewWidget(QWidget *parent, Qt::WindowFlags f, GeoXWorkSpace *context, GeoXCanvas *container, string data);

	void displayDAOS2D(const DynamicArrayOfStructures2D *aos, std::string channel = "");
	/// function is not used right now - ported from GeoXL for future extensions
	// void displayQImage(const QImage *img);

	static const card32 PNG = 0;
	static const card32 JPG = 1;
	card32 getLoggingFormat();
	QPixmap getCurrentImage();

	void changeZoom(double factor);
	void changeMapping(double offset, double scale, double gamma);

	// inherited
	bool canHandleCanvas(const MetaClass *canvasType);
	// inherited
	bool canHandleData(const MetaClass *dataType);

	~GeoXImageViewWidget();

};


#endif
