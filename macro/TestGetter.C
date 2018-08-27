
void TestGetter(const TString file = "../build/test.root", const TString infile = "../input/test_input.root" )
{
    std::unique_ptr <TFile> f {TFile::Open(file, "read")};    
    std::unique_ptr <Centrality::Getter> getter {(Centrality::Getter*)f->Get("centr_getter")};
    
    const float value = 20.;
    const float centrality = getter->GetCentrality(value);
    std::cout << "value = " << value << " centrality = " << centrality << std::endl;
}
