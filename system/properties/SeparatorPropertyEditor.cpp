//---------------------------------------------------------------------------
#include "precompiledHeaders.h"
//---------------------------------------------------------------------------
#include "SeparatorPropertyEditor.h"
//---------------------------------------------------------------------------
#include "Tools.h"
//---------------------------------------------------------------------------
#include <QBoxLayout>
#include <QWidget>
#include <QFrame>
//---------------------------------------------------------------------------


IMPLEMENT_GEOX_CLASS( SeparatorPropertyEditor, 0 )
{
}


QWidget *SeparatorPropertyEditor::makeEditor(QWidget * parent, bool readOnly) {
	UNUSED(readOnly);
	QWidget *result = new MyElementEditor(parent);
	QVBoxLayout *layout = new QVBoxLayout();
	layout->setSpacing(2);
	layout->setMargin(3);
	QFrame *hline1 = new QFrame(result);
	hline1->setFrameStyle(QFrame::HLine | QFrame::Raised);
	layout->addWidget(hline1); 
	QFrame *hline2 = new QFrame(result);
	hline2->setFrameStyle(QFrame::HLine | QFrame::Raised);
	layout->addWidget(hline2); 
	result->setLayout(layout);
	return result;
}

card32 SeparatorPropertyEditor::getPreferredHeight() {
	return 14;
}

