/** @file   Fitter.h
    @class  Glauber::Fitter
    @author Viktor Klochkov (klochkov44@gmail.com)
    @author Ilya Selyuzhenkov (ilya.selyuzhenkov@gmail.com)
    @brief  Class to fit histo with Glauber based function
*/

#ifndef GlauberFitter_H
#define GlauberFitter_H 1

#include <vector>
#include <map>
#include "TString.h"
#include "TNamed.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TTree.h"
// #include "TMinuit.h"


namespace Glauber
{
	enum eGlauberParameters {
		kNpart = 0, kNcoll, kNhard, kB, kBNN, kNcollpp, kNcollpn, kNcollnn, kVarX, kVarY, kVarXY, kNpartA, kNpartB, kNpart0, kAreaW, kPsi1, kEcc1, kPsi2, kEcc2, kPsi3, kEcc3, kPsi4, kEcc4, kPsi5, kEcc5, kAreaO, kAreaA, kX0, kY0, kPhi0, kLength, kMeanX, kMeanY, kMeanX2, kMeanY2, kMeanXY, kMeanXSystem, kMeanYSystem, kMeanXA, kMeanYA, kMeanXB, kMeanYB, kPhiA, kThetaA, kPhiB, kThetaB, kGP
				};

	const struct TGlauberParameters {
	  Int_t id;
	  TString name;
	  TString title; 
	  TString axis_title;
	  Float_t bin_value;
	  } gGlauberParameters[kGP] = {
	  	{ .id = kNpart,       .name = "Npart",       .title = "Number of participating nucleons",                .axis_title="N_{part}",               .bin_value = 1    },
	  	{ .id = kNcoll,       .name = "Ncoll",       .title = "Number of binary collisions",                     .axis_title="N_{coll}",               .bin_value = 1    },
	  	{ .id = kNhard,       .name = "Nhard",       .title = "Number of hard collisions",                       .axis_title="N_{hard}",               .bin_value = 1    },
		{ .id = kB,           .name = "B",           .title = "Impact parameter",                                .axis_title="#it{B}, fm",             .bin_value = 0.01 },
		{ .id = kBNN,         .name = "BNN",         .title = "Mean nucleon–nucleon impact parameter",           .axis_title="#it{B_{NN}}, fm",        .bin_value = 0.01 },
		{ .id = kNcollpp,     .name = "Ncollpp",     .title = "Number of pp collisions",                         .axis_title="N^{pp}_{coll}",          .bin_value = 1    },
		{ .id = kNcollpn,     .name = "Ncollpn",     .title = "Number of pn collisions",                         .axis_title="N^{pn}_{coll}",          .bin_value = 1    },
		{ .id = kNcollnn,     .name = "Ncollnn",     .title = "Number of nn collisions",                         .axis_title="N^{nn}_{coll}",          .bin_value = 1    },
		{ .id = kVarX,        .name = "VarX",        .title = "Variance of x for wounded nucleons",              .axis_title="#sigma^{2}_{x}, fm^{2}", .bin_value = 0.01 },
		{ .id = kVarY,        .name = "VarY",        .title = "Variance of y for wounded nucleons",              .axis_title="#sigma^{2}_{x}, fm^{2}", .bin_value = 0.01 },
		{ .id = kVarXY,       .name = "VarXY",       .title = "Covariance of x and y for wounded nucleons",      .axis_title="sigma_{xy}, fm^{2}",     .bin_value = 0.01 },
		{ .id = kNpartA,      .name = "NpartA",      .title = "Number of participating nucleons from nucleus A", .axis_title="N^{A}_{part}",           .bin_value = 1    },
		{ .id = kNpartB,      .name = "NpartB",      .title = "Number of participating nucleons from nucleus B", .axis_title="N^{B}_{part}",           .bin_value = 1    },
		{ .id = kNpart0,      .name = "Npart0",      .title = "Number of singly-wounded participating nucleons", .axis_title="N^{0}_{part}",           .bin_value = 1    },
		{ .id = kAreaW,       .name = "AreaW",       .title = "Area defined by width of participants",           .axis_title="AreaW, fm^{2}",          .bin_value = 0.01 },
		{ .id = kPsi1,        .name = "Psi1",        .title = "Event plane angle of 1st harmonic",               .axis_title="#psi_{1}, rad",          .bin_value = 0.01 },
		{ .id = kEcc1,        .name = "Ecc1",        .title = "Participant eccentricity for 1st harmonic",       .axis_title="#epsilon_{1}",           .bin_value = 0.01 },
		{ .id = kPsi2,        .name = "Psi2",        .title = "Event plane angle of 2nd harmonic",               .axis_title="#psi_{2}, rad",          .bin_value = 0.01 },
		{ .id = kEcc2,        .name = "Ecc2",        .title = "Participant eccentricity for 2nd harmonic",       .axis_title="#epsilon_{2}",           .bin_value = 0.01 },
		{ .id = kPsi3,        .name = "Psi3",        .title = "Event plane angle of 3rd harmonic",               .axis_title="#psi_{3}, rad",          .bin_value = 0.01 },
		{ .id = kEcc3,        .name = "Ecc3",        .title = "Participant eccentricity for 3rd harmonic",       .axis_title="#epsilon_{3}",           .bin_value = 0.01 },
		{ .id = kPsi4,        .name = "Psi4",        .title = "Event plane angle of 4th harmonic",               .axis_title="#psi_{4}, rad",          .bin_value = 0.01 },
		{ .id = kEcc4,        .name = "Ecc4",        .title = "Participant eccentricity for 4th harmonic",       .axis_title="#epsilon_{4}",           .bin_value = 0.01 },
		{ .id = kPsi5,        .name = "Psi5",        .title = "Event plane angle of 5th harmonic",               .axis_title="#psi_{5}, rad",          .bin_value = 0.01 },
		{ .id = kEcc5,        .name = "Ecc5",        .title = "Participant eccentricity for 5th harmonic",       .axis_title="#epsilon_{5}",           .bin_value = 0.01 },
		{ .id = kAreaO,       .name = "AreaO",       .title = "Area by ”or” of participants in grid",            .axis_title="Area OR",                .bin_value = 0.01 },
		{ .id = kAreaA,       .name = "AreaA",       .title = "Area by ”and” of participants in grid",           .axis_title="Area AND",               .bin_value = 0.01 },
		{ .id = kX0,          .name = "X0",          .title = "Production point in x",                           .axis_title="x, fm",                  .bin_value = 0.01 },
		{ .id = kY0,          .name = "Y0",          .title = "Production point in y",                           .axis_title="y, fm",                  .bin_value = 0.01 },
		{ .id = kPhi0,        .name = "Phi0",        .title = "Direction in #varphi",                            .axis_title="#varphi, rad",           .bin_value = 0.01 },
		{ .id = kLength,      .name = "Length",      .title = "Length in #varphi",                               .axis_title="p_{T} (GeV/#it{c})",     .bin_value = 0.01 },
		{ .id = kMeanX,       .name = "MeanX",       .title = "Mean of x for wounded nucleons",                  .axis_title="<x>, fm",                .bin_value = 0.01 },
		{ .id = kMeanY,       .name = "MeanY",       .title = "Mean of y for wounded nucleons",                  .axis_title="<y>, fm",                .bin_value = 0.01 },
		{ .id = kMeanX2,      .name = "MeanX2",      .title = "Mean of x^{2} for wounded nucleons",              .axis_title="<x^{2}>, fm^2",          .bin_value = 0.01 },
		{ .id = kMeanXY,      .name = "MeanY2",      .title = "Mean of y^{2} for wounded nucleons",              .axis_title="<y^{2}>, fm^2",          .bin_value = 0.01 },  
		{ .id = kMeanY2,      .name = "MeanXY",      .title = "Mean of xy for wounded nucleons",                 .axis_title="<xy>, fm^2",             .bin_value = 0.01 },
		{ .id = kMeanXSystem, .name = "MeanXSystem", .title = "Mean of x for all nucleons",                      .axis_title="<x>, fm",                .bin_value = 0.01 },
		{ .id = kMeanYSystem, .name = "MeanYSystem", .title = "Mean of y for all nucleons",                      .axis_title="<y>, fm",                .bin_value = 0.01 },
		{ .id = kMeanXA,      .name = "MeanXA",      .title = "Mean of x for nucleons in nucleus A",             .axis_title="<x>, fm",                .bin_value = 0.01 },
		{ .id = kMeanYA,      .name = "MeanYA",      .title = "Mean of y for nucleons in nucleus A",             .axis_title="<y>, fm",                .bin_value = 0.01 },
		{ .id = kMeanXB,      .name = "MeanXB",      .title = "Mean of x for nucleons in nucleus B",             .axis_title="<x>, fm",                .bin_value = 0.01 },
		{ .id = kMeanYB,      .name = "MeanYB",      .title = "Mean of y for nucleons in nucleus B",             .axis_title="<y>, fm",                .bin_value = 0.01 },
		{ .id = kPhiA,        .name = "PhiA",        .title = "Azimuthal angle for nucleus A",                   .axis_title="#varphi^{A}, rad",       .bin_value = 0.01 },
		{ .id = kThetaA,      .name = "ThetaA",      .title = "Polar angle for nucleus A",                       .axis_title="#theta^{A}, rad",        .bin_value = 0.01 },
		{ .id = kPhiB,        .name = "PhiB",        .title = "Azimuthal angle for nucleus B",                   .axis_title="#varphi^{B}, rad",       .bin_value = 0.01 },
		{ .id = kThetaB,      .name = "ThetaB",      .title = "Polar angle for nucleus B",                       .axis_title="#theta^{A}, rad",        .bin_value = 0.01 }
	        };

