//---------------------------------------------------------------------------
#ifndef MetaClassH
#define MetaClassH
//---------------------------------------------------------------------------
#include <string>
#include <list>
#include <vector>
#include <deque>
#include "Exceptions.h"
#include "PTypes.h"
#include "HashTables.h"
//---------------------------------------------------------------------------


using namespace std;

class  Object;
class  ClassProperty;
class  ClassMethod;
class  MetaClass;


/// This class describes the user interface aspects of a MetaClass. It is part of a MetaClass.
///
class  GUIDescriptor {
 public:
   /// pointer to a procedure that starts a new application that works with the given object
   class ApplicationPtr {
    public:
      typedef void (*ApplicationStartProc)(Object *object);
      ApplicationStartProc startProc;
      string name;
      ApplicationPtr(ApplicationStartProc startProc, string name) {
         this->startProc = startProc; this->name = name;
      }
   };

 private:
   bool autoApplyEditChanges;
   vector<const MetaClass*> classCreators; /* class of ClassCreator */
   deque<const MetaClass*> classEditors; /* class of ClassEditor */
   vector<ApplicationPtr> applications;
   /// resource identifier for icon
   string icon; 

 public:
   GUIDescriptor();
   /// apply changes automatically in class editors? (default, used everywhere right now)
   bool getAutoApplyEditChanges() const; // default = true
   /// apply changes automatically in class editors? (default, used everywhere right now)
   void setAutoApplyEditChanges(bool aap);
   /// class editors are used to interact with instances at run time
   void addClassEditor(const MetaClass *classEditor, bool defaultEditor = false);
   /// class creators are used to interactively create new instances of classes (not used frequently at this point)
   void addClassCreator(const MetaClass *classCreator);
   /// applications are special stand-alone class editors (directly instantiated as top-level windows)
   void addApplication(ApplicationPtr app);
   const MetaClass *getDefaultClassCreator() const; // may return NULL
   /// class editors are used to interact with instances at run time -- returns default editor
   /// (default for the default is the member browser)
   const MetaClass *getDefaultClassEditor() const; // default = member browser
   /// how many class creators are there?
   card32 getNumClassCreators() const;
   /// how many class editors are there?
   card32 getNumClassEditors() const;
   /// how many application class editors are there?
   card32 getNumApplications() const;
   /// class creators are used to interactively create new instances of classes (not used frequently at this point)
   const MetaClass *getClassCreator(card32 i) const;
   /// class editors are used to interact with instances at run time
   const MetaClass *getClassEditor(card32 i) const;
   /// applications are special stand-alone class editors (directly instantiated as top-level windows)
   ApplicationPtr getApplication(card32 i) const;
   /// get resource string for the class icon
   string getIcon() const;
   /// set resource string for the class icon
   void setIcon(string resourceString);
};


/// This is a class that describe classes. The mechanism is used for structural reflection.
///
class  MetaClass {
 private:

   class  MetaClass_ClassRegHashComparator {
     public:
       static bool equals(string obj1, string obj2) { return obj1 == obj2; }
   };

   class  MetaClass_ClassRegHashTable : public HashTable<string, MetaClass*, MetaClass_ClassRegHashComparator> {
    public:
      virtual max_platform_card hashFunction(string className) const;
   };

   typedef Object* (*InstanceCreator)();

   static deque<MetaClass*> oldClassRegistration;  // obsolete, legacy data structure
   static MetaClass_ClassRegHashTable classRegistration;

   card32 uniqueIdentifier;  // obsolete, >= 1000 means: not used
   card32 instanceSize;
   string className;
   string classDescription;
   card32 classVersion;      // has to be >= 1000
   InstanceCreator newInstanceMethod;
   MetaClass *parentClass;
   GUIDescriptor *guiDescr;

   vector<ClassProperty*> properties;
   vector<ClassMethod*> methods;
   vector<MetaClass*> descendants;

 public:
   class  EClassAbstract : public PException {
    public:
      EClassAbstract(string msg) : PException(msg) {}
	   virtual string getTypeString() {return "EClassAbstract";}
   };

   typedef card32 ClassVersion;

   static const ClassVersion FIRST_VERSION = 1000;

