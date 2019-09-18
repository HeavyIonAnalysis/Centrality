#include <iostream>

#include "Fitter.h"
#include "FitterHelper.h"

#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include "TLegend.h"
#include "TH2.h"

#include <map>

using namespace Glauber;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "Wrong number of parameters! Executable usage:" << std::endl;
        std::cout << "   ./glauber f0 k0" << std::endl;
        return -1;
    }
    const Float_t f0 = atof( argv[1]);
    std::cout << "f=" << f0 << std::endl;
    const Float_t f1 = atof( argv[2]);
    std::cout << "f=" << f1 << std::endl;
    const Int_t k0 = atoi( argv[3] );
    std::cout << "k0=" << k0 << std::endl;
    const Int_t k1 = atoi( argv[4] );
    std::cout << "k1=" << k1 << std::endl;

    Fitter fitter;
    
    // *****************************************
    // Modify this part according to your needs
    // *****************************************

    ///  |Ancestor_Mode|   function for Na     |
    ///  |    Default  | f*Npart + (1-f)*Ncoll |
    ///  |     PSD     |       f-Npart         |
    ///  |    Npart    |       Npart^f         |
    ///  |    Ncoll    |       Ncoll^f         |


    fitter.SetFitMinBin  (60);                                                                        // Set not fitting low multiplicity region due to trigger bias, etc
    fitter.SetFitMaxBin  (200);                                                                       // Set very large number to fit the whole histo
    fitter.SetGlauber_filename ("/home/vad/NA61/Centrality/glau_PbPb_13GeV.root");                    // Set input file with Glauber Tree
    fitter.SetGlauber_treename ("nt_Pb_Pb");                                                          // Set name of tree with GlauberMC model
    fitter.SetDataHisto_filename ("/home/vad/NA61/QA/qa_new.root");             // Set input file with data histo
    fitter.SetDataHisto_name ("reco_info/hMreco");                                                    // Set name of data histo
    fitter.SetOutDirName (".");                                                                       // Set name of directory for output files
    fitter.SetOutFileIDName ("_M13new_5");                                                        // Set ID of output files (ending of all output files of one job will have same ID)
    fitter.SetAncestor_Mode ("Default");                                                              // Set mode for counting ancestors
    fitter.SetFitFunction_Mode ("NBD");                                                               // Set mode for fitting function (COMING SOON)
    fitter.SetnIter  (10);                                                                             // Set number of iterations of finding better mu (with same f and k)
    fitter.SetMassNumber (207);                                                                       // Set mass number of projectile nucleus
    fitter.SetNEvents (999999);                                                                       // Set number of events that you need from GlauberMC (or you can uncomment the same setter a few lines down with default number of events: 10 times the number of events in the data histo in the fitting range)

    std::cout << "min_bin=" << fitter.GetFitMinBin() << "   max_bin=" << fitter.GetFitMaxBin() << std::endl;

    // *****************************************
    // *****************************************

    std::unique_ptr<TFile> glauber_file{ TFile::Open(fitter.GetGlauber_filename(), "read") };
    std::unique_ptr<TTree> glauber_tree{ (TTree*) glauber_file->Get(fitter.GetGlauber_treename()) };
    std::unique_ptr<TFile> f{TFile::Open(fitter.GetDataHisto_filename())};    
    TH1F *hdata = (TH1F*)f->Get(fitter.GetDataHisto_name());

    fitter.SetNEvents(10*(int(hdata->Integral(fitter.GetFitMinBin(),fitter.GetFitMaxBin()))));

    fitter.SetInputHisto(*hdata);

    fitter.Init(std::move(glauber_tree));

    float par[4];
	
    float chi2=1e10;
    chi2 = fitter.FitGlauber(par, f0, f1, k0, k1);

    std::cout << "f = " << par[0] << "    mu = " << par[1] << "    k = " << par[2] << "    chi2 = " << chi2 << "    chi2_error = " << par[3] << std::endl; 
    
//    Glauber::DrawHistos(fitter, true, true, true, true);

    DrawHistos(fitter, par, chi2, true, true, true, true);
    
    std::cout << "END!" << std::endl;

    return 0;
}
