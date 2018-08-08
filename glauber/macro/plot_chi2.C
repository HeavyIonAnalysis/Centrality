 
 
void plot_chi2(TString InFileName)
{
    gStyle->SetOptStat(0000);   

    TString qa_filename = "/home/vklochkov/Data/na61/na61_30_qa.root";
    TString histoname = "reco_info/hMreco";

    std::unique_ptr<TFile> fqa{TFile::Open( qa_filename.Data() )};    
    TH1F *hData1 = (TH1F*)fqa->Get( histoname );
    
    TFile *file = new TFile(InFileName);    
    TTree *tree = (TTree*) file->Get("test_tree");
    
    Float_t f, mu, k;
    Float_t chi2 ;
    Float_t sigma;
    TH1F *h1 = new TH1F ("h1", "", 500, 0, 500);
    
    tree->SetBranchAddress("f", &f);
    tree->SetBranchAddress("mu", &mu);  
    tree->SetBranchAddress("k", &k);
    tree->SetBranchAddress("chi2", &chi2);  
    tree->SetBranchAddress("sigma", &sigma);  
    tree->SetBranchAddress("histo", &h1);    
    
    Int_t n = tree->GetEntries();
    
    std::cout << " GetEntries = " << n << std::endl;
    
    std::vector <Float_t> x;
    std::vector <Float_t> y;
    std::vector <Float_t> z;
    
    for (Int_t i=0; i<n; i++)
    {
        tree->GetEntry(i);
        
        std::cout << " f = " << f  << " mu = " << mu  << " k = " << k  << " sigma = " << sigma  << " chi2 = " << chi2  << std::endl;
        
        x.push_back(f);
        y.push_back(k);
        z.push_back(chi2);
        
//         if ( chi2 > 2.5 ) continue;
        
        if ( false )
        {
            TCanvas *c1 = new TCanvas ( "c1", "c1", 1200, 800 ); 
            
            hData1->Draw();    
            h1->Draw("same");
            h1->GetYaxis()->SetTitle("counts");    
            h1->GetZaxis()->SetTitle("multiplicity");

            gPad->SetLogy();
            gPad->Update();
            hData1->SetLineColor(kRed);
            TLegend* leg1 = new TLegend(0.7, 0.75, 0.85, 0.89);    
            leg1->AddEntry(hData1, "M_{STS}", "l"); 
            leg1->AddEntry(h1, "MC-Glauber Fit", "l");
            leg1->Draw("same");     
            gPad->Update();
            
            c1->SaveAs("fit.root");
            c1->SaveAs("fit.C");
            
        }
        
//         Int_t jj;
//         std::cin >> jj;
//         if (jj == 0) break;
    }
    
    TCanvas *c1 = new TCanvas("c1", "c1", 800, 800);
    
    TGraph2D *g = new TGraph2D(x.size(), &(x[0]), &(y[0]), &(z[0]));
    g->SetName("#chi^{2} vs f, k");
    g->SetTitle("#chi^{2} vs f, k; f; k; #chi^{2}");
    
//     gPad->SetLogz();    
    g->Draw("colz");
    gPad->Update();

}
