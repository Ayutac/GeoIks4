//---------------------------------------------------------------------------
#ifndef GeoXOutputH
#define GeoXOutputH
//---------------------------------------------------------------------------
#include <string>
#include "LinearAlgebra.h"
#include "DynamicLinearAlgebra.h"
#include "BoundingBox.h"
#include "SparseLinearAlgebra.h"
#include "StringListener.h"
//---------------------------------------------------------------------------

using namespace std;


class MetaClass;


/// This class provides a simple string output. The "<<" methods are self-explanatory.
/// Upon startup, the class is connected to the main window.
/// Further "listeners" can be registered if needed (advanced use).
///
class GeoXOutput {
 private:
   vector<StringListener*> listeners;

 protected:
   void outputString(const string &s);

 public:
   GeoXOutput& operator<<(bool b);
   GeoXOutput& operator<<(int i);
   GeoXOutput& operator<<(unsigned int i);
   // this is necessary for 64-bit builds
   GeoXOutput& operator<<(int64 i);
   GeoXOutput& operator<<(card64 i);
   GeoXOutput& operator<<(long double d);
   GeoXOutput& operator<<(float f);
   GeoXOutput& operator<<(double d);
   GeoXOutput& operator<<(const string& s);
   GeoXOutput& operator<<(const char* s);
   GeoXOutput& operator<<(const Vector2f &v);
   GeoXOutput& operator<<(const Vector3f &v);
   GeoXOutput& operator<<(const Vector4f &v);
   GeoXOutput& operator<<(const Vector6f &v);
   GeoXOutput& operator<<(const Vector3i &v);
   GeoXOutput& operator<<(const Matrix4f &m);
   GeoXOutput& operator<<(const Matrix3f &m);
   GeoXOutput& operator<<(const BoundingBox3f &m);
   GeoXOutput& operator<<(const StaticVector<float32,7> &v);
   GeoXOutput& operator<<(const DynamicVector<float> &m);
   GeoXOutput& operator<<(const DynamicMatrix<float> &m);
   GeoXOutput& operator<<(SparseMatrixF &A);

   /// write a string with identation (@param ident refers to the number of spaces)
   void writeIdented(unsigned ident, string s);
   /// output a "return"
   void cr();

   /// This method registers a new output window, logger, or whatever.
   /// Usually, you do not need to call this directly.
   void addListener(StringListener *listener);

   /// Remove an output window (when it is closed / output device becomes unavailable)
   void removeListener(StringListener *listener);
};

extern  GeoXOutput output;

/// printout warning message on debug console, pop-up window if non-visible.
void  error(const string& msg);

/// printout error message on debug console, pop-up window if non-visible.
void  warning(const string& msg);

/// printout message on debug console, pop-up window if non-visible.
void  message(const string& msg);



#endif
