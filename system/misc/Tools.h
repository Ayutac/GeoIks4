//---------------------------------------------------------------------------
#ifndef ToolsH
#define ToolsH
//---------------------------------------------------------------------------
#include <string>
//---------------------------------------------------------------------------


using namespace std;

class Object;

//#if defined(_MSC_VER)
//   #ifdef _DEBUG
//		template <class T>
//		void UNUSED(T) {}
//	#else
//		#pragma warning(disable:4101)
//      #pragma warning(disable:4100)
//      #define UNUSED(x)
//	#endif
//#else
	#define UNUSED(x) ((void)(x))
//#endif

extern void  notImplemented();

string  makeObjectDescription(const Object *obj, bool shortDescr = false);

extern  bool geoxFileExists( const char * filename );


#endif

