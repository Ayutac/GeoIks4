//---------------------------------------------------------------------------
#include "precompiledHeaders.h"
//---------------------------------------------------------------------------
#include "SingleClassProperty.h"
//---------------------------------------------------------------------------



SingleClassProperty::SingleClassProperty(const string cName, const Version introducedInVersion)
:ClassProperty( cName, introducedInVersion )
{
   m_Offset = (pointer_diff)NULL;
}

SingleClassProperty::~SingleClassProperty(void)
{
}
