//---------------------------------------------------------------------------
#ifndef DynamicArrayOfStructures2DH
#define DynamicArrayOfStructures2DH
//---------------------------------------------------------------------------
#include "DynamicArrayOfStructures.h"
//---------------------------------------------------------------------------


/// An 2D array of structures. 
/// The class has the same semantics as the DynamicArrayOfStructures; however, it represents a two-dimensional array
/// indexed by x and y coordinates (with width and height).
///
/// This class is for example used to represent 2D images (with an arbitrary amount of channels; the standard
/// setup is to use a 3dim or 4dim color channel to represent standard bitmap images.
///
/// Examples (shorthand notation for format used):
///  - color3f: floating point RGB image
///  - color4ub: 8-bit RGBA image
///  - depth1f: a depth image (range image)
/// 
/// Internally, the class maps all functionality to an ordinary DynamicArrayOfStructures, but makes sure that the
/// index computations take the 2D structure into account.
///
class DynamicArrayOfStructures2D : public GeoXData {
	GEOX_CLASS(DynamicArrayOfStructures2D)

 private:
	DynamicArrayOfStructures *data;

	mpcard width;
	mpcard height;

 private:
	/// This method can be calle from the user interface to create an small example
	/// data set (to test the viewers etc).
	void createExampleDataSet();
 
 public:
	DynamicArrayOfStructures2D();

	/// inherited - overridden for technical reasons
	virtual void assign(const Object* obj);

   /// Clear / setup a new instance. VertDescr will be owned by new Point Set afterwards
	void clearAndSetup(mpcard width, mpcard height, const StructureDescriptor *sDescr);

	/// Just remove everything.
   void clear();

	/// get the width of the DAOS2D
	mpcard getWidth() const;
	/// get the height of the DAOS2D
	mpcard getHeight() const;
	/// get read access to the internal data object that contains the data in a 1D linear list
	const DynamicArrayOfStructures *getData() const;

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

   /// Creates copy with new Channels. Duplicates are avoided: if an attribute name already exists, no new data field is created.
	DynamicArrayOfStructures2D *appendAttribs(const StructureDescriptor *newAttribs) const;

	// --- specialized, templated getters and setters see DynamicArrayOfStructures class description for discussion

   template<class Format, int dim>
	inline StaticVector<Format, dim> get(mpcard x, mpcard y, AAT token) const;
   template<class Format, int dim>
	inline void set(mpcard x, mpcard y, AAT token, const StaticVector<Format, dim> &v);

   /// getter - see DynamicArrayOfStructures class description
   template<typename ScalarType>
	inline ScalarType get(mpcard x, mpcard y, AAT token) const;
   template<typename ScalarType>
	inline void set(mpcard x, mpcard y, AAT token, ScalarType v);

   template<class Format, int dim1, int dim2>
	inline StaticMatrix<Format, dim1, dim2> get(mpcard x, mpcard y,AAT token) const;
   template<class Format, int dim1, int dim2>
	inline void set(mpcard x, mpcard y, AAT token, const StaticMatrix<Format, dim1, dim2> &m);

	inline card8 get1ub(mpcard x, mpcard y,AAT token) const{ return (get<card8,1>(x, y, token))[0]; }
	inline void set1ub(mpcard x, mpcard y, AAT token, const StaticVector<card8,1> &v){ set<card8,1>(x, y,token, v); }
	inline void set1ub(mpcard x, mpcard y, AAT token, const card8 value ){
		StaticVector<card8,1> v; v[0] = value;
		set<card8,1>(x, y,token, v);
	}

	inline int32 get1i(mpcard x, mpcard y,AAT token) const{ return (get<int32,1>(x, y, token))[0]; }
	inline void set1i(mpcard x, mpcard y, AAT token, const StaticVector<int32,1> &v){ set<int32,1>(x, y,token, v); }
	inline void set1i(mpcard x, mpcard y, AAT token, const int32 value ){
		StaticVector<int32,1> v; v[0] = value;
		set<int32,1>(x, y,token, v);
	}

	inline card32 get1u(mpcard x, mpcard y,AAT token) const{ return (get<card32,1>(x, y, token))[0]; }
	inline void set1u(mpcard x, mpcard y, AAT token, const StaticVector<card32,1> &v){ set<card32,1>(x, y,token, v); }
	inline void set1u(mpcard x, mpcard y, AAT token, const card32 value ){
		StaticVector<card32,1> v; v[0] = value;
		set<card32,1>(x, y,token, v);
	}

	inline float32 get1f(mpcard x, mpcard y,AAT token) const{ return (get<float32,1>(x, y, token))[0]; }
	inline void set1f(mpcard x, mpcard y, AAT token, const StaticVector<float32,1> &v){ set<float32,1>(x, y,token, v); }
	inline void set1f(mpcard x, mpcard y, AAT token, const float32 value ){
		StaticVector<float32,1> v; v[0] = value;
		set<float32,1>(x, y,token, v);
	}

