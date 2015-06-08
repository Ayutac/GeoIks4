#include "precompiledHeaders.h"
#include "GeoXTextViewWidget.h"
#include "ui_GeoXTextViewWidget.h"

#include "GeoXWorkSpace.h"
#include "GeoXCanvas.h"
#include "NamedObjectList.h"
#include "GeoXData.h"
#include "DynamicArrayOfStructures.hpp"
#include <QMessageBox>
#include "GeoXOutput.h"

GeoXTextViewWidget::GeoXTextViewWidget(QWidget *parent, Qt::WindowFlags f, GeoXWorkSpace *context, GeoXCanvas *container, string data) :
   GeoXSingleWidgetViewWidget(parent, f, context, container, data),
	ui(new Ui::GeoXTextViewWidget)
{
	ui->setupUi(this);
	changed = false;
	ui->leDataName->setText(data.c_str());
	ui->teData->setTabStopWidth(100);
	//SyntaxHighlighter *highlighter = new SyntaxHighlighter( ui->teData );
}


GeoXTextViewWidget::~GeoXTextViewWidget()
{
	delete ui;
}


void GeoXTextViewWidget::on_pbDump_clicked()
{
	GeoXData *dataPtr = (GeoXData*)context->getData()->getObject(data);
	DynamicArrayOfStructures *aos = dynamic_cast<DynamicArrayOfStructures*>(dataPtr);
	if (aos) {
		bool goAhead = true;
		if (changed) {
			if (QMessageBox::question(this, "Confirm Changes...",
											  "The text buffer has been changed and not stored back yet;"
											  "do you want to proceed and overwrite the content with the "
											  "text representation of the data object?",
											  QMessageBox::Ok | QMessageBox::Cancel , QMessageBox::Cancel)
				 != QMessageBox::Ok)
			{
				goAhead = false;
			}
		}
		mpcard mem = aos->getNumEntries()*aos->getElementSize();
		if (mem >= 100*1024*1024) {
			goAhead = (QMessageBox::question(this, "Size Warning...",
														"Attention: The data object you are viewing is larger than 100MB. "
														"The operation might take a while. Continue?",
														QMessageBox::Ok | QMessageBox::Cancel , QMessageBox::Cancel)
						  == QMessageBox::Ok);
		}
		if (goAhead) {
			ui->teData->clear();
			string leText = aos->toString();
			ui->teData->setText(leText.c_str());
		}
	} else {
		error("Sorry, right now I can only handle objects of type 'DynamicArrayOfStructures'.");
	}
}

void GeoXTextViewWidget::on_pbReRead_clicked()
{
	//changed = false;
	warning("GeoXTextViewWidget::on_pbReRead_clicked() -- not implemented yet (nothing happend).");
}

void GeoXTextViewWidget::on_teData_textChanged()
{
	changed = true;
}


/*void GeoXTextViewWidget::SyntaxHighlighter::highlightBlock ( const QString & text )
{
	QTextCharFormat multiLineCommentFormat;
	multiLineCommentFormat.setForeground(Qt::darkGray);

	QTextCharFormat keywordFormat;
	keywordFormat.setForeground( Qt::darkRed );
	keywordFormat.setFontWeight(QFont::Bold);

	QTextCharFormat numberFormat;
	numberFormat.setForeground( Qt::darkBlue );


	QRegExp keywords("\\b("
		"(card8|card16|card32|card64|int8|int16|int32|int64|float32|float64)"
		")\\b");

	//QRegExp numberExp("\\b("
	//	"[0123456789.,-+]+"
	//	")\\b");
	QRegExp numberExp("\\s[-+]?[0-9]*\\.?[0-9]+([eE][-+]?[0-9]+)?\\s");

//	setCurrentBlockState(0);

//	int startIndex = 0;
//	if (previousBlockState() != 1)
//	  startIndex = text.indexOf(startExpression);

//	while (startIndex >= 0)
//	{
//		int endIndex = text.indexOf(endExpression, startIndex);
//		int commentLength;
//		if (endIndex == -1)
//		{
//			setCurrentBlockState(1);
//			commentLength = text.length() - startIndex;
//		}
//		else
//		{
//			commentLength = endIndex - startIndex + endExpression.matchedLength();
//		}
//		setFormat(startIndex, commentLength, multiLineCommentFormat);
//		startIndex = text.indexOf(startExpression,startIndex + commentLength);
//	}


	std::string stlText = text.toLatin1().constData();
	int begin = (int)stlText.find("#");
	while( begin >= 0 )
	{
		int end = (int)stlText.find( "\n", begin );
		if( end == -1 )
		{
			setFormat( begin, (int)stlText.length() - begin, multiLineCommentFormat );
			begin = -1;
		}
		else
		{
			setFormat( begin, end - begin, multiLineCommentFormat );
			begin = (int)stlText.find( "#",end);
		}
	}

	int index = text.indexOf(keywords);
	while (index >= 0)
	{
		int length = keywords.matchedLength();
		if( format( index ) != multiLineCommentFormat )
			setFormat(index, length, keywordFormat);
		index = text.indexOf(keywords, index + length);
	}

	index = text.indexOf(numberExp);
	while (index >= 0)
	{
		int length = numberExp.matchedLength();
		if( format( index ) != multiLineCommentFormat )
			setFormat(index, length, numberFormat);
		index = text.indexOf(numberExp, index + length);
	}

}
*/


