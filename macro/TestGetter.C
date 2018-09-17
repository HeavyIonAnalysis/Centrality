enum eEstimators{
    kPsd = 0,
    kTracks,
    kTracksPsd2D,
    nEstimators
};

void TestGetter(const TString file = "../build/test.root" )
{
    std::unique_ptr <TFile> f {TFile::Open(file, "read")};    

    std::unique_ptr <Centrality::Getter> getter1d {(Centrality::Getter*)f->Get("centr_getter_1d")};
    std::unique_ptr <Centrality::Getter> getter2d {(Centrality::Getter*)f->Get("centr_getter_2d")};
    
    Centrality::Container cont;
    
    for (uint iEvent = 0; iEvent<1; ++iEvent)
    {
        const float value1 = 150.;
        const float value2 = 20.;
                
        const float centrality1 = getter1d->GetCentrality(value1);
        const float centrality2 = getter2d->GetCentrality(value1, value2);
        
        cont.Clear();
        cont.AddCentralityEstimator(kTracks, centrality1);
        cont.AddCentralityEstimator(kTracksPsd2D, centrality2);
    }
    
    std::cout << "centrality (estimator 1) = " << cont.GetCentrality(kTracks) << std::endl;
    std::cout << "centrality (estimator 2) = " << cont.GetCentrality(kTracksPsd2D) << std::endl;
}
