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
    if (argc == 0)
    {
        std::cout << "Wrong number of parameters! Executable usage:" << std::endl;
        std::cout << "   ./glauber" << std::endl;
        return -1;
    }


    Fitter fitter;

    //**********************************************************************************************************************************************************************************************//
    //                                                                           Modify this part according to your needs                                                                           //
    //**********************************************************************************************************************************************************************************************//

    ///  |Ancestor_Mode|   function for Na     |
    ///  |   Default   | f*Npart + (1-f)*Ncoll |
    ///  |     PSD     |       f-Npart         |
    ///  |    Npart    |       Npart^f         |
    ///  |    Ncoll    |       Ncoll^f         |
    
    fitter.SetMultiplicityStep (1);								   // Set step of variating lower and upper multiplicity range
    fitter.SetMinMultiplicity (5);								   // Set not fitting low multiplicity region due to trigger bias, etc
    fitter.SetMaxMultiplicity (200);								   // Set very large number to fit the whole histo
    fitter.SetMinFitRange (190);	           						   // Set minimal multiplicity range for fitting
    fitter.Set_kMin (95.0);									   // Set lower parameter k
    fitter.Set_kMax (100.0);									   // Set upper parameter k
    fitter.Set_kStep (0.5);									   // Set step of variating of parameter k
    fitter.Set_fMin (0.9);									   // Set lower parameter f
    fitter.Set_fMax (1.0);									   // Set upper parameter f
    fitter.Set_fStep (0.1);								           // Set of step of variating of parameter f
    fitter.SetGlauber_filename ("/home/vad/NA61/Centrality/glau_PbPb_13GeV.root");                 // Set input file with Glauber Tree
    fitter.SetGlauber_treename ("nt_Pb_Pb");                                                       // Set name of tree with GlauberMC model
    fitter.SetDataHisto_filename ("/home/vad/NA61/QA/qa_new.root");                                // Set input file with data histo
    fitter.SetDataHisto_name ("reco_info/hMreco");                                                 // Set name of data histo
    fitter.SetOutDirName (".");                                                                    // Set name of directory for output files
    fitter.SetOutFileIDName ("_MTEST");                                                         // Set ID of output files (ending of all output files of one job will have same ID)
    fitter.SetAncestor_Mode ("Default");                                                           // Set mode for counting ancestors
    fitter.SetFitFunction_Mode ("NBD");                                                            // Set mode for fitting function (COMING SOON)
    fitter.SetFit_Mode ("Iteration");                                                              // Set mode for fitting procedure ("GoldenSection" or "Iteration")
    fitter.SetnMuIter  (10);                                                                       // Set number of iterations of finding better mu (with same f and k)
    fitter.SetMassNumber (207);                                                                    // Set mass number of projectile nucleus
    fitter.SetNEvents (999999);                                                                    // Set number of events that you need from GlauberMC (or you can uncomment the same setter a few lines down with default number of events: 10 times the number of events in the data histo in the fitting range)

    //**********************************************************************************************************************************************************************************************//
    //**********************************************************************************************************************************************************************************************//

    std::cout << "f0=" << fitter.Get_fMin() << std::endl;
    std::cout << "f1=" << fitter.Get_fMax() << std::endl;
    std::cout << "f_step=" << fitter.Get_fStep() << std::endl;
    std::cout << "k0=" << fitter.Get_kMin() << std::endl;
    std::cout << "k1=" << fitter.Get_kMax() << std::endl;
    std::cout << "k_step=" << fitter.Get_kStep() << std::endl;
    std::cout << "min_bin=" << fitter.GetMinMultiplicity() << std::endl;
    std::cout << "max_bin=" << fitter.GetMaxMultiplicity() << std::endl;
    std::cout << "bin_step=" << fitter.GetMultiplicityStep() << std::endl;

    std::unique_ptr<TFile> glauber_file{ TFile::Open(fitter.GetGlauber_filename(), "read") };
    std::unique_ptr<TTree> glauber_tree{ (TTree*) glauber_file->Get(fitter.GetGlauber_treename()) };
    std::unique_ptr<TFile> f{TFile::Open(fitter.GetDataHisto_filename())};    
    TH1F *hdata = (TH1F*)f->Get(fitter.GetDataHisto_name());

    fitter.SetNEvents(10*(int(hdata->Integral(fitter.GetFitMinBin(),fitter.GetFitMaxBin()))));

    fitter.SetInputHisto(*hdata);

    fitter.Init(std::move(glauber_tree));

    float par[4];
	
    float chi2=1e10;
    chi2 = fitter.FitGlauber(par);

    DrawHistos(fitter, par, chi2, true, true, true, true);
    
    std::cout << "END!" << std::endl;

    return 0;
}
