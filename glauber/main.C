#include "CentralityGetter.h"
#include "CentralitySlicesFinder.h"
#include "CentralityManager.h"

#include "glauber/Fitter.h"
#include "glauber/FitterHelper.h"

#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include "TLegend.h"
#include "TH2.h"

using namespace Glauber;

const std::vector <float> slices = 
{
    99.257812,
    83.917968,
    70.984375,
    59.554687,
    49.628906,
    41.056640,
    33.386718,
    26.919921,
    21.430664,
    16.84375,
    13.083984,
    10.000976,
    7.5571289,
    5.6020507,
    4.0229492,
    2.7446289,
    1.6354980,
    0.7707519,
    0.2161865
};


int main(int argc, char *argv[])
{
    const Float_t f0 = atof( argv[1]);
    const Int_t k0 = atoi( argv[2] );
    const Int_t k1 = atoi( argv[3] );
    const Int_t BinMin = atoi( argv[4] );
    
    TString mode = "Default";
    
    TString glauber_filename = "/home/vklochkov/Data/glauber/pbpb/30agev/glau_pbpb_ntuple_signn_31.0_7.6AGeV_CM_30AGeV_LC.root";
    TString qa_filename = "/home/vklochkov/Data/na61/pbpb/30agev/qa/na61_30_qa.root";
    TString histoname;    
        
    if (mode == "PSD")    
        histoname = "reco_info/hPSDMultCorr_PSD1";
    else
        histoname = "reco_info/hMreco";

    const Int_t BinMax = 400;
    const Int_t nEvents = 199000;

    TString treename = "nt_Pb_Pb";
    
    std::unique_ptr<TFile> fSimFile{TFile::Open (glauber_filename.Data(), "read")};
    std::unique_ptr<TTree> fSimTree{(TTree*) fSimFile->Get(treename.Data())};
    std::unique_ptr<TFile> f{TFile::Open( qa_filename.Data() )};    

    TH1F *hData1;
    
    if (mode == "PSD")  
    {
        hData1 = (TH1F*) ((TH2F*)f->Get( histoname ))->ProjectionY("_y", 20, 400);
        for (int i=0; i<5; ++i) hData1->SetBinContent(i+1, 0);
        for (int i=500; i<1000; ++i) hData1->SetBinContent(i+1, 0);
    }
    else
        hData1 = (TH1F*)f->Get( histoname );

    
    
    
    Fitter fitter ( std::move(fSimTree) );

    fitter.SetMode(mode);
    fitter.SetInputHisto (*hData1);
    fitter.SetBinSize (1);
    fitter.Init (nEvents);
    
    TString OutDir = ".";
    
    fitter.SetFitMinBin (BinMin);
    fitter.SetFitMaxBin (BinMax);
    fitter.SetOutDirName (OutDir);

    float par[3];
    fitter.FitGlauber(par, f0, k0, k1, nEvents);
    Glauber::DrawHistos(fitter, true, true, true, true);

//     const float mu[12] = {0.1,0.2,0.3,0.4,0.7,0.71,0.74,0.79,0.87,0.89,0.91,0.99};
//     const float k[12] = {10000,10000,10000,10000,10000,10000,10000,10000,10000,10000,10000,10000};
//     
//     TFile fout("test_nbd.root", "recreate");
//     
//     for (int i=8; i<12; ++i)
//     {
// //         fitter.SetNBDhist(mu[i], k[i]);
// //         fitter.GetNBDHisto().Write();
//         
// //         fitter.SetGlauberFitHisto (1, mu[i], k[i], nEvents, false);
// //         fitter.GetGlauberFitHisto().Write();
//     }    
//     
    const float range[2] = {99.257812, 160.};
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




