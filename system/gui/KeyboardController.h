//---------------------------------------------------------------------------
#ifndef KeyboardControllerH
#define KeyboardControllerH
//---------------------------------------------------------------------------
#include "GeneralKey.h"
#include "Tools.h"
//---------------------------------------------------------------------------

#ifdef _MSC_VER
   #pragma warning(disable:4100)
#endif


class KeyboardController { /* INTERFACE */
 public:
   virtual void keyDown(GeneralKey key) {UNUSED(key);}
   virtual void keyUp(GeneralKey key)   {UNUSED(key);}
    virtual ~KeyboardController()       {}
};


#endif
