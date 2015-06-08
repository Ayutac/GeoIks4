//---------------------------------------------------------------------------
#ifndef PopupWindowH
#define PopupWindowH
//---------------------------------------------------------------------------
#include <QWidget>
//---------------------------------------------------------------------------


class  PopupWindow : public QWidget {
	Q_OBJECT

 private:
	bool idontcare;
	
 protected:
	virtual void focusOutEvent(QFocusEvent* event);
	
 public:
	PopupWindow(QWidget *parent);
	
	void popup(int x, int y);
	void popup(QWidget *belowThis);
};



#endif