    class Fitter
    {
        
    public:
        
        /**   Default constructor   **/
        Fitter() {};
        /**   Destructor   **/
        virtual ~Fitter(){};
        
        void Init(std::unique_ptr<TTree> tree);															// Initialize all parameters of class Fitter
        void SetGlauberFitHisto (Float_t f, Float_t mu, Float_t k, Bool_t Norm2Data = true);									// Setting of model histogtam 
        void NormalizeGlauberFit ();																// Function for normilize model histogram with data histogram
        void DrawHistos (Bool_t isSim = true, Bool_t isData = true, Bool_t isGlauber = false, Bool_t isNBD = false);						// Function for drawing output histograms and creating output files
        
        float FitGlauber (float *par, Float_t f0, Float_t f1, Int_t k0, Int_t k1);										// Main function for fitting
        void FindMuGoldenSection (Float_t *mu, Float_t *chi2, float*chi2_error, Float_t mu_min, Float_t mu_max, Float_t f, Float_t k, int n=0);			// Function that find mu by iteration it to the minimum of chi2
        
        Float_t GetChi2 (void) const;																// Function for chi2 counting
	Float_t GetChi2Error (void) const;															// Function for chi2 error counting
        
        Float_t NBD(Float_t n, Float_t mu, Float_t k) const;													// Function for NBD counting with parameters mu and k
        void SetNBDhist(Float_t mu, Float_t k);															// Setting of NBD distribution with parameters mu and k

