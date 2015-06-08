//---------------------------------------------------------------------------
#ifndef MouseControllerH
#define MouseControllerH
//---------------------------------------------------------------------------
#include "MouseButtons.h"
#include "Modifiers.h"
#include "PTypes.h"
//---------------------------------------------------------------------------



class  MouseController { /* INTERFACE */
 public:
   virtual void mouseDown(int32 x, int32 y, MouseButtons buttonsState, Modifiers modifiersState) {UNUSED(x);UNUSED(y);UNUSED(buttonsState);UNUSED(modifiersState);}
   virtual void mouseMoved(int32 x, int32 y)                                                     {UNUSED(x);UNUSED(y);}
   virtual void mouseUp(int32 x, int32 y, MouseButtons buttonsState, Modifiers modifiersState)   {UNUSED(x);UNUSED(y);UNUSED(buttonsState);UNUSED(modifiersState);}
   virtual void mouseWheelRotated(int32 rotatedDelta,  Modifiers modifiersState)                 {UNUSED(rotatedDelta);UNUSED(modifiersState);}
   virtual void areaResize(card32 width, card32 height)                                          {UNUSED(width);UNUSED(height);}
   virtual ~MouseController() {}
};



#endif
