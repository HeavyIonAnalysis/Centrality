
#ifndef GlauberFitter_H
#define GlauberFitter_H 1

#include <vector>
#include "TString.h"
#include "TNamed.h"
#include "TH1F.h"
#include "TTree.h"
// #include "TMinuit.h"


namespace Glauber
{
    class Fitter
    {
        
    public:
        
        /**   Default constructor   **/
        Fitter() {};
        Fitter(std::unique_ptr<TTree> tree) ;
        /**   Destructor   **/
        virtual ~Fitter(){};
        
        void Init(int nEntries);
        void SetGlauberFitHisto (Float_t f, Float_t mu, Float_t k, Int_t n = 10000, Bool_t Norm2Data = true);
        void NormalizeGlauberFit ();
        void DrawHistos (Bool_t isSim = true, Bool_t isData = true, Bool_t isGlauber = false, Bool_t isNBD = false);
        
        void FitGlauber (float *par, Float_t f0, Int_t k0, Int_t k1, Int_t nEvents);
        void FindMuGoldenSection (Float_t *mu, Float_t *chi2, Float_t mu_min, Float_t mu_max, Float_t f, Float_t k, Int_t nEvents = 10000, Int_t nIter = 5);
        
        Float_t GetChi2 (void) const;
        
        Float_t NBD(Float_t n, Float_t mu, Float_t k) const;
        void SetNBDhist(Float_t mu, Float_t k);
        float Nancestors(float f) const;
        float NancestorsMax(float f) const;
        
        std::unique_ptr<TH1F> GetModelHisto (const Float_t range[2], TString name, const Float_t par[3], Int_t nEvents);
        
//         
//         Setters
//         
        void SetInputHisto (const TH1F &h)   { fDataHisto = h; }
        void SetFitMinBin  (Int_t min)      { fFitMinBin = min; }
        void SetFitMaxBin  (Int_t min)      { fFitMaxBin = min; }
        void SetNormMinBin  (Int_t min)     { fNormMinBin = min; }
        void SetBinSize  (Int_t size)        { fBinSize = size; }
        void SetOutDirName (TString name)    { fOutDirName = name; }
        void SetMode (const TString mode) { fMode = mode; }
 
//         
//         Getters
//         
        TH1F GetGlauberFitHisto () const { return fGlauberFitHisto; }
        TH1F GetDataHisto ()       const { return fDataHisto;  }
        TH1F GetNBDHisto ()        const { return fNbdHisto;   }
        TH1F GetNpartHisto ()      const { return fNpartHisto; }
        TH1F GetNcollHisto ()      const { return fNcollHisto;  }
        TH1F GetBestFiHisto ()     const { return fBestFitHisto;  }

        
    private:
        
        /**   Data members  **/
        TH1F fNpartHisto; 
        TH1F fNcollHisto; 
        TH1F fDataHisto; 
        TH1F fNbdHisto; 
        TH1F fGlauberFitHisto; 
        TH1F fBestFitHisto;
        
        /* MC data */
        std::unique_ptr<TTree> fSimTree{nullptr};
        
        Float_t fNpart{-1.};
        Float_t fNcoll{-1.};

        Float_t fMaxValue{-1.};
        
        Int_t fNbins{-1};
        Int_t fBinSize{-1};
        
        Int_t fFitMinBin{-1};
        Int_t fFitMaxBin{-1};

        Int_t fNormMinBin{-1};
        
        TString fMode{"Default"};
        
        TString fOutDirName{""};
        ClassDef(Fitter, 2);
        
    };
}

#endif
