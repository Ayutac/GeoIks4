//---------------------------------------------------------------------------
#include "precompiledHeaders.h"
//---------------------------------------------------------------------------
#include "ClassMethods.h"
#include "StringHelper.h"
//---------------------------------------------------------------------------


ClassMethod::ClassMethod(string name, string description, string icon, bool visible) {
   static const string splitter = "::";
   vector<string> separated = splitString( name, splitter );
   if (separated.empty()) {
      this->name = name;
   } else {
      this->name = separated.back();
   }
   this->description = description;
	this->icon = icon;
	this->visible = visible;
}
string ClassMethod::getDescription() const {
   return description;
}

string ClassMethod::getName() const {
	return name;
}

string ClassMethod::getIcon() const {
	return icon;
}

bool ClassMethod::getVisible() const {
	return visible;
}

// ---

NoArgsClassMethod::NoArgsClassMethod(MethodPointer method, string name, string description, string icon, bool visible) : ClassMethod(name, description, icon, visible) {
   this->method = method;
}

void NoArgsClassMethod::call(Object *obj) const {
   (obj->*method)();
}

ClassMethod *NoArgsClassMethod::copyMethod() const {
   ClassMethod *result = new NoArgsClassMethod(method, getName(), getDescription(), getIcon());
   return result;
}

TimerClassMethod::TimerClassMethod(NoArgsClassMethod::MethodPointer method, string name,
											  string description, string icon, bool visible, card32 defaultIntervalMilliSec)
	: NoArgsClassMethod(method, name, description, icon, visible)
{
	this->defaultIntervalMilliSec = defaultIntervalMilliSec;
}

card32 TimerClassMethod::getDefaultIntervalMilliSec() const
{
	 return defaultIntervalMilliSec;
}

void TimerClassMethod::setDefaultIntervalMilliSec(const card32 &value)
{
	 defaultIntervalMilliSec = value;
}
