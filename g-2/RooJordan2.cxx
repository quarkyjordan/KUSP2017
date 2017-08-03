/***************************************************************************** 
 * Project: RooFit                                                           * 
 *                                                                           * 
 * This code was autogenerated by RooClassFactory                            * 
 *****************************************************************************/ 

// Your description goes here... 

#include "Riostream.h" 

#include "RooJordan2.h" 
#include "RooAbsReal.h" 
#include "RooAbsCategory.h" 
#include <math.h> 
#include "TMath.h" 

ClassImp(RooJordan2) 

 RooJordan2::RooJordan2(const char *name, const char *title, 
                        RooAbsReal& _t,
                        RooAbsReal& _wa,
                        RooAbsReal& _A,
                        RooAbsReal& _tau,
                        RooAbsReal& _phi,
                        RooAbsReal& _N) :
   RooAbsPdf(name,title), 
   t("t","t",this,_t),
   wa("wa","wa",this,_wa),
   A("A","A",this,_A),
   tau("tau","tau",this,_tau),
   phi("phi","phi",this,_phi),
   N("N","N",this,_N)
 { 
 } 


 RooJordan2::RooJordan2(const RooJordan2& other, const char* name) :  
   RooAbsPdf(other,name), 
   t("t",this,other.t),
   wa("wa",this,other.wa),
   A("A",this,other.A),
   tau("tau",this,other.tau),
   phi("phi",this,other.phi),
   N("N",this,other.N)
 { 
 } 



 Double_t RooJordan2::evaluate() const 
 { 
   // ENTER EXPRESSION IN TERMS OF VARIABLE ARGUMENTS HERE 
   return N*exp(-t/tau)*(1+A*cos(wa*t + phi)); 
 } 



