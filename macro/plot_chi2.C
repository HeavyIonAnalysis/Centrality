void plot_chi2(const std::string& filename) {
  TFile* fileIn = TFile::Open(filename.c_str(), "read");
  TTree* treeIn = fileIn->Get<TTree>("test_tree");

  double f, mu, k, chi2;
  treeIn->SetBranchAddress("f", &f);
  treeIn->SetBranchAddress("mu", &mu);
  treeIn->SetBranchAddress("k", &k);
  treeIn->SetBranchAddress("chi2", &chi2);

  double opt_chi2 = 1e9;// a very large number
  double opt_f, opt_k, opt_mu;

  TGraph2D* graph = new TGraph2D();
  graph->GetXaxis()->SetTitle("f");
  graph->GetYaxis()->SetTitle("k");

  for (int i = 0; i < treeIn->GetEntries(); i++) {
    treeIn->GetEntry(i);
    if (chi2 < opt_chi2) {
      opt_chi2 = chi2;
      opt_f = f;
      opt_k = k;
      opt_mu = mu;
    }
    graph->AddPoint(f, k, chi2);
  }

  std::cout << "opt_chi2 = " << opt_chi2 << ", opt_f = " << opt_f << ", opt_k = " << opt_k << ", opt_mu = " << opt_mu << "\n";
  graph->Draw("colz");
}
