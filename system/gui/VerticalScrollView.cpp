//---------------------------------------------------------------------------
#include "precompiledHeaders.h"
//---------------------------------------------------------------------------
#include "VerticalScrollView.h"
#include "Tools.h"
//---------------------------------------------------------------------------



VerticalScrollView::VerticalScrollView(QWidget * parent)
{
	UNUSED(parent);
	m_Content = NULL;
	setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
	setWidgetResizable(true);
}

void VerticalScrollView::resizeEvent( QResizeEvent * e ) 
{
	m_Content = viewport()->childAt(1,1);
	if( m_Content == NULL )
		return;

	int content_height = m_Content->height();
	int scrollbarWidth = 20;

	if( content_height+3 > height() )
	{
		m_Content->resize( width() - scrollbarWidth, content_height );
	}
	else
	{
		m_Content->resize( width(), content_height );
	}
	QScrollArea::resizeEvent(e);
}


