#include "precompiledHeaders.h"

#include "RaytracingMaterial.h"
#include "FixedArrayClassProperty.h"


IMPLEMENT_GEOX_CLASS(RaytracingMaterial, 0) {
	BEGIN_CLASS_INIT(RaytracingMaterial)
	ADD_VECTOR3F_PROP(diffuseColor, 0)
	ADD_VECTOR3F_PROP(specularColor, 0)
	ADD_VECTOR3F_PROP(ambientColor, 0)
	ADD_FLOAT32_PROP(reflectivity, 0)
	ADD_FLOAT32_PROP(specularExponent, 0)
}




RaytracingMaterial::RaytracingMaterial()
{
	diffuseColor = makeVector3f(0.3f,0.2f,0.8f);
	specularColor = makeVector3f(0.8f,0.9f,0.8f);
	ambientColor = makeVector3f(0.3f,0.3f,0.35f);
	reflectivity = 0;
	specularExponent = 20;
}
