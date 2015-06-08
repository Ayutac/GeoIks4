//---------------------------------------------------------------------------
#ifndef DynamicArrayOfStructuresH
#define DynamicArrayOfStructuresH
//---------------------------------------------------------------------------
#include "Persistent.h"
#include "PAssert.h"
#include "LinearAlgebra.h"
#include "StructureDescriptor.h"
#include "GeoXData.h"
//---------------------------------------------------------------------------
#include <string>
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------


/// An array of structures. The composition of the structure ("channels") can be changed at runtime.
/// Each channel has
///  - a base type (any simple scalar type, for example, float32, or card8, or int32)
///  - a dimension (for example, a channel for "position" or "normal" might be three-dimensional)
///  - a unique name (for example "position" or "normal)
///
/// The DynamicArrayOfStructures is a collection of elements, where each element has entries for each of the specified channels.
///
/// For example, a vertex buffer of a triangle mesh might have channels "position", "color", "normal", each of which provide 3 floats.
/// Each entry consists of 9 floats, and the object stores a whole array of such entries.
///
/// DynamicArrayOfStructures are used to represent various more complex geometric primitives, such as triangle meshes or point clouds.
/// A point cloud consists of a single DynamicArrayOfStructures with at least a "position" channel (three floats).
/// A triangle mesh is a point cloud that adds a second DynamicArrayOfStructures that provides a sequence of triangles;
/// this one has at least the "index" channel (three times an int32)
///
/// Usage:
///
/// Channels are identified by strings. Therefore, direct access would be super slow. Instead, you should get an
/// AttributeAccessToken (AAT) first; with the AAT (which is just an integer offset to the channel) you can access
/// the attributes extremely quickly.
///
/// The class provides a large set of templated, inlined (i.e., fast!) getters and setters. Please use the one with the matching
/// dimension and type.
///
/// To create a new DynamicArrayOfStructures, you can use the clearAndSetup method; it needs to know how much space to reserve
/// initially (filled with zeros), and what channel structure to use (specified by a StructureDescriptor)
///
/// Do not forget to include the file DynamicArrayOfStructures.hpp so that you can use the template code!
///
class DynamicArrayOfStructures : public GeoXData {
   GEOX_CLASS(DynamicArrayOfStructures)

 private:
	StructureDescriptor *sDescr;
   char *data;
   mpcard numEntries;
   mpcard elementSize;
   mpcard allocatedNumEntries;

	string getDescrString() const;

   char * getDataPtr() const;
   friend class StructureDescriptor;

 private:
	/// This method can be calle from the user interface to create an small example
	/// data set (to test the viewers etc).
	void createExampleDataSet();
 
 public:
   DynamicArrayOfStructures();

   virtual void assign(const Object* obj);
   virtual void write(OutputObjectStream *s) const;
   virtual void read(InputObjectStream *s);

   /// Clear / setup a new instance. VertDescr will be owned by new Point Set afterwards
	void clearAndSetup(mpcard numEntries, const StructureDescriptor *sDescr);

   /// Just remove all points.
   void clear();

   /// Get the StructureDescriptor.
   const StructureDescriptor *getDescr() const;

   // macro
   /// get access to a specific channel, by name
	inline AttributeAccessToken getAAT(string name) const;
   /// get access to a specific channel, by name and check all other parameters, too
   inline AttributeAccessToken getAAT(string name, card32 numComponents, card32 dataFormat) const;
   /// check whether the channel exists (name only)
   inline bool providesAttribute(string name) const;
   /// check whether the channel exists (with all parameters)
   inline bool providesAttribute(string name, card32 numComponents, card32 dataFormat) const;

   /// get the number of entries
   inline mpcard getNumEntries() const;
   /// get the size of a single element (according to StructureDescriptor)
   inline mpcard getElementSize() const;

   /// smart resize (double/half allocated size if necessary)
   void changeSize(const mpcard newSize);
   /// remove single point at given index
   void deletePoint(const mpcard index);

