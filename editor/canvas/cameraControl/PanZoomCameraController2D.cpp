//---------------------------------------------------------------------------
#include "precompiledHeaders.h"
//---------------------------------------------------------------------------
#include "PanZoomCameraController2D.h"
//---------------------------------------------------------------------------
#include "FixedArrayClassProperty.h"
#include "NumericalClassProperty.h"
#include "BooleanClassProperty.h"
#include "ObjectViewsTable.h"
#include "LinearAlgebra.h"
#include "LinearAlgebra.hpp"
#include "MathTools.h"
#include "MathTools.hpp"
//---------------------------------------------------------------------------

IMPLEMENT_GEOX_CLASS(PanZoomCameraController2D, 0) {
   BEGIN_CLASS_INIT(PanZoomCameraController2D)
   ADD_VECTOR2F_PROP( center, 0 );
   ADD_FLOAT32_PROP( zoom, 0 );
}

void PanZoomCameraController2D::calcCamera() {
   Vector3f camView = makeVector3f(0,0,1);
   Vector3f camUp = makeVector3f(0,1,0);
   Vector3f camPos = makeVector3f(0,0,-10);

   Vector3f transfCamPos = camPos + makeVector3f(center[0], center[1], 0);
   Vector3f lookAt =  (camPos + camView) + makeVector3f(center[0], center[1], 0);
   Vector3f transfCamUp =  camUp;

   camera->setup(transfCamPos, lookAt, transfCamUp);
}

void PanZoomCameraController2D::changed() {
   if (camera == NULL) return;
   ObjectViewsTable::update(camera);
   calcCamera();
   cameraChange();
}


// ----


PanZoomCameraController2D::PanZoomCameraController2D() {
   mouseAreaWidth = 500;
   mouseAreaHeight = 500;

   state = neutral;

   center = makeVector2f(0.0, 0.0);
   zoom = 0.9f;
	changed();
}

// ----


// ----

void PanZoomCameraController2D::mouseDown(int32 x, int32 y, MouseButtons buttonsState, Modifiers modifiersState) {
   UNUSED(modifiersState);
   lastX = x;
   lastY = y;
   if (buttonsState.getLeft()) {
      state = dragCenter;
   }  else {
      state = dragZoom;
   }
}

void PanZoomCameraController2D::mouseMoved(int32 x, int32 y) {
   if (mouseAreaHeight <= 0) return;
   int32 deltaX = x - lastX;
   int32 deltaY = y - lastY;
   lastX = x;
   lastY = y;
   if (state == dragZoom) {
      float32 normDy = pow (2.0f, - 5.0f * (float32)deltaY / (float32)mouseAreaHeight);
      zoom *= normDy;
      changed();
   }
	else if (state == dragCenter) {
      static const Vector2f RIGHT_VEC = makeVector2f(2,0);
      static const Vector2f UP_VEC = makeVector2f(0,2);
      float32 scale = zoom / (mouseAreaHeight);
      center += RIGHT_VEC * deltaX * scale;
      center +=  UP_VEC * deltaY * scale;
		changed();
	}
   
}

void PanZoomCameraController2D::mouseUp(int32 x, int32 y, MouseButtons buttonsState, Modifiers modifiersState) {
   UNUSED(x);UNUSED(y);UNUSED(buttonsState);UNUSED(modifiersState);
   state = neutral;
}

void PanZoomCameraController2D::mouseWheelRotated( int32 rotatedDelta, Modifiers modifiersState )
{
   UNUSED(modifiersState);
   static const float ROTATE_FACTOR = (float)pow(2.0,(1.0/8.0));
   if (rotatedDelta>0)
   {
      //Zoom in
      zoom /= ROTATE_FACTOR;
      changed();
   } else {
      // Zoom out
      zoom *= ROTATE_FACTOR;
      changed();
   }
}


void PanZoomCameraController2D::areaResize(card32 width, card32 height) {
   this->mouseAreaWidth = width;
   this->mouseAreaHeight = height;
}

// ----

void PanZoomCameraController2D::update() {}

void PanZoomCameraController2D::apply() {
   ObjectViewsTable::update(camera);
   calcCamera();
   cameraChange();
}

void PanZoomCameraController2D::reset()
{
   center = makeVector2f(0.0, 0.0);
   zoom = 0.9f;
   calcCamera();
   cameraChange();
}



