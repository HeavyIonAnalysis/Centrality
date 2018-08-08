 
 
void RunGlauberAna( TString InFileName = "root_files/MinMult_60/merged.root")
{
    TString CentralityFrameworkDir = "/lustre/nyx/cbm/users/klochkov/soft/CentralityFramework/";
//     TString DataFileName = CentralityFrameworkDir + "containers/ana_dima_merged.root";
    TString DataFileName = "/lustre/nyx/cbm/users/klochkov/soft/PidFramework/input/DCM_1M.root";
    gStyle->SetOptStat(0000);   
    
    TFile *DataFile = new TFile ( DataFileName, "read" );    
    CentralityEventContainer *container = new CentralityEventContainer;
    TTree *ContTree = (TTree*)DataFile->Get("cbm_data");
    ContTree->SetBranchAddress("CentralityEventContainer", &container);
    ContTree->Draw("CentralityEventContainer.GetDetectorWeight(3) >> h1(500, 0, 500)");
    TH1F *hData1 = (TH1F*)gPad->GetPrimitive("h1");
    
    TFile *file = new TFile(InFileName);    
    TTree *tree = file->Get("test_tree");
    
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
    
    std::vector <Double_t> x;
    std::vector <Double_t> y;
    std::vector <Double_t> z;
    
    for (Int_t i=0; i<n; i++)
    {
        tree->GetEntry(i);
        
        std::cout << " f = " << f  << " mu = " << mu  << " k = " << k  << " sigma = " << sigma  << " chi2 = " << chi2  << std::endl;
        
        x.push_back(f);
        y.push_back(sigma);
        z.push_back(chi2);
        
        if ( chi2 > 2.5 ) continue;
        
        if ( true )
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
        
        Int_t jj;
        std::cin >> jj;
        if (jj == 0) break;
    }
    
    TGraph2D *g = new TGraph2D(x.size(), &(x[0]), &(y[0]), &(z[0]));
    g->GetXaxis()->SetTitle("f");
    g->SetName(" ");
    g->SetMinimum(5);
    g->GetYaxis()->SetTitle("#sigma");    
    g->GetZaxis()->SetTitle("#chi^{2}");
    gPad->SetLogz();    
    g->Draw("colz");
    gPad->Update();

}