	inline Vector2ub get2ub(mpcard x, mpcard y, AAT token){
		return get<card8,2>(x, y,token);
	}
	inline void set2ub(mpcard x, mpcard y, AAT token, const Vector2ub& v){
		return set<card8,2>(x, y,token,v);
	}

	inline Vector2i get2i(mpcard x, mpcard y, AAT token) const{
		return get<int32,2>(x, y,token);
	}
	inline void set2i(mpcard x, mpcard y, AAT token, const Vector2i& v){
		return set<int32,2>(x, y,token,v);
	}

	inline Vector2f get2f(mpcard x, mpcard y, AAT token) const{
		return get<float32,2>(x, y,token);
	}
	inline void set2f(mpcard x, mpcard y, AAT token, const Vector2f& v){
		return set<float32,2>(x, y,token,v);
	}

	inline Vector3ub get3ub(mpcard x, mpcard y,AAT token) const{
		 return get<card8,3>(x, y, token);
   }
	inline void set3ub(mpcard x, mpcard y, AAT token, const Vector3ub &v){
		 set<card8,3>(x, y,token, v);
	}

	inline Vector3i get3i(mpcard x, mpcard y,AAT token) const{
		 return get<int32,3>(x, y, token);
   }
	inline void set3i(mpcard x, mpcard y, AAT token, const Vector3i &v){
		 set<int32,3>(x, y,token, v);
	}

	inline Vector3f get3f(mpcard x, mpcard y,AAT token) const{
		 return get<float32,3>(x, y, token);
   }
	inline void set3f(mpcard x, mpcard y, AAT token, const Vector3f &v){
		 set<float32,3>(x, y,token, v);
	}

	inline Vector4ub get4ub(mpcard x, mpcard y,AAT token) const{
		 return get<card8,4>(x, y, token);
   }
	inline void set4ub(mpcard x, mpcard y, AAT token, const Vector4ub &v){
		 set<card8,4>(x, y,token, v);
	}

	inline Vector4i get4i(mpcard x, mpcard y,AAT token) const{
		 return get<int32,4>(x, y, token);
   }
	inline void set4i(mpcard x, mpcard y, AAT token, const Vector4i &v){
		 set<int32,4>(x, y,token, v);
	}

	inline Vector4f get4f(mpcard x, mpcard y,AAT token) const{
		 return get<float32,4>(x, y, token);
   }
	inline void set4f(mpcard x, mpcard y, AAT token, const Vector4f &v){
		 set<float32,4>(x, y,token, v);
	}

	inline Matrix3f get9f(mpcard x, mpcard y,AAT token) const{
		return get<float32,3,3>(x, y, token);
	}
	inline void set9f(mpcard x, mpcard y, AAT token, const Matrix3f &m){
		set<float32,3,3>(x, y,token, m);
	}

   /// set a single entry to a buffer (needs packed channel memory layout; not recommended)
	inline void setAllAttribs(mpcard x, mpcard y, const char *buffer);

   /// copy single entry to a buffer (needs packed channel memory layout; not recommended)
	inline void getAllAttribs(mpcard x, mpcard y, char *buffer) const;

   /// get pointer to single entry (low-level, not recommended)
	inline char *getDataPointer(mpcard x, mpcard y);

   /// get pointer to single entry (low-level, not recommended) [const version]
	inline const char *getDataPointer(mpcard x, mpcard y) const;

	/// Read all attributes at once, and store result in a dynamic vector (slow, but easy to use)
	/// Important: The dimension of values must be large enough to store all the channels, i.e.,
	/// values.getDim() >= getDescriptor()->computeOverallDimension()
	/// Otherwise, the program will crash (this is not checked for efficiency reasons).
	template <typename FloatType>
	inline void getAsDVector(mpcard x, mpcard y, DynamicVector<FloatType> &values) const;

	/// Write all attributes at once, read from a dynamic vector (slow, but easy to use)
	/// Important: The dimension of values must be large enough to store all the channels, i.e.,
	/// values.getDim() >= getDescriptor()->computeOverallDimension()
	/// Otherwise, the program will crash (this is not checked for efficiency reasons).
	template <typename FloatType>
	inline void setByDVector(mpcard x, mpcard y, const DynamicVector<FloatType> &values) const;

   // inherited
   virtual const MetaClass *getDefaultViewer();
   // inherited
   virtual const MetaClass *getDefaultViewerDefaultCanvas();

	~DynamicArrayOfStructures2D();
};

typedef DynamicArrayOfStructures2D DAOS2D;



#endif
