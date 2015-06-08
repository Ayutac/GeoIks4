//---------------------------------------------------------------------------
#include "precompiledHeaders.h"
//---------------------------------------------------------------------------
#include "GeoXOutput.h"
//---------------------------------------------------------------------------
#include "LinearAlgebra.hpp"
#include "DynamicLinearAlgebra.hpp"
#include "SparseLinearAlgebra.hpp"
#include "StringHelper.h"
#include <sstream>
//---------------------------------------------------------------------------


using namespace std;

GeoXOutput output;


GeoXOutput& GeoXOutput::operator<<(bool b) {
   if (b) outputString("true"); else outputString("false");
   return *this;
}

GeoXOutput& GeoXOutput::operator<<(int i)  {
   outputString(intToStr(i));
   return *this;
}

GeoXOutput& GeoXOutput::operator<<(unsigned int i)  {
   outputString(intToStr(i));
   return *this;
}

GeoXOutput& GeoXOutput::operator<<(int64 i)  {
   outputString(intToStr(i));
   return *this;
}

GeoXOutput& GeoXOutput::operator<<(card64 i)  {
   outputString(intToStr((int64)i));
   return *this;
}

GeoXOutput& GeoXOutput::operator<<(long double d)  {
   std::string s;
   ostringstream os;
   os << d;
   s = os.str();
   outputString(s);
   return *this;
}


GeoXOutput& GeoXOutput::operator<<(float f)  {
   std::string s;
   ostringstream os;
   os << f;
   s = os.str();
   outputString(s);
   return *this;
}

GeoXOutput& GeoXOutput::operator<<(double d)  {
   std::string s;
   ostringstream os;
   os << d;
   s = os.str();
   outputString(s);
   return *this;
}


GeoXOutput& GeoXOutput::operator<<(const string& s)
{
   outputString(s);
   return *this;
}

GeoXOutput& GeoXOutput::operator<<(const char* s)
{
   string tmpS = s;
   outputString(tmpS);
   return *this;
}

GeoXOutput& GeoXOutput::operator<<(const Vector2f &v)  {
	(*this) << "(" << (long double)v[0] << " / " << (long double)v[1] << ")";
	return *this;
}

GeoXOutput& GeoXOutput::operator<<(const Vector3f &v)  {
	(*this) << "(" << (long double)v[0] << " / " << (long double)v[1] << " / " << (long double)v[2] << ")";
	return *this;
}

GeoXOutput& GeoXOutput::operator<<(const Vector3i &v)  {
	(*this) << "(" << int32(v[0]) << " / " << (int32)v[1] << " / " << (int32)v[2] << ")";
	return *this;
}

GeoXOutput& GeoXOutput::operator<<(const Vector4f &v)  {
	(*this) << "(" << (long double)v[0] << " / " << (long double)v[1] << " / " << (long double)v[2] << " / " << (long double)v[3] << ")";
   return *this;
}

GeoXOutput& GeoXOutput::operator<<(const Vector6f &v)  {
	(*this) << "(" << (long double)v[0] << " / " << (long double)v[1] << " / " << (long double)v[2] << " / " << (long double)v[3] << " / " << (long double)v[4] <<" / " << (long double)v[5] <<")";
   return *this;
}

GeoXOutput& GeoXOutput::operator<<(const Matrix4f &m)  {
   (*this) << "         " << m[0][0] << "  " << m[0][1] << "  " << m[0][2]<< "  " << m[0][3];
   (*this).cr();
   (*this) << "         " << m[1][0] << "  " << m[1][1] << "  " << m[1][2]<< "  " << m[1][3];
   (*this).cr();
   (*this) << "         " << m[2][0] << "  " << m[2][1] << "  " << m[2][2]<< "  " << m[2][3];
   (*this).cr();
   (*this) << "         " << m[3][0] << "  " << m[3][1] << "  " << m[3][2]<< "  " << m[3][3];
   return *this;
}

GeoXOutput& GeoXOutput::operator<<(const Matrix3f &m)  {
   (*this) << " " << m[0][0] << "  " << m[0][1] << "  " << m[0][2];
   (*this).cr();
   (*this) << " " << m[1][0] << "  " << m[1][1] << "  " << m[1][2];
   (*this).cr();
   (*this) << " " << m[2][0] << "  " << m[2][1] << "  " << m[2][2];

   return *this;
}


GeoXOutput& GeoXOutput::operator<<(const BoundingBox3f &bbox )  {
	(*this) << "(" << bbox.lowerCorner[0] << "," << bbox.lowerCorner[1] << "," << bbox.lowerCorner[2] << ")/";
	(*this) << "(" << bbox.upperCorner[0] << "," << bbox.upperCorner[1] << "," << bbox.upperCorner[2] << ")";
	return *this;
}

GeoXOutput& GeoXOutput::operator<<(const StaticVector<float32,7> &v)
{
	(*this) << "(" << v[0] << "," << v[1] << "," << v[2] << "," << v[3] << "," << v[4] << "," << v[5] << "," << v[6] << ")";
	return *this;
}


GeoXOutput& GeoXOutput::operator<<(const DynamicVector<float> &v)
{
	(*this) << "(";
	for( unsigned i=0;i<v.size();i++ )
	{
		if( i )
			(*this) << ",";
		(*this) << v[i];
	}
	(*this) << ")";
	
	return *this;	
}

GeoXOutput& GeoXOutput::operator<<(const DynamicMatrix<float> &m)
{
   for ( unsigned int r = 0; r < m.getRows(); r++ ){
      for ( unsigned int c = 0; c < m.getColumns(); c++ ){
         (*this) << m[c][r] << " ";
      }
      (*this).cr();
   }
   return *this;
}

GeoXOutput& GeoXOutput::operator<<(SparseMatrixF &A)
{
	float32 value;
	card32 index;
	for ( unsigned int r = 0; r < A.getRows(); r++ ){
		SparseVectorF& row = A[r];
		for ( unsigned int i = 0; i < row.getNumSparseEntries(); i++ ){
			row.getSparseEntry( i, index, value );
			(*this) << index << ": " << value << ", ";
		}
		(*this).cr();
	}
	return *this;
}


void GeoXOutput::writeIdented(unsigned ident, string s) {
   string identsp;
   for (unsigned i = 0; i<ident; i++) identsp += " ";
   outputString(identsp + s);
}

void GeoXOutput::cr()  {
   outputString("\n");
}

void GeoXOutput::addListener(StringListener *listener)
{
   listeners.push_back(listener);
}

void GeoXOutput::removeListener(StringListener *listener)
{
   for (vector<StringListener*>::iterator pos = listeners.begin();
        pos != listeners.end(); ++pos) {
      if (*pos == listener) {
         listeners.erase(pos);
         return;
      }
   }
}

void GeoXOutput::outputString(const string &s) {
   if (listeners.empty()) {
      cout << s;
   } else {
      for (size_t i=0; i<listeners.size(); i++) {
         listeners[i]->outputString(s);
      }
   }
}


void warning(const string& msg) {
   output << string("WARNING: ") + msg;
   output.cr();
}

void error(const string& msg) {
   output << string("ERORR: ") + msg;
   output.cr();
}

void message(const string& msg) {
   output << msg;
}