   /// Add a single point (buffer points to data in format according to current StructureDescriptor)
   /// New point will become the last element of the array (getNumEntries() - 1)
   /// The buffer must point to a piece of memory with a densely packed channel structure.
   /// It is recommended to not use this method but rather create a larger buffer and use the getters and setters for writing.
   /// TODO: add emtpy point entries without pointer to empty memory.
   void addPoint(const char *buffer);
   /// same as addPoint hoever adds a point as a first element in the array
   void addPointFront(const char *buffer);

   /// returns a subset of the entries as new DAOS
   DynamicArrayOfStructures* subset(const vector<mpcard>& indices);

   /// Creates copy with new Channels. Duplicates are avoided: if an attribute name already exists, no new data field is created.
	DynamicArrayOfStructures *appendAttribs(const StructureDescriptor *newAttribs) const;

	/// This method dumps the content into a simple, linear buffer in a white-space separated text format
	/// See method fromString() for a format description
	virtual string toString();
	/// This method reads the data from a text buffer in a simple, white-space separated format.
	/// The object is re-initialized from scratch.
	///
	/// The format is quite simple: Any line starting with "#" is ignored (comments)
	/// The first line must contain the structure descriptor in the format [<name> <dim> <type>]*
	/// For example: position 3 float32 color 3 card8 flags 1 int32
	/// The next lines are lists of white-space separated values, such as:
	/// 1.2 1.5 1.3 128 129 255 1014 (this would be a typical entry for the example descriptor).
	virtual void fromString(const string &buffer);

   // --- specialized, templated getters and setters see DynamicArrayOfStructures class description for discussion

   template<class Format, int dim>
   inline StaticVector<Format, dim> get(mpcard i,AAT token) const;
   template<class Format, int dim>
   inline void set(mpcard i, AAT token, const StaticVector<Format, dim> &v);

   /// getter - see DynamicArrayOfStructures class description
   template<typename ScalarType>
   inline ScalarType get(mpcard i,AAT token) const;
   template<typename ScalarType>
   inline void set(mpcard i, AAT token, ScalarType v);

   template<class Format, int dim1, int dim2>
	inline StaticMatrix<Format, dim1, dim2> get(mpcard i,AAT token) const;
   template<class Format, int dim1, int dim2>
	inline void set(mpcard i, AAT token, const StaticMatrix<Format, dim1, dim2> &m);

   inline card8 get1ub(mpcard i,AAT token) const{ return (get<card8,1>(i, token))[0]; }
	inline void set1ub(mpcard i, AAT token, const StaticVector<card8,1> &v){ set<card8,1>(i,token, v); }
	inline void set1ub(mpcard i, AAT token, const card8 value ){
		StaticVector<card8,1> v; v[0] = value;
		set<card8,1>(i,token, v);
	}

   inline int32 get1i(mpcard i,AAT token) const{ return (get<int32,1>(i, token))[0]; }
   inline void set1i(mpcard i, AAT token, const StaticVector<int32,1> &v){ set<int32,1>(i,token, v); }
	inline void set1i(mpcard i, AAT token, const int32 value ){
		StaticVector<int32,1> v; v[0] = value;
		set<int32,1>(i,token, v);
	}

	inline card32 get1u(mpcard i,AAT token) const{ return (get<card32,1>(i, token))[0]; }
	inline void set1u(mpcard i, AAT token, const StaticVector<card32,1> &v){ set<card32,1>(i,token, v); }
	inline void set1u(mpcard i, AAT token, const card32 value ){
		StaticVector<card32,1> v; v[0] = value;
		set<card32,1>(i,token, v);
	}

	inline float32 get1f(mpcard i,AAT token) const{ return (get<float32,1>(i, token))[0]; }
	inline void set1f(mpcard i, AAT token, const StaticVector<float32,1> &v){ set<float32,1>(i,token, v); }
	inline void set1f(mpcard i, AAT token, const float32 value ){
		StaticVector<float32,1> v; v[0] = value;
		set<float32,1>(i,token, v);
	}

