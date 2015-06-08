//---------------------------------------------------------------------------
#ifndef TimerH
#define TimerH
//---------------------------------------------------------------------------
#include "PTypes.h"
#include <QElapsedTimer>
//---------------------------------------------------------------------------



class  Timer {
 private:
	 QElapsedTimer timer;
	 card64 lastTime;

 public:
   Timer();
	card64 getCurrentValue();
	card64 getDeltaValue(card32 lastValue);
	card64 getDeltaValue();
   ~Timer();
};



#endif