   static const card32 NO_UNIQUE_ID = 1000; /// end of legacy class id range

   /** create new meta class. newInstanceMethod == NULL for abstract classes. */
   MetaClass(string className, string classDescription, InstanceCreator newInstanceMethod,
             card32 instanceSize);
   /// initialize meta class.
   /// parentClass - parent class, NULL for root class ("Object")
   /// version -  per class version number, set to FIRST_VERSION + number
   void         init(const MetaClass *parentClass, card32 classVersion = FIRST_VERSION);

   /// ---legacy--- initialize meta class (legacy method, do not use for new code).
   /// ---legacy--- uniqueIdentifier - unique class id (for new files: automatically generated)
   /// ---legacy--- parentClass - parent class, NULL for root class ("Object")
   /// ---legacy--- version -  per class version number, set to FIRST_VERSION + number
   void         init(card32 uniqueIdentifier, const MetaClass *parentClass,
                     card32 classVersion = FIRST_VERSION);
   /// used internally by registration macros
   virtual void copyParentProperties();
   /// number of properties (member variables or getter/setter pairs)
   card32       getNumProperties() const;
   /// add a property (member variable or getter/setter pair)
   void         addProperty(ClassProperty *prop);
   /// properties (member variable or getter/setter pair)
   const ClassProperty *getProperty(card32 i) const;
   /// registered methods
   card32       getNumMethods() const;
   /// register a member method
   void         addMethod(ClassMethod *m);
   /// registered methods
   const ClassMethod *getMethod(card32 i) const;
   /// Create a new instance. Throws EClassAbstract for abstract classes.
   Object*      newInstance() const; // throws EClassAbstract
   /// Is the class abstract?
   bool         isAbstract() const; // returns newInstanceMethod == NULL
   /// obsolete - ignore
   card32       getUniqueIdentifier() const; // obsolete
   /// size of the instances (excluding references)
   card32       getInstanceSize() const;
   /// check whether the class is a descendant from mc.
   bool         inheritsFrom(const MetaClass* mc) const;
   /// The name of the class.
   const string getClassName() const;
   /// A description (mostly ignored now).
   const string getClassDescription() const;
   /// The version number - classes of the same version have the same property structure.
   card32       getClassVersion() const;

   // interface for the last class property
   /// used internally by registration macros
   void actProperty_setUserEdit( bool b );
   /// used internally by registration macros
   void actProperty_setEditReadonly( bool b );
   /// used internally by registration macros
   void actProperty_setStreamProperty( bool b );
   /// used internally by registration macros
   void actProperty_setPropertyName( string name );
   /// used internally by registration macros
   void actProperty_addPropertyEditor( const MetaClass *mc, bool defaultEd = true );
   
   /// get number of derived classes
   card32 getNumDescendants() const {return (card32)descendants.size();}
   /// get derived classes by index
   const MetaClass *getDescendant(card32 i) const {return descendants[i];}
   /// get the parent class (single inheritance model)
   const MetaClass *getParentClass() const {return parentClass;}

   /// more details about how to handle the user interface
   const GUIDescriptor *getGUIDescriptor() const {return guiDescr;}
   /// more details about how to handle the user interface
         GUIDescriptor *getGUIDescriptor() {return guiDescr;}

   inline bool  operator==(MetaClass *c) {return this == c;}

   static const MetaClass* getClassByUniqueIdentifier(card32 uniqueIdentifier);    // obsolete
   /// returns NULL if no matching class is found 
   static const MetaClass* getClassByName(const string className);
   /// returns true if class is present in global class registration table (useful for debugging)
   static bool isClassRegistered(const string className);

	/// calls a class method that does not require arguments
	/// returns true if the method was found and called successfully, false otherwise
	bool callNoArgsMethod(Object *obj, string methodName) const;

   /** creates new instance, uses (default) class creator to display creation dialog if possible */
   //Object *interactivelyCreateNewInstance(TControl *AOwner) const;


   /// call at the end of the application to check whether all classes have been shutdown (check is ommited if not in debug mode, i.e. if _DEBUG is not defined)
   static void checkAllShutdown();

   virtual ~MetaClass();
};




#endif
