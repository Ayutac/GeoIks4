//---------------------------------------------------------------------------
#include "precompiledHeaders.h"
//---------------------------------------------------------------------------
#include "ElementEditor.h"
//---------------------------------------------------------------------------



ElementEditor::ElementEditor(QWidget * parent, Qt::WindowFlags f)
:QWidget( parent, f )
{
   m_Offset = (pointer_diff)NULL;
}

ElementEditor::~ElementEditor(void)
{
}

void ElementEditor::modified()
{
	emit sigModified();
}

void ElementEditor::sltUpdate()
{
	updateEditor();
}


