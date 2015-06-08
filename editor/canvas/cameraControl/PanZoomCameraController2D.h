//---------------------------------------------------------------------------
#ifndef PanZoomCameraController2DH
#define PanZoomCameraController2DH
//---------------------------------------------------------------------------
#include "CameraController.h"
//---------------------------------------------------------------------------




class  PanZoomCameraController2D : public CameraController {
   GEOX_CLASS(PanZoomCameraController2D)
 private:
  enum MouseState {neutral, dragZoom, dragCenter};

   MouseState state;
   int32 lastX;
   int32 lastY;

   card32 mouseAreaWidth;
   card32 mouseAreaHeight;

   Vector2f center;
   float32 zoom;

 protected:
   void changed();
   void calcCamera();

 public:
   PanZoomCameraController2D();

   // KeyboardController:
   virtual void keyDown(GeneralKey key) {UNUSED(key);}
   virtual void keyUp(GeneralKey key) {UNUSED(key);}

   // MouseController:
   virtual void mouseDown(int32 x, int32 y, MouseButtons buttonsState, Modifiers modifiersState);
   virtual void mouseMoved(int32 x, int32 y);
   virtual void mouseUp(int32 x, int32 y, MouseButtons buttonsState, Modifiers modifiersState);
   virtual void areaResize(card32 width, card32 height);
	virtual void mouseWheelRotated(int32 rotatedDelta,  Modifiers modifiersState);

   // Editor:
   virtual void update();
   virtual void apply();
   virtual void setup( const Vector3f& position, const Vector3f& ref, const Vector3f& up ) {UNUSED(position);UNUSED(ref);UNUSED(up);}

   // Properties:
   float32 getZoom() const {return zoom;}
   void setZoom(float32 zoom) {this->zoom = zoom;}

   Vector2f getCenter() const {return center;}
   void setCenter(const Vector2f center) {this->center = center;}

   /// set to constructor defaults
   void reset();
};



#endif
