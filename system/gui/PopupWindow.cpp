//---------------------------------------------------------------------------
#include "precompiledHeaders.h"
//---------------------------------------------------------------------------
#include "PopupWindow.h"
//---------------------------------------------------------------------------
#include <QDesktopWidget>
#include <QApplication>
#include <QFocusEvent>
//---------------------------------------------------------------------------



PopupWindow::PopupWindow(QWidget *parent) : QWidget(parent, Qt::Window) {// | Qt::FramelessWindowHint) {
	setFocus(Qt::OtherFocusReason);
	idontcare = false;
}
	
void PopupWindow::focusOutEvent(QFocusEvent* event) {
	if (event->reason() == Qt::ActiveWindowFocusReason) hide();
}



void PopupWindow::popup(int x, int y) {
   QRect screen = QApplication::desktop()->availableGeometry(this);
   if (x>screen.width()-width()) x -= x+width()-screen.width();
   if (y>screen.height()-height()) y -= y+height()-screen.height();
   if (x<0) x=0;
   if (y<0) y=0;
	move(x,y);
	show();
}

void PopupWindow::popup(QWidget *belowThis) {
	QPoint pos = belowThis->mapToGlobal(QPoint(0,0));
	int x = pos.x();
	int y = pos.y();
	y += belowThis->height();
	popup(x,y);	
}


