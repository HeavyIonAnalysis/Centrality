const int nPoints = 10000;


void TreeReader(Int_t MultMin = 130, Int_t Mode = 2)
{
    TH1F *h1d_Mult_TPC_Ref/* = new TH1F("h1d_Mult_TPC_Ref","M_{TPC}^{ref}", 125, 0, 250)*/;
    TFile *f111 = new TFile("/lustre/nyx/cbm/users/klochkov/soft/CentralityFramework/Macro/hTPC_ref.root");    
    h1d_Mult_TPC_Ref = (TH1F*)f111->Get("h1Corr");
    h1d_Mult_TPC_Ref->GetYaxis()->SetRangeUser(0.1, 1e5);

    
    TCanvas *c1 = new TCanvas("c1", "canvas", 1400, 1000);
    
    if (Mode ==2){
        c1->Divide(2,1);
        TPad *c1_1 = (TPad*) c1->GetListOfPrimitives()->FindObject("c1_1");
        c1_1->SetLogy(1);
    }

    TString filename = Form ( "MinMult_%d/merged_%d.root", MultMin, MultMin);
        
    TFile *file = new TFile(filename);    
//     TTree *tree = file->Get("fit_tree");
    TTree *tree = file->Get("test_tree");
    
    TH1F *h1 = new TH1F ("h1", "", 125, 0, 250);
    TH1F *h3 = new TH1F ("h3", "", 125, 0, 250);
    for (Int_t i=0; i<125; i++)
        h3->SetBinContent(i+1, 1);
    h3->SetLineColor(3);
    
    TH1F *hDiff = new TH1F ("hDiff", "", 125, 0, 250);
    hDiff->GetYaxis()->SetRangeUser(0.0, 2);

    Float_t f, mu, k;
    Float_t chi2 ;
    Float_t sigma;
    
    tree->SetBranchAddress("f", &f);
    tree->SetBranchAddress("mu", &mu);  
    tree->SetBranchAddress("k", &k);
    tree->SetBranchAddress("chi2", &chi2);  
    tree->SetBranchAddress("sigma", &sigma);  
    tree->SetBranchAddress("histo", &h1);    
        
    Int_t n = tree->GetEntries();
    
    TH2F *h2 = new TH2F ("h2", "",  20, 0., 1., 50, 0., 1);
    h1d_Mult_TPC_Ref->GetYaxis()->SetRangeUser(0.1, 1e6);
    h1d_Mult_TPC_Ref->SetLineColor(2);
    
    Float_t x[nPoints], y[nPoints], z[nPoints];
    for (Int_t i=0; i<nPoints; i++)
        x[i] = y[i] = z[i] = 0.0;
    
    for (Int_t i=0; i<n; i++)
    {
        tree->GetEntry(i);
        

        if (Mode == 2){
            if (chi2 > 1)  continue;
            c1->cd(1);
            h1d_Mult_TPC_Ref->Draw();    
            h1->Draw("same");
            gPad->Update();
        }      
//         sigma = mu*(1+mu/k);
        for (Int_t ii=0; ii<125; ii++)
            if (h1d_Mult_TPC_Ref->GetBinContent(ii+1) > 0)
            {
                Float_t diff1 = ( h1->GetBinContent(ii+1) / h1d_Mult_TPC_Ref->GetBinContent(ii+1) );
                hDiff->SetBinContent (ii+1, diff1);
            }
            
        if (Mode == 2){
            c1->cd(2);    
            hDiff->Draw();
            h3->Draw("same");
            gPad->Update();
        }
        
        
        Int_t f_bin = TMath::Nint(f*20) + 1 ;
        Int_t k_bin = int(k/0.02 + 0.01) ;
        Int_t sigma_bin = TMath::Nint(sigma*50) + 1 ;

//         if (chi2 < 2)
            std::cout << "f = " << f << "    mu = " << mu << "    k = " << k << "    sigma = " << sigma << "    chi2 = " << chi2 << std::endl;    
//         std::cout << "f = " << f_bin << "    mu = " << mu << "    k = " << k_bin << "    sigma = " << sigma_bin << std::endl;    

        x[i] = f;
        y[i] = sigma;
        z[i] = chi2;
        
//         if (chi2 < 50)
        h2->SetBinContent (f_bin, sigma_bin, chi2);
        
        if (Mode == 2){
            Int_t jj;
            std::cin >> jj;
            if (jj == 0) break;
        }
    }   
        
    if (Mode == 1){
        TGraph2D *g = new TGraph2D(nPoints, x, y, z);
        g->GetXaxis()->SetTitle("f");
        g->GetYaxis()->SetTitle("sigma");
        g->Draw("colz");
    //     h2->Draw("surf1");
    }
}
