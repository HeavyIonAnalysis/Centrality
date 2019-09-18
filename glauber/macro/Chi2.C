void Chi2(TString InFileName)
{
    TFile *file = new TFile(InFileName);    
    TTree *tree = (TTree*) file->Get("test_tree");
    TTree *delta = new TTree("delta","delta");
    
    Float_t f, mu, k;
    Float_t chi2, chi2_error, chi2_min_error, CHI2, DELTA=1e10 ;
    Float_t chi2_min=1e10;
    Float_t f_min, mu_min, k_min;
    Float_t f_max=0, mu_max=0, k_max=0;
    Float_t f_delta=0, mu_delta=0, k_delta=0;
    Float_t sigma;
    
    tree->SetBranchAddress("f", &f);
    tree->SetBranchAddress("mu", &mu);  
    tree->SetBranchAddress("k", &k);
    tree->SetBranchAddress("chi2", &chi2);
    tree->SetBranchAddress("chi2_error", &chi2_error);  
    tree->SetBranchAddress("sigma", &sigma);

    delta->Branch("chi2", &chi2_min);
    delta->Branch("delta_chi2", &chi2_min_error);
    delta->Branch("mu", &mu_min);
    delta->Branch("delta_mu", &mu_delta);
    delta->Branch("k", &k_min);
    delta->Branch("delta_k", &k_delta);
    delta->Branch("f", &f_min);
    delta->Branch("delta_f", &f_delta);      
    
    Int_t n = tree->GetEntries();
    
    std::cout << " GetEntries = " << n << std::endl;
    
    std::vector <Float_t> x;
    std::vector <Float_t> y;
    std::vector <Float_t> z;
    
    int X, Y;
    for (Int_t i=0; i<n; i++)
    {
        tree->GetEntry(i);
        
        std::cout << " f = " << f << " mu = " << mu << " k = " << k << " sigma = " << sigma << " chi2 = " << chi2 << " chi2_error = " << chi2_error << std::endl;
        
        x.push_back(f);
        y.push_back(k);
        z.push_back(chi2);

	if (chi2<chi2_min) {chi2_min=chi2; chi2_min_error=chi2_error; CHI2=chi2_min+chi2_min_error; f_min=f; mu_min=mu; k_min=k; X=f; Y=k;}
    }

    
    
    TCanvas *c1 = new TCanvas("c1", "c1", 800, 800);
    
    TGraph2D *g = new TGraph2D(x.size(), &(x[0]), &(y[0]), &(z[0]));
    g->SetName("#chi^{2} vs f, k");
    g->SetTitle("#chi^{2} vs f, k; f; k; #chi^{2}");
        
    g->Draw("colz");

    float F,K;
    for (double xx=X; (xx<=X+0.2 && xx<=g->GetXmax()); xx+=0.001) {
    	for (double yy=Y; (yy<=Y+10.0 && yy<=g->GetYmax()); yy+=0.001) if (TMath::Abs(g->Interpolate(xx,yy)-CHI2)<DELTA) {
		if (TMath::Abs(xx-f_min)>f_delta) f_delta=TMath::Abs(xx-f_min); 
		if (TMath::Abs(yy-k_min)>k_delta) k_delta=TMath::Abs(yy-k_min);
		F=1e10; K=1e10;
		for (Int_t ii=0; ii<n; ii++) {tree->GetEntry(ii); if (TMath::Abs(f-xx)<=F && TMath::Abs(k-yy)<=K && (TMath::Abs(mu-mu_min))>mu_delta) mu_delta=TMath::Abs(mu-mu_min);}
		DELTA=TMath::Abs(g->Interpolate(xx,yy)-CHI2);
		}
	for (double yy=Y; (yy>=Y-10.0 && yy>=g->GetYmin()); yy-=0.001) if (TMath::Abs(g->Interpolate(xx,yy)-CHI2)<DELTA) {
		if (TMath::Abs(xx-f_min)>f_delta) f_delta=TMath::Abs(xx-f_min); 
		if (TMath::Abs(yy-k_min)>k_delta) k_delta=TMath::Abs(yy-k_min);
		F=1e10; K=1e10;
		for (Int_t ii=0; ii<n; ii++) {tree->GetEntry(ii); if (TMath::Abs(f-xx)<=F && TMath::Abs(k-yy)<=K && (TMath::Abs(mu-mu_min))>mu_delta) mu_delta=TMath::Abs(mu-mu_min);}
		DELTA=TMath::Abs(g->Interpolate(xx,yy)-CHI2);
		}
	}

    for (double xx=X; (xx>=X-0.2 && xx>=g->GetXmin()); xx-=0.001) {
    	for (double yy=Y; (yy<=Y+2.0 || yy<=g->GetYmax()); yy+=0.001) if (TMath::Abs(g->Interpolate(xx,yy)-CHI2)<DELTA) {
		if (TMath::Abs(xx-f_min)>f_delta) f_delta=TMath::Abs(xx-f_min); 
		if (TMath::Abs(yy-k_min)>k_delta) k_delta=TMath::Abs(yy-k_min);
		F=1e10; K=1e10;
		for (Int_t ii=0; ii<n; ii++) {tree->GetEntry(ii); if (TMath::Abs(f-xx)<=F && TMath::Abs(k-yy)<=K && (TMath::Abs(mu-mu_min))>mu_delta) mu_delta=TMath::Abs(mu-mu_min);}
		DELTA=TMath::Abs(g->Interpolate(xx,yy)-CHI2);
		}
	for (double yy=Y; (yy>=Y-2.0 && yy>=g->GetYmin()); yy-=0.001) if (TMath::Abs(g->Interpolate(xx,yy)-CHI2)<DELTA) {
		if (TMath::Abs(xx-f_min)>f_delta) f_delta=TMath::Abs(xx-f_min); 
		if (TMath::Abs(yy-k_min)>k_delta) k_delta=TMath::Abs(yy-k_min);
		F=1e10; K=1e10;
		for (Int_t ii=0; ii<n; ii++) {tree->GetEntry(ii); if (TMath::Abs(f-xx)<=F && TMath::Abs(k-yy)<=K && (TMath::Abs(mu-mu_min))>mu_delta) mu_delta=TMath::Abs(mu-mu_min);}
		DELTA=TMath::Abs(g->Interpolate(xx,yy)-CHI2);
		}
	}
    
    g->Draw("colz");

    std::cout << " f = " << f_min << "+/-" << f_delta << " mu = " << mu_min << "+/-" << mu_delta << " k = " << k_min << "+/-" << k_delta << " chi2 = " << chi2_min << "+/-" << chi2_min_error << std::endl;
    
    TFile *f1 = new TFile("Fit_Errors_RPC.root", "recreate");
    delta->Write();
    g->Write();
    f1->Close();  

    
}
