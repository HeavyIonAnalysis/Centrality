// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME G__Centrality

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "/lustre/nyx/cbm/users/lubynets/soft/centrality/src/BordersFinder.h"
#include "/lustre/nyx/cbm/users/lubynets/soft/centrality/src/BordersFinderHelper.h"
#include "/lustre/nyx/cbm/users/lubynets/soft/centrality/src/Getter.h"
#include "/lustre/nyx/cbm/users/lubynets/soft/centrality/src/BordersFinder2D.h"
#include "/lustre/nyx/cbm/users/lubynets/soft/centrality/src/Container.h"
#include "/lustre/nyx/cbm/users/lubynets/soft/centrality/glauber/Fitter.h"
#include "/lustre/nyx/cbm/users/lubynets/soft/centrality/glauber/FitterHelper.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_CentralitycLcLGetter(void *p = 0);
   static void *newArray_CentralitycLcLGetter(Long_t size, void *p);
   static void delete_CentralitycLcLGetter(void *p);
   static void deleteArray_CentralitycLcLGetter(void *p);
   static void destruct_CentralitycLcLGetter(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Centrality::Getter*)
   {
      ::Centrality::Getter *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Centrality::Getter >(0);
      static ::ROOT::TGenericClassInfo 
         instance("Centrality::Getter", ::Centrality::Getter::Class_Version(), "src/Getter.h", 21,
                  typeid(::Centrality::Getter), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Centrality::Getter::Dictionary, isa_proxy, 4,
                  sizeof(::Centrality::Getter) );
      instance.SetNew(&new_CentralitycLcLGetter);
      instance.SetNewArray(&newArray_CentralitycLcLGetter);
      instance.SetDelete(&delete_CentralitycLcLGetter);
      instance.SetDeleteArray(&deleteArray_CentralitycLcLGetter);
      instance.SetDestructor(&destruct_CentralitycLcLGetter);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Centrality::Getter*)
   {
      return GenerateInitInstanceLocal((::Centrality::Getter*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::Centrality::Getter*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_GlaubercLcLFitter(void *p = 0);
   static void *newArray_GlaubercLcLFitter(Long_t size, void *p);
   static void delete_GlaubercLcLFitter(void *p);
   static void deleteArray_GlaubercLcLFitter(void *p);
   static void destruct_GlaubercLcLFitter(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Glauber::Fitter*)
   {
      ::Glauber::Fitter *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::Glauber::Fitter >(0);
      static ::ROOT::TGenericClassInfo 
         instance("Glauber::Fitter", ::Glauber::Fitter::Class_Version(), "glauber/Fitter.h", 21,
                  typeid(::Glauber::Fitter), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::Glauber::Fitter::Dictionary, isa_proxy, 4,
                  sizeof(::Glauber::Fitter) );
      instance.SetNew(&new_GlaubercLcLFitter);
      instance.SetNewArray(&newArray_GlaubercLcLFitter);
      instance.SetDelete(&delete_GlaubercLcLFitter);
      instance.SetDeleteArray(&deleteArray_GlaubercLcLFitter);
      instance.SetDestructor(&destruct_GlaubercLcLFitter);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Glauber::Fitter*)
   {
      return GenerateInitInstanceLocal((::Glauber::Fitter*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::Glauber::Fitter*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace Centrality {
//______________________________________________________________________________
atomic_TClass_ptr Getter::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *Getter::Class_Name()
{
   return "Centrality::Getter";
}

//______________________________________________________________________________
const char *Getter::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Centrality::Getter*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int Getter::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Centrality::Getter*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Getter::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Centrality::Getter*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Getter::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Centrality::Getter*)0x0)->GetClass(); }
   return fgIsA;
}

} // namespace Centrality
namespace Glauber {
//______________________________________________________________________________
atomic_TClass_ptr Fitter::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *Fitter::Class_Name()
{
   return "Glauber::Fitter";
}

//______________________________________________________________________________
const char *Fitter::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Glauber::Fitter*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int Fitter::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::Glauber::Fitter*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Fitter::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Glauber::Fitter*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Fitter::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::Glauber::Fitter*)0x0)->GetClass(); }
   return fgIsA;
}

} // namespace Glauber
namespace Centrality {
//______________________________________________________________________________
void Getter::Streamer(TBuffer &R__b)
{
   // Stream an object of class Centrality::Getter.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(Centrality::Getter::Class(),this);
   } else {
      R__b.WriteClassBuffer(Centrality::Getter::Class(),this);
   }
}

} // namespace Centrality
namespace ROOT {
   // Wrappers around operator new
   static void *new_CentralitycLcLGetter(void *p) {
      return  p ? new(p) ::Centrality::Getter : new ::Centrality::Getter;
   }
   static void *newArray_CentralitycLcLGetter(Long_t nElements, void *p) {
      return p ? new(p) ::Centrality::Getter[nElements] : new ::Centrality::Getter[nElements];
   }
   // Wrapper around operator delete
   static void delete_CentralitycLcLGetter(void *p) {
      delete ((::Centrality::Getter*)p);
   }
   static void deleteArray_CentralitycLcLGetter(void *p) {
      delete [] ((::Centrality::Getter*)p);
   }
   static void destruct_CentralitycLcLGetter(void *p) {
      typedef ::Centrality::Getter current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::Centrality::Getter

namespace Glauber {
//______________________________________________________________________________
void Fitter::Streamer(TBuffer &R__b)
{
   // Stream an object of class Glauber::Fitter.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(Glauber::Fitter::Class(),this);
   } else {
      R__b.WriteClassBuffer(Glauber::Fitter::Class(),this);
   }
}

} // namespace Glauber
namespace ROOT {
   // Wrappers around operator new
   static void *new_GlaubercLcLFitter(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::Glauber::Fitter : new ::Glauber::Fitter;
   }
   static void *newArray_GlaubercLcLFitter(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::Glauber::Fitter[nElements] : new ::Glauber::Fitter[nElements];
   }
   // Wrapper around operator delete
   static void delete_GlaubercLcLFitter(void *p) {
      delete ((::Glauber::Fitter*)p);
   }
   static void deleteArray_GlaubercLcLFitter(void *p) {
      delete [] ((::Glauber::Fitter*)p);
   }
   static void destruct_GlaubercLcLFitter(void *p) {
      typedef ::Glauber::Fitter current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::Glauber::Fitter

namespace ROOT {
   static TClass *vectorlEfloatgR_Dictionary();
   static void vectorlEfloatgR_TClassManip(TClass*);
   static void *new_vectorlEfloatgR(void *p = 0);
   static void *newArray_vectorlEfloatgR(Long_t size, void *p);
   static void delete_vectorlEfloatgR(void *p);
   static void deleteArray_vectorlEfloatgR(void *p);
   static void destruct_vectorlEfloatgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<float>*)
   {
      vector<float> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<float>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<float>", -2, "vector", 214,
                  typeid(vector<float>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEfloatgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<float>) );
      instance.SetNew(&new_vectorlEfloatgR);
      instance.SetNewArray(&newArray_vectorlEfloatgR);
      instance.SetDelete(&delete_vectorlEfloatgR);
      instance.SetDeleteArray(&deleteArray_vectorlEfloatgR);
      instance.SetDestructor(&destruct_vectorlEfloatgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<float> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<float>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEfloatgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<float>*)0x0)->GetClass();
      vectorlEfloatgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEfloatgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEfloatgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<float> : new vector<float>;
   }
   static void *newArray_vectorlEfloatgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<float>[nElements] : new vector<float>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEfloatgR(void *p) {
      delete ((vector<float>*)p);
   }
   static void deleteArray_vectorlEfloatgR(void *p) {
      delete [] ((vector<float>*)p);
   }
   static void destruct_vectorlEfloatgR(void *p) {
      typedef vector<float> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<float>

namespace ROOT {
   static TClass *vectorlEarraylEfloatcO2gRsPgR_Dictionary();
   static void vectorlEarraylEfloatcO2gRsPgR_TClassManip(TClass*);
   static void *new_vectorlEarraylEfloatcO2gRsPgR(void *p = 0);
   static void *newArray_vectorlEarraylEfloatcO2gRsPgR(Long_t size, void *p);
   static void delete_vectorlEarraylEfloatcO2gRsPgR(void *p);
   static void deleteArray_vectorlEarraylEfloatcO2gRsPgR(void *p);
   static void destruct_vectorlEarraylEfloatcO2gRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<array<float,2> >*)
   {
      vector<array<float,2> > *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<array<float,2> >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<array<float,2> >", -2, "vector", 214,
                  typeid(vector<array<float,2> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEarraylEfloatcO2gRsPgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<array<float,2> >) );
      instance.SetNew(&new_vectorlEarraylEfloatcO2gRsPgR);
      instance.SetNewArray(&newArray_vectorlEarraylEfloatcO2gRsPgR);
      instance.SetDelete(&delete_vectorlEarraylEfloatcO2gRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEarraylEfloatcO2gRsPgR);
      instance.SetDestructor(&destruct_vectorlEarraylEfloatcO2gRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<array<float,2> > >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<array<float,2> >*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEarraylEfloatcO2gRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<array<float,2> >*)0x0)->GetClass();
      vectorlEarraylEfloatcO2gRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEarraylEfloatcO2gRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEarraylEfloatcO2gRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<array<float,2> > : new vector<array<float,2> >;
   }
   static void *newArray_vectorlEarraylEfloatcO2gRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<array<float,2> >[nElements] : new vector<array<float,2> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEarraylEfloatcO2gRsPgR(void *p) {
      delete ((vector<array<float,2> >*)p);
   }
   static void deleteArray_vectorlEarraylEfloatcO2gRsPgR(void *p) {
      delete [] ((vector<array<float,2> >*)p);
   }
   static void destruct_vectorlEarraylEfloatcO2gRsPgR(void *p) {
      typedef vector<array<float,2> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<array<float,2> >

namespace {
  void TriggerDictionaryInitialization_libCentrality_Impl() {
    static const char* headers[] = {
"/lustre/nyx/cbm/users/lubynets/soft/centrality/src/BordersFinder.h",
"/lustre/nyx/cbm/users/lubynets/soft/centrality/src/BordersFinderHelper.h",
"/lustre/nyx/cbm/users/lubynets/soft/centrality/src/Getter.h",
"/lustre/nyx/cbm/users/lubynets/soft/centrality/src/BordersFinder2D.h",
"/lustre/nyx/cbm/users/lubynets/soft/centrality/src/Container.h",
"/lustre/nyx/cbm/users/lubynets/soft/centrality/glauber/Fitter.h",
"/lustre/nyx/cbm/users/lubynets/soft/centrality/glauber/FitterHelper.h",
0
    };
    static const char* includePaths[] = {
"/cvmfs/fairroot.gsi.de/fairsoft/may18/include/root",
"/lustre/nyx/cbm/users/lubynets/soft/centrality",
"/cvmfs/fairroot.gsi.de/fairsoft/may18/include/root",
"/lustre/nyx/cbm/users/lubynets/soft/centrality/build/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "libCentrality dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
namespace Centrality{class __attribute__((annotate("$clingAutoload$/lustre/nyx/cbm/users/lubynets/soft/centrality/src/BordersFinderHelper.h")))  Getter;}
namespace Glauber{class __attribute__((annotate("$clingAutoload$/lustre/nyx/cbm/users/lubynets/soft/centrality/glauber/Fitter.h")))  Fitter;}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "libCentrality dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif
#ifndef R__HAVE_CONFIG
  #define R__HAVE_CONFIG 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "/lustre/nyx/cbm/users/lubynets/soft/centrality/src/BordersFinder.h"
#include "/lustre/nyx/cbm/users/lubynets/soft/centrality/src/BordersFinderHelper.h"
#include "/lustre/nyx/cbm/users/lubynets/soft/centrality/src/Getter.h"
#include "/lustre/nyx/cbm/users/lubynets/soft/centrality/src/BordersFinder2D.h"
#include "/lustre/nyx/cbm/users/lubynets/soft/centrality/src/Container.h"
#include "/lustre/nyx/cbm/users/lubynets/soft/centrality/glauber/Fitter.h"
#include "/lustre/nyx/cbm/users/lubynets/soft/centrality/glauber/FitterHelper.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"Centrality::Getter", payloadCode, "@",
"Glauber::Fitter", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("libCentrality",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_libCentrality_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_libCentrality_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_libCentrality() {
  TriggerDictionaryInitialization_libCentrality_Impl();
}
