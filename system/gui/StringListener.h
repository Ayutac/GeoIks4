#ifndef STRINGLISTENER_H
#define STRINGLISTENER_H


/// This interface class represents a string-output device.
/// Used for the standard output window.
///
class StringListener { // interface
 public:
   virtual void outputString(const string &s) = 0;
   virtual ~StringListener() {} // make sure that the class always destructs orderly
};



#endif // STRINGLISTENER_H

