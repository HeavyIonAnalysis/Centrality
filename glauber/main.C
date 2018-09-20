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
    if (argc < 2)
    {
        std::cout << "Wrong number of parameters! Executable usage:" << std::endl;
        std::cout << "   ./glauber f0 k0" << std::endl;
        return -1;
    }
    const Float_t f0 = atof( argv[1]);
    const Int_t k0 = atoi( argv[2] );
    const Int_t k1 = atoi( argv[2]+1 );
    
    // *****************************************
    // Modify this part according to your needs
    // *****************************************

    const Int_t min_bin = 50;
    const TString mode = "Default";
    
    const TString glauber_filename = "/home/vklochkov/Data/glauber/pbpb/30agev/glau_pbpb_ntuple_signn_31.0_7.6AGeV_CM_30AGeV_LC.root";
    const TString in_filename = "../input/test_input.root";
    const TString histoname = "hMreco";

    const Int_t max_bin = 400;
    const Int_t nevents = 199000;

    const TString treename = "nt_Pb_Pb";
    const TString outdir = ".";
    // *****************************************
    // *****************************************

    std::unique_ptr<TFile> glauber_file{ TFile::Open(glauber_filename, "read") };
    std::unique_ptr<TTree> glauber_tree{ (TTree*) glauber_file->Get(treename) };

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
    const float chi2 = fitter.FitGlauber(par, f0, k0, k1, nevents);

    std::cout << "f = " << par[0] << "    mu = " << par[1] << "    k = " << par[2] << "    chi2 = " << chi2 << std::endl; 
    
    Glauber::DrawHistos(fitter, true, true, true, true);

//     const float range[2] = {99.257812, 160.};
//     const float par[3] = {1,0.299188,17};
//     for (int i=0; i<5; ++i)
//     {
//         std::unique_ptr<TH1F> hB(fitter.GetModelHisto (range, "Npart", par, 1000000));
//         hB->SaveAs( "b_test.root" );
//     }
    
    std::cout << "END!" << std::endl;

    return 0;
}


























// int main(int argc, char *argv[])
// {
// 
//     const Float_t f0 = atof( argv[1]);
//     const Float_t f1 = atof( argv[1]);
//     const Int_t nsigma = atoi( argv[2] );
//     
//     TString glauber_filename = "~/Data/GlauberModel/PbPb/30AGeV/glau_pbpb_ntuple_signn_31.0_7.6AGeV_CM_30AGeV_LC.root";
//     TString qa_filename = "/home/vklochkov/Data/na61/na61_30_qa.root";
//     TString histoname = "reco_info/hMreco";
// 
//     const Int_t BinMin = atoi( argv[3] );
//     const Int_t BinMax = 400;
// 
//     const Int_t nf = 1;
//     const Int_t nEvents = 199000;
// 
// 
//     TString treename = "nt_Pb_Pb";
//     
//     std::unique_ptr<TFile> fSimFile{TFile::Open (glauber_filename.Data(), "read")};
//     std::unique_ptr<TTree> fSimTree{(TTree*) fSimFile->Get(treename.Data())};
//     
//     std::unique_ptr<TFile> f{TFile::Open( qa_filename.Data() )};    
// //     TH1F *hData1 = (TH1F*)f->Get("reco_info/hE");
//     TH1F *hData1 = (TH1F*)f->Get( histoname );
//    
//     Fitter fitter ( std::move(fSimTree) );
// 
// //     fitter.SetMode("PSD");
//     fitter.SetInputHisto (*hData1);
//     fitter.SetBinSize (1);
//     fitter.Init (nEvents);
//     
//     TString OutDir = ".";
//     
//     fitter.SetFitMinBin (BinMin);
//     fitter.SetFitMaxBin (BinMax);
//     fitter.SetOutDirName (OutDir);
// 
//     float par[3];
// //     fitter.FitGlauber(par, nf, f0, f1, nsigma, nEvents);
// //     Glauber::DrawHistos(fitter, true, true, true, true);
// 
//     const float mu[11] = {0.7,0.7,0.7,0.7,0.7,0.7,0.7,0.7,0.7,0.7,0.7};
//     const float k[11] = {1,3,5,7,9,12,15,18,21,30,1000};
//     
//     TFile fout("test_nbd.root", "recreate");
//     
//     for (int i=0; i<11; ++i)
//     {
//         fitter.SetNBDhist(mu[i], k[i]);
//         fitter.GetNBDHisto().Write();
//         
//         fitter.SetGlauberFitHisto (1, mu[i], k[i], nEvents, false);
//         fitter.GetGlauberFitHisto().Write();
//     }    
//     
//     const float range[2] = {100., 150.};
// //     const float par[3] = {1,0.76,70};
//     
// //     std::unique_ptr<TH1F> hB(fitter.GetModelHisto (range, "B", par, 100000));
// //     hB->SaveAs("test_b.root");
//     
//     std::cout << "END!" << std::endl;
// 
//     return 0;
// }
// 




