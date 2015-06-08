//-------------------------------------------- // 
#ifndef ExampleExperimentH                     // <--- don't UNUSED the "guards"
#define ExampleExperimentH                     // <--- use a unique name, for example FileNameH, as shown here
//-------------------------------------------- //
#include "Experiment.h"                        // <--- include the base class
#include "LinearAlgebra.h"                     // <--- for Vector3f, Matrix3f
//-------------------------------------------- //
                                               //
                                               //
                                               //
///                                            //
/// This is an example experiment.             //
///                                            //
/// The code is meant to demonstrate how       //
///  to use the GeoX framework                 //
///                                            //
class ExampleExperiment : public Experiment {  // <--- derive your experiment from base class <Experiment>
   GEOX_CLASS(ExampleExperiment)               // <--- use the macro to create registration code
                                               //
 private:                                      //
   int32 numberA;                              // <---
   float32 numberB;                            // <---
   Vector3f vector1;                           // <---
   Vector3f vector2;                           // <--- declare the parameters you need (will be registered in *.cpp file)
   Matrix3f matrix;                            // <---
   string someString;                          // <---
   bool someBoolean;                           // <--- 
                                               //
 public:                                       //
   ExampleExperiment();                        // <--- the constructor is used to intialize the parameters
   void computeAPlusB();                       // <--- method 1: first experiment, will do some calculations
   void crossProduct();                        // <--- method 2: second experiment, will do some linear algebra
   void matrixVectorProduct();                 // <--- method 3: third experiment, will do more some linear algebra
	void outputCounter();                       // <--- method 4: this method uses a "timer"; it will count numbers on the console
};                                             //
                                               //

                                               
#endif                                         