        float Nancestors(float f) const;															// Function for counting amount of ancestors in one event
        float NancestorsMax(float f) const;															// Function for counting maximum amount of ancestors in one event
        
        std::unique_ptr<TH1F> GetModelHisto (const Float_t range[2], TString name, const Float_t par[4]);                                                       // Returns model histogram with known parameters
        
//         
//         Setters
//         
        void SetInputHisto (const TH1F &h)   { fDataHisto = h; }												// Setting of data histogram
        void SetFitMinBin  (Int_t min)      { fFitMinBin = min; }												// Setting of not fitting low multiplicity region due to trigger bias, etc
        void SetFitMaxBin  (Int_t max)      { fFitMaxBin = max; }												// Setting of very large number to fit the whole histo
        void SetnIter  (Int_t nIter)     { fnIter = nIter; }													// Setting of number of iterations of finding better mu (with same f and k)
        void SetOutDirName (const TString name)    { fOutDirName = name; }											// Setting of the name of directory for output files
	void SetOutFileIDName (const TString name) { fOutFileIDName = name; }											// Setting of ID of output files (ending of all output files of one job will have same ID)
	void SetAncestor_Mode (const TString Ancestor_Mode) { fAncestor_Mode = Ancestor_Mode; }									// Setting of the mode for counting ancestors
	void SetFitFunction_Mode (const TString FitFunction_Mode) { fFitFunction_Mode = FitFunction_Mode; }							// Setting of the mode for fitting function (COMING SOON)
	void SetGlauber_filename (const TString Glauber_filename) { fGlauber_filename = Glauber_filename; }							// Setting of input file with Glauber Tree
	void SetGlauber_treename (const TString Glauber_treename) { fGlauber_treename = Glauber_treename; }							// Setting of the name of tree with GlauberMC model
	void SetDataHisto_filename (const TString DataHisto_filename) { fDataHisto_filename = DataHisto_filename; }						// Setting of input file with data histo
	void SetDataHisto_name (const TString DataHisto_name) { fDataHisto_name = DataHisto_name; }								// Setting of the name of data histo
	void SetMassNumber (Float_t A) { fA = A; }														// Setting of mass number of projectile nucleus
	void SetNEvents (Int_t Events) { nEvents = Events; }													// Setting of number of events that you need from GlauberMC
 
//         
//         Getters
//      
	TString GetOutFileIDName () const { return fOutFileIDName; }												// Returns ID of output files (its ending)
	TString GetGlauber_filename () { return fGlauber_filename; }												// Returns input file with Glauber Tree
	TString GetGlauber_treename () { return fGlauber_treename; }												// Returns the name of tree with
	TString GetDataHisto_filename () { return fDataHisto_filename; }											// Returns input file with data histo
	TString GetDataHisto_name () { return fDataHisto_name; }												// Returns the name of data histo
	Int_t GetFitMinBin  ()   const   { return fFitMinBin; }													// Returns not fitting low multiplicity region due to trigger bias, etc
        Int_t GetFitMaxBin  ()   const   { return fFitMaxBin; }													// Returns very large number to fit the whole histo
	   
