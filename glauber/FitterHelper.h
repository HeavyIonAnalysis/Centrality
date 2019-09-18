/** @file   FitterHelper.h
    @author Viktor Klochkov (klochkov44@gmail.com)
    @author Ilya Selyuzhenkov (ilya.selyuzhenkov@gmail.com)
    @brief  Methods for fit QA
*/

#ifndef GlauberFitterHelper_H
#define GlauberFitterHelper_H 1

#include <map>
#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "TPad.h"
#include "TLegend.h"
#include "TFile.h"

#include "Fitter.h"


namespace Glauber
{
    inline void DrawHistos (const Fitter& fit, float par[4], float chi2, Bool_t isSim, Bool_t isData, Bool_t isGlauber, Bool_t isNBD)
    {
        std::unique_ptr <TCanvas> c1 {new TCanvas("c1", "canvas", 1500, 900)};

        c1->Divide(2,2);
        
        std::unique_ptr <TPad> c1_1 {(TPad*) c1->GetListOfPrimitives()->FindObject("c1_1")};
        std::unique_ptr <TPad> c1_2 {(TPad*) c1->GetListOfPrimitives()->FindObject("c1_2")};
        std::unique_ptr <TPad> c1_4 {(TPad*) c1->GetListOfPrimitives()->FindObject("c1_4")};

        c1_1->SetLogy(1);
        c1_2->SetLogy(1);
        c1_4->SetLogy(1);
        
        /*const*/ TH1F hGlaub = fit.GetGlauberFitHisto();
        /*const*/ TH1F hData = fit.GetDataHisto();
        /*const*/ TH1F hNBD = fit.GetNBDHisto();
        /*const*/ TH1F hNcoll = *((fit.GetMapOfGlauber_Parameters_Histos()).at("Ncoll"));
        /*const*/ TH1F hNpart = *((fit.GetMapOfGlauber_Parameters_Histos()).at("Npart"));
        /*const*/ TH1F hBestFit = fit.GetBestFitHisto();

		  TH2F* BestHistos[kGP];
		  for (int i=0; i<kGP; i++) BestHistos[i]=(fit.GetMapOfGlauber_Parameters_VS_Multiplicity_BestHistos()).at(gGlauberParameters[i].name);

        std::unique_ptr <TFile> fOut{TFile::Open(Form("glauber_qa%s.root", fit.GetOutFileIDName().Data()), "recreate")}; 

        if (isSim){
            c1->cd(1);
            hNcoll.SetLineColor(2);
            
            hNcoll.Draw();
            hNpart.Draw("same");
            
            std::unique_ptr <TLegend> legSim { new TLegend(0.6,0.75,0.75,0.83) }; 
            legSim->AddEntry(&hNpart ,"Npart", "l");    
            legSim->AddEntry(&hNcoll ,"Ncoll", "l");    
            legSim->Draw("same");
            
            hNcoll.Write();
            hNpart.Write();
        }
        
        if (isData){
            c1->cd(2);
            hData.Draw();
            hData.Write();
            if (isGlauber){
                hBestFit.SetLineColor (kRed);
                hBestFit.Draw("same");
                
                std::unique_ptr <TLegend>  legData { new TLegend(0.6,0.75,0.75,0.83) };
                legData->AddEntry(&hBestFit ,"Fit", "l");    
                legData->AddEntry(&hData ,"Data", "l");    
                legData->Draw("same");   
                hBestFit.Write();
		for (int i=0; i<kGP; i++) BestHistos[i]->Write();
            }
        }
        
        if (isNBD){
            c1->cd(3);
            hNBD.Draw();
            hNBD.Write();

        }
        
        if (isGlauber){
            c1->cd(4);
            hBestFit.Draw();
        }

	TTree *BestResult=new TTree("BestResult", "BestResult");
	Float_t mu, f, k, chi2_error;
	BestResult -> Branch("mu", &mu);
    	BestResult -> Branch("f", &f);
    	BestResult -> Branch("k", &k);
    	BestResult -> Branch("chi2", &chi2);
    	BestResult -> Branch("chi2_error", &chi2_error);

    	mu=par[1]; f=par[0]; k=par[2]; chi2_error=par[3];

    	BestResult -> Fill();
	BestResult -> Write();

        c1->Write();
        c1->SaveAs(Form("glauber%s.pdf", fit.GetOutFileIDName().Data()));
        fOut->Close();
    }
    
    
}






#endif
