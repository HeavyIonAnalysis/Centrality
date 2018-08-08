


void HistoCompare ()
{
    TFile *f = new TFile("root_files/out_histo_2.root");    

    
    TH1F* hData = (TH1F*)f->Get("h1d_Mult_TPC_Ref");
    
    TH1F* h0 = new TH1F ("h0", "", 125, 0, 250);
    TH1F* h1 = (TH1F*)f->Get("glaub_0.25_0.1709_0.78_1000000");
    TH1F* h2 = (TH1F*)f->Get("glaub_0.80_0.3048_0.75_1000000");
    TH1F* h3 = (TH1F*)f->Get("glaub_0.80_0.3048_0.75_1000000");

    TH1F* hDiff1 = new TH1F ("diff1", "", 125, 0, 250);
    TH1F* hDiff2 = new TH1F ("diff2", "", 125, 0, 250);
    TH1F* hDiff3 = new TH1F ("diff3", "", 125, 0, 250);
    
    for (Int_t ii=0; ii<125; ii++)
    {
        if (hData->GetBinContent(ii+1) > 0)
        {
            Float_t diff1 = ( h1->GetBinContent(ii+1) / hData->GetBinContent(ii+1) );
            hDiff1->SetBinContent (ii+1, diff1);
           
            Float_t diff2 = ( h2->GetBinContent(ii+1) / hData->GetBinContent(ii+1) );
            hDiff2->SetBinContent (ii+1, diff2);

            Float_t diff3 = ( h3->GetBinContent(ii+1) / hData->GetBinContent(ii+1) );
            hDiff3->SetBinContent (ii+1, diff3);
            
            h0->SetBinContent (ii+1, 1);
        }
    }
    
    TCanvas *c1 = new TCanvas("c1", "canvas", 1500, 900);
    
    hDiff1->SetLineColor(2);
    hDiff2->SetLineColor(3);
    hDiff3->SetLineColor(4);
    hDiff1->Draw();
    hDiff2->Draw("same");
//     hDiff3->Draw("same");
    h0->Draw("same");
    
    TLegend* legDiff = new TLegend(0.4,0.75,0.75,0.9 );
    legDiff->AddEntry(hDiff1 ,"f = 0.25 mu = 0.1709 k = 0.78", "l");    
    legDiff->AddEntry(hDiff2 ,"f = 0.80 mu = 0.3048 k = 0.75", "l");    
//     legDiff->AddEntry(hDiff3 ,"f = 1", "l");    
    legDiff->Draw("same");    

    TCanvas *c2 = new TCanvas("c2", "canvas", 1500, 900);
    
    h1->SetLineColor(2);
    h2->SetLineColor(3);
    h3->SetLineColor(5);
    hData->SetLineColor(4);

    h1->Draw();
    hData->Draw("same");    
    h2->Draw("same");
//     h3->Draw("same");

    TLegend* legData = new TLegend(0.4,0.75,0.75,0.9 );
    legData->AddEntry(h1 ,"f = 0.25 mu = 0.1709 k = 0.78", "l");    
    legData->AddEntry(h2 ,"f = 0.80 mu = 0.3048 k = 0.75", "l");    
//     legData->AddEntry(h3 ,"f = 1", "l");    
    legData->AddEntry(hData ,"data", "l");    
    legData->Draw("same");    
}