        TH1F GetGlauberFitHisto () const { return fGlauberFitHisto; }												// Returns model histogtam
	TH1F GetBestFitHisto ()    const { return fBestFitHisto;  }												// Returns model histogtam with minimal chi2
        TH1F GetDataHisto ()       const { return fDataHisto;  }												// Returns data histogram
        TH1F GetNBDHisto ()        const { return fNbdHisto;   }												// Returns histogram of NBD distribution
        
	std::map <TString, TH1F*> GetMapOfGlauber_Parameters_Histos () const { return Glauber_Parameters_Histos; }						// Returns map with historams of parameters of GlauberMC input file
	std::map <TString, TH2F*> GetMapOfGlauber_Parameters_VS_Multiplicity_Histos () const { return Glauber_Parameters_VS_Multiplicity_Histos; }		// Returns map with historams of parameters of GlauberMC input file versus modeled multiplicity
	std::map <TString, TH2F*> GetMapOfGlauber_Parameters_VS_Multiplicity_BestHistos () const { return Glauber_Parameters_VS_Multiplicity_BestHistos; }	// Returns map with historams of parameters of GlauberMC input file versus modeled multiplicity with minimal #chi^{2}
        
    private:
        
        /**   Data members  **/

	TH1F fDataHisto; 
        TH1F fNbdHisto;
        TH1F fGlauberFitHisto; 
        TH1F fBestFitHisto;

	std::map <TString, TH1F*> Glauber_Parameters_Histos;       
	std::map <TString, TH2F*> Glauber_Parameters_VS_Multiplicity_Histos;
	std::map <TString, TH2F*> Glauber_Parameters_VS_Multiplicity_BestHistos;
	
        
        /* MC data */
        std::unique_ptr<TTree> fSimTree{nullptr};
        
