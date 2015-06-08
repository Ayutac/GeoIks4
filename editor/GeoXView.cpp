#include "precompiledHeaders.h"
#include "GeoXView.h"

IMPLEMENT_GEOX_ABSTRACT_CLASS(GeoXView, 0) {
   BEGIN_CLASS_INIT(GeoXView)
}


GeoXView::GeoXView()
{
   context = NULL;
   container = NULL;
}

void GeoXView::setup(GeoXWorkSpace *context, GeoXCanvas *container, string data)
{
   this->context = context;
   this->container = container;
   this->data = data;
}
