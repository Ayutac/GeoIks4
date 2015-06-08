#ifndef GEOXTEXTVIEWWIDGET_H
#define GEOXTEXTVIEWWIDGET_H

#include "GeoXSingleWidgetViewWidget.h"

#include <QWidget>
#include <QSyntaxHighlighter>

namespace Ui {
class GeoXTextViewWidget;
}

class QTextDocument;

/// Text viewer (this part is the QT Widget of the GeoXTextView).
/// It displays the content of GeoXData objects as text.
///
/// Limitations: Currently, only objects of type "DynamicArrayOfStructures" are supported.
///
class GeoXTextViewWidget : public GeoXSingleWidgetViewWidget {
	Q_OBJECT

 private:
	Ui::GeoXTextViewWidget *ui;
	/// stores whether the text editor content has been modified
	bool changed;

//	class SyntaxHighlighter : public QSyntaxHighlighter {
//	 public:
//		explicit SyntaxHighlighter(QTextDocument * parent) : QSyntaxHighlighter(parent) {}
//		explicit SyntaxHighlighter(QObject * parent) : QSyntaxHighlighter(parent) {}

//	 protected:
//		virtual void highlightBlock ( const QString & text );
//	};


 private slots:
	void on_pbDump_clicked();
	void on_pbReRead_clicked();
	void on_teData_textChanged();

 public:
    explicit GeoXTextViewWidget(QWidget *parent, Qt::WindowFlags f, GeoXWorkSpace *context, GeoXCanvas *container, std::string data);

	~GeoXTextViewWidget();
};

#endif // GEOXTEXTVIEWWIDGET_H