	Float_t fA{-1.}; //mass number
	std::map <TString, Float_t> Glauber_Parameters = {
		{"Npart", -1.},                   // Number of participating nucleons
	        {"Ncoll", -1.},                   // Number of binary collisions
		{"Nhard", -1.},                   // Number of hard collisions (with hard particle production)
		{"B", -1.},                       // Generated impact parameter, fm
	      	{"BNN", -1.},                     // Mean nucleon–nucleon impact parameter, fm
		{"Ncollpp", -1.},                 // Number of pp collisions
		{"Ncollpn", -1.},                 // Number of pn collisions
		{"Ncollnn", -1.},                 // Number of nn collisions
		{"VarX", -1.},                    // Variance of x for wounded nucleons, σ^2_x, fm^2
		{"VarY", -1.},                    // Variance of y for wounded nucleons, σ^2_y, fm^2
		{"VarXY", -1.},                   // Covariance of x and y for wounded nucleons, σ_xy ≡ <xy> − <x><y>, fm^2
		{"NpartA", -1.},                  // Number of participating nucleons from nucleus A
		{"NpartB", -1.},                  // Number of participating nucleons from nucleus B
		{"Npart0", -1.},                  // Number of singly-wounded participating nucleons
		{"AreaW", -1.},                   // Area defined by width of participants, fm^2
		{"Psi1", -1.},                    // Event plane angle of 1st harmonic, rad
		{"Ecc1", -1.},                    // Participant eccentricity for 1st harmonic
		{"Psi2", -1.},                    // Event plane angle of 2nd harmonic, rad
		{"Ecc2", -1.},                    // Participant eccentricity for 2nd harmonic
		{"Psi3", -1.},                    // Event plane angle of 3rd harmonic, rad
		{"Ecc3", -1.},                    // Participant eccentricity for 3rd harmonic
		{"Psi4", -1.},                    // Event plane angle of 4th harmonic, rad
		{"Ecc4", -1.},                    // Participant eccentricity for 4th harmonic
		{"Psi5", -1.},                    // Event plane angle of 5th harmonic, rad
		{"Ecc5", -1.},                    // Participant eccentricity for Nth harmonic
		{"AreaO", -1.},                   // Area by ”or” of participants in grid
		{"AreaA", -1.},                   // Area by ”and” of participants in grid
		{"X0", -1.},                      // Production point in x, fm
		{"Y0", -1.},                      // Production point in y, fm
		{"Phi0", -1.},                    // Direction in phi
		{"Length", -1.},                  // Length in phi0
		{"MeanX", -1.},                   // Mean of x for wounded nucleons, <x>, fm
		{"MeanY", -1.},                   // Mean of y for wounded nucleons, <y>, fm
		{"MeanX2", -1.},                  // Mean of x^2 for wounded nucleons, <x^2>, fm^2
		{"MeanY2", -1.},                  // Mean of y^2 for wounded nucleons, <y^2>, fm^2
		{"MeanXY", -1.},                  // Mean of xy for wounded nucleons, <xy>, fm^2
		{"MeanXSystem", -1.},             // Mean of x for all nucleons, fm^2
		{"MeanYSystem", -1.},             // Mean of y for all nucleons, fm^2
		{"MeanXA", -1.},                  // Mean of x for nucleons in nucleus A, fm^2
		{"MeanYA", -1.},                  // Mean of y for nucleons in nucleus A, fm^2
		{"MeanXB", -1.},                  // Mean of x for nucleons in nucleus B, fm^2
		{"MeanYB", -1.},                  // Mean of y for nucleons in nucleus B, fm^2
		{"PhiA", -1.},                    // Azimuthal angle for nucleus A, rad
		{"ThetaA", -1.},                  // Polar angle for nucleus A, rad
		{"PhiB", -1.},                    // Azimuthal angle for nucleus B, rad
		{"ThetaB", -1.},                  // Polar angle for nucleus B, rad
		};
	
	
	std::map <TString, const int> Glauber_ParametersMax;
	std::map <TString, const int> Glauber_ParametersMin;

        Float_t fMaxValue{-1.};
        
        Int_t fNbins{-1	};
        
        Int_t fFitMinBin{-1};
        Int_t fFitMaxBin{-1};
	Int_t nEvents;

	Int_t fnIter;
        
        TString fAncestor_Mode{"Default"};
	TString fFitFunction_Mode{"NBD"};
	TString fGlauber_filename{""};
	TString fGlauber_treename{""};
	TString fDataHisto_filename{""};
	TString fDataHisto_name{""};
        
        TString fOutDirName{""};
	TString fOutFileIDName {""};
        ClassDef(Fitter, 2);
        
    };
}

#endif
