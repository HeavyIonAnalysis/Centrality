#include <iostream>

#include "Fitter.h"
#include "FitterHelper.h"

#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include "TLegend.h"
#include "TH2.h"

using namespace Glauber;

int main(int argc, char *argv[])
{
    
    bool flag;
    TString letter = argv[1];
    TString dir{"../"};
    if(letter=="M" || letter=="m")
        flag = false;
    else
        if(letter=="E" || letter=="e")
            flag = true;
        else
        {
            std::cout << argv[1] << std::endl;
            std::cout << "Wrong first argument! Enter M for multiplicity fit and E for energy fit" << std::endl;
            return -1;
        }
    
    Float_t f0;
    Int_t k0, k1;
    
    if(flag == false)
    {
        if (argc < 5)
        {
            std::cout << "Wrong number of parameters! Executable usage:" << std::endl;
            std::cout << "   ./glauber M f0 k0 k1" << std::endl;
            return -1;
        }
        f0 = atof( argv[2]);
        k0 = atoi( argv[3] );
        k1 = atoi( argv[4] );
        if (argc > 5)
            dir = argv[5];
    }
        
    if(flag == true)
    {
        if(argc < 3)
        {
            std::cout << "Wrong number of parameters! Executable usage:" << std::endl;
            std::cout << "    ./glauber E k0 k1" << std::endl;
            return -1;
        }
        k0 = atoi( argv[2] );
        k1 = atoi( argv[3] );
        if (argc > 4)
            dir = argv[4];
    }   
    
    // *****************************************
    // Modify this part according to your needs
    // *****************************************

    ///  |   mode    |   function for Na     |
    ///  |   Default | f*Npart + (1-f)*Ncoll |
    ///  |   PSD     |     f - Npart         |
    ///  |   Npart   |     Npart^f           |
    ///  |   Ncoll   |     Ncoll^f           |
    const TString mode = "Default";

    const TString glauber_filename = dir +  "/input/glauber_auau_sigma_30_10M.root";   // input files
    const TString glauber_treename = "nt_Au_Au";
    const TString in_filename = dir + "/input/pure.urqmd.ana.root";
    //const TString in_filename = dir + "/input/test_input_01.root";
    
    TString histoname;
    if(flag == false)
        histoname = "hMpure";
    else
        histoname = "hEpsd";
    
    const Int_t min_bin = 50;      // not fitting low multiplicity region due to trigger bias, etc
    const Int_t max_bin = 10000;   // very large number to fit the whole histo
    const Int_t nevents = 10000000;

    const TString outdir = ".";
    // *****************************************
    // *****************************************

    std::unique_ptr<TFile> glauber_file{ TFile::Open(glauber_filename, "read") };
    std::unique_ptr<TTree> glauber_tree{ (TTree*) glauber_file->Get(glauber_treename) };

    std::unique_ptr<TFile> f{TFile::Open(in_filename)};    
    TH1F *hdata = (TH1F*)f->Get(histoname);
    
    Fitter fitter ( std::move(glauber_tree) );

    fitter.SetMode(mode);
    fitter.SetInputHisto(*hdata);
    fitter.SetBinSize(1);
    fitter.Init(nevents);
    
    fitter.SetFitMinBin(min_bin);
    fitter.SetFitMaxBin(max_bin);
    fitter.SetOutDirName(outdir);

    float par[3];
    float chi2;
    if(flag == false)
    {
        chi2 = fitter.FitGlauber(par, f0, k0, k1, nevents);
        std::cout << "f = " << par[0] << "    mu = " << par[1] << "    k = " << par[2] << "    chi2 = " << chi2 << std::endl;
    }
    else
    {
        chi2 = fitter.FitGlauberE(par, k0, k1, nevents);
        std::cout << "mu = " << par[0] << "    sigma = " << par[1] << "    chi2 = " << chi2 << std::endl; 
    }
    
    Glauber::DrawHistos(fitter, true, true, true, true);

    const float range[2] = {300, 350.};
    std::unique_ptr<TH1F> hB(fitter.GetModelHisto (range, "B", par, 100000));
    hB->SaveAs( "b_test.root" );
    
    std::cout << "END!" << std::endl;

    return 0;
}
