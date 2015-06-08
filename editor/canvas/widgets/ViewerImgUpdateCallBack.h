//---------------------------------------------------------------------------
#ifndef ViewerImgUpdateCallBackH
#define ViewerImgUpdateCallBackH
//---------------------------------------------------------------------------
#include <qimage.h>
//---------------------------------------------------------------------------

class ViewerImgUpdateCallBack {
 public:
	virtual void viewImage(QImage &currentImg) = 0;
};


#endif