	inline Vector2ub get2ub(mpcard i, AAT token){
		return get<card8,2>(i,token);
	}
	inline void set2ub(mpcard i, AAT token, const Vector2ub& v){
		return set<card8,2>(i,token,v);
	}

	inline Vector2i get2i(mpcard i, AAT token) const{
		return get<int32,2>(i,token);
	}
	inline void set2i(mpcard i, AAT token, const Vector2i& v){
		return set<int32,2>(i,token,v);
	}

	inline Vector2f get2f(mpcard i, AAT token) const{
		return get<float32,2>(i,token);
	}
	inline void set2f(mpcard i, AAT token, const Vector2f& v){
		return set<float32,2>(i,token,v);
	}

	inline Vector3ub get3ub(mpcard i,AAT token) const{
		 return get<card8,3>(i, token);
   }
	inline void set3ub(mpcard i, AAT token, const Vector3ub &v){
		 set<card8,3>(i,token, v);
	}

	inline Vector3i get3i(mpcard i,AAT token) const{
		 return get<int32,3>(i, token);
   }
	inline void set3i(mpcard i, AAT token, const Vector3i &v){
		 set<int32,3>(i,token, v);
	}

	inline Vector3f get3f(mpcard i,AAT token) const{
		 return get<float32,3>(i, token);
   }
	inline void set3f(mpcard i, AAT token, const Vector3f &v){
		 set<float32,3>(i,token, v);
	}

	inline Vector4ub get4ub(mpcard i,AAT token) const{
		 return get<card8,4>(i, token);
   }
	inline void set4ub(mpcard i, AAT token, const Vector4ub &v){
		 set<card8,4>(i,token, v);
	}

	inline Vector4i get4i(mpcard i,AAT token) const{
		 return get<int32,4>(i, token);
   }
	inline void set4i(mpcard i, AAT token, const Vector4i &v){
		 set<int32,4>(i,token, v);
	}

	inline Vector4f get4f(mpcard i,AAT token) const{
		 return get<float32,4>(i, token);
   }
	inline void set4f(mpcard i, AAT token, const Vector4f &v){
		 set<float32,4>(i,token, v);
	}

	inline Matrix3f get9f(mpcard i,AAT token) const{
		return get<float32,3,3>(i, token);
	}
	inline void set9f(mpcard i, AAT token, const Matrix3f &m){
		set<float32,3,3>(i,token, m);
	}

   /// set a single entry to a buffer (needs packed channel memory layout; not recommended)
	inline void setAllAttribs(mpcard index, const char *buffer);

   /// copy single entry to a buffer (needs packed channel memory layout; not recommended)
	inline void getAllAttribs(mpcard index, char *buffer) const;

   /// get pointer to single entry (low-level, not recommended)
   inline char *getDataPointer(mpcard index = 0);

   /// get pointer to single entry (low-level, not recommended) [const version]
   inline const char *getDataPointer(mpcard index = 0) const;

	/// Read all attributes at once, and store result in a dynamic vector (slow, but easy to use)
	/// Important: The dimension of values must be large enough to store all the channels, i.e.,
	/// values.getDim() >= getDescriptor()->computeOverallDimension()
	/// Otherwise, the program will crash (this is not checked for efficiency reasons).
	template <typename FloatType>
	inline void getAsDVector(mpcard index, DynamicVector<FloatType> &values) const;

	/// Write all attributes at once, read from a dynamic vector (slow, but easy to use)
	/// Important: The dimension of values must be large enough to store all the channels, i.e.,
	/// values.getDim() >= getDescriptor()->computeOverallDimension()
	/// Otherwise, the program will crash (this is not checked for efficiency reasons).
	template <typename FloatType>
    inline void setByDVector(mpcard index, const DynamicVector<FloatType> &values);

   // inherited
   virtual const MetaClass *getDefaultViewer();
   // inherited
   virtual const MetaClass *getDefaultViewerDefaultCanvas();

   ~DynamicArrayOfStructures();
};

typedef DynamicArrayOfStructures DAOS;



#endif
