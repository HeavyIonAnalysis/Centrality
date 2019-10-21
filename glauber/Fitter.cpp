#include "Fitter.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TFile.h"
#include "TGraph.h"
#include "TLegend.h"
#include "TMath.h"
#include "TRandom.h"
#include <map>

ClassImp(Glauber::Fitter)

void Glauber::Fitter::Init(std::unique_ptr<TTree> tree)
{

    fSimTree = std::move(tree);
    
    if (!fSimTree) 
    {
        std::cout << "SetSimHistos: *** Error - " << std::endl;
        exit(EXIT_FAILURE);
    }
    
    std::map <TString, Float_t>::iterator it0=Glauber_Parameters.begin();
    for (int i=0; i<kGP; i++) 
    {
    	fSimTree->SetBranchAddress(it0->first,  &(it0->second)); 
	it0++;
    }

    if ( fnEvents < 0 || fnEvents > fSimTree->GetEntries() )
    {
        std::cout << "Init: *** ERROR - number of entries < 0 or less than number of entries in input tree" << std::endl;
        std::cout << "Init: *** number of entries in input tree = " << fSimTree->GetEntries() << std::endl;        
        exit(EXIT_FAILURE);
    }

    it0=Glauber_Parameters.begin();
    for (int i=0; i<kGP; i++) 
    {
	Glauber_ParametersMax.insert( std::pair<TString, const int> ((it0->first), int (fSimTree->GetMaximum(it0->first))) );
        Glauber_ParametersMin.insert( std::pair<TString, const int> ((it0->first), int (fSimTree->GetMinimum(it0->first))) );
	it0++;
    }
    
    it0=Glauber_Parameters.begin();
    std::map <TString, const int>::iterator it1=Glauber_ParametersMin.begin();
    std::map <TString, const int>::iterator it2=Glauber_ParametersMax.begin();
    for (int j=0; j<kGP; j++) 
    {
    	for (int i=0; i<kGP; i++) if (gGlauberParameters[i].name == it2->first) Glauber_Parameters_Histos.insert( std::pair<TString, TH1F*> ((it2->first), new TH1F (Form("%s_Histo", (gGlauberParameters[i].name).Data()), Form("%s;%s;counts", (gGlauberParameters[i].title).Data(), (gGlauberParameters[i].axis_title).Data()), 1.3*((it2->second)-(it1->second))/gGlauberParameters[i].bin_value, 1.3*it1->second, 1.3*it2->second)) );
    	it1++;
	it2++;
    }

    it0=Glauber_Parameters.begin();
    it1=Glauber_ParametersMin.begin();
    it2=Glauber_ParametersMax.begin();		
    std::map <TString, TH1F*>::iterator it3=Glauber_Parameters_Histos.begin();
    for (int i=0; i<fnEvents; i++)
    {
       	fSimTree->GetEntry(i);
	it0=Glauber_Parameters.begin();
	it3=Glauber_Parameters_Histos.begin();
	for (int j=0; j<kGP; j++) 
	{
		(it3->second)->Fill(it0->second);
		it0++;
		it3++;
	}
    }

    std::cout << "Entries in GlauberTree:" << fSimTree->GetEntries() << std::endl;

    fNbins = fDataHisto.GetNbinsX();

    while ( fDataHisto.GetBinContent(fNbins-1) == 0)
        fNbins--;

    fNbins++;

    const float min = fDataHisto.GetXaxis()->GetXmin();
    const float max = fDataHisto.GetXaxis()->GetXmax();

    fMaxValue = min + (max - min)*fNbins/fDataHisto.GetNbinsX() ;

    it0=Glauber_Parameters.begin();
    it1=Glauber_ParametersMin.begin();
    it2=Glauber_ParametersMax.begin();
    it3=Glauber_Parameters_Histos.begin();
    for (int j=0; j<kGP; j++) 
    {
	for (int i=0; i<kGP; i++) if (gGlauberParameters[i].name == it2->first) Glauber_Parameters_VS_Multiplicity_Histos.insert( std::pair<TString, TH2F*> ((it2->first), new TH2F (Form("%s_VS_Multiplicity_Histo", (gGlauberParameters[i].name).Data()), Form("%s VS Multiplicity;Multiplicity;%s", (gGlauberParameters[i].title).Data(), (gGlauberParameters[i].axis_title).Data()), 1.3*fNbins, 0, 1.3*fMaxValue, 1.3*((it2->second)-(it1->second))/gGlauberParameters[i].bin_value, 1.3*it1->second, 1.3*it2->second)) );
    	it1++;
	it2++;
    }

    it0=Glauber_Parameters.begin();
    it1=Glauber_ParametersMin.begin();
    it2=Glauber_ParametersMax.begin();
    it3=Glauber_Parameters_Histos.begin();
    for (int j=0; j<kGP; j++) 
    {
	for (int i=0; i<kGP; i++) if (gGlauberParameters[i].name == it2->first) Glauber_Parameters_VS_Multiplicity_BestHistos.insert( std::pair<TString, TH2F*> ((it2->first), new TH2F (Form("%s_VS_Multiplicity_BestHisto", (gGlauberParameters[i].name).Data()), Form("%s VS Multiplicity;Multiplicity;%s", (gGlauberParameters[i].title).Data(), (gGlauberParameters[i].axis_title).Data()), 1.3*fNbins, 0, 1.3*fMaxValue, 1.3*((it2->second)-(it1->second))/gGlauberParameters[i].bin_value, 1.3*it1->second, 1.3*it2->second)) );
    	it1++;
	it2++;
    }

    std::cout << "fNbins = " << fNbins << std::endl;
    std::cout << "fMaxValue = " << fMaxValue << std::endl;
}

float Glauber::Fitter::Nancestors(float f) const
{
    if       (fAncestor_Mode == "Default")    return f*Glauber_Parameters.at("Npart") + (1-f)*Glauber_Parameters.at("Ncoll");
    else if  (fAncestor_Mode == "PSD")        return f-Glauber_Parameters.at("Npart");
    else if  (fAncestor_Mode == "Npart")      return TMath::Power(Glauber_Parameters.at("Npart"), f); 
    else if  (fAncestor_Mode == "Ncoll")      return TMath::Power(Glauber_Parameters.at("Ncoll"), f);
    
    return -1.;
}

float Glauber::Fitter::NancestorsMax(float f) const
{
    const int NpartMax = Glauber_Parameters_Histos.at("Npart")->GetXaxis()->GetXmax() ;  // some magic
    const int NcollMax = Glauber_Parameters_Histos.at("Ncoll")->GetXaxis()->GetXmax() ; //TODO 
    
    if       (fAncestor_Mode == "Default")    return f*NpartMax + (1-f)*NcollMax;
    else if  (fAncestor_Mode == "PSD")        return f;
    else if  (fAncestor_Mode == "Npart")      return TMath::Power(NpartMax, f); 
    else if  (fAncestor_Mode == "Ncoll")      return TMath::Power(NcollMax, f);
    
    return -1.;
}

/*
 * take Glauber MC data from fSimTree
 * Populate fGlauberFitHisto with NBD x Na
 */

void Glauber::Fitter::SetGlauberFitHisto (float f, float mu, float k, Bool_t Norm2Data)
{    
    fGlauberFitHisto = TH1F("glaub", "", fNbins*1.3, 0, 1.3*fMaxValue);
    
    fGlauberFitHisto.SetName("glaub_fit_histo");
    
    SetNBDhist(mu,  k);

    std::unique_ptr<TH1F> htemp {(TH1F*)fNbdHisto.Clone("htemp")}; // WTF??? Not working without pointer
    for (int i=0; i<fnEvents; i++)
    {
        fSimTree->GetEntry(i);
        const int Na = int(Nancestors(f));
                
        float nHits {0.};
        for (int j=0; j<Na; j++) nHits += int(htemp->GetRandom());
        fGlauberFitHisto.Fill(nHits);
	
	std::map <TString, Float_t>::iterator it1=Glauber_Parameters.begin();
	std::map <TString, TH2F*>::iterator it2=Glauber_Parameters_VS_Multiplicity_Histos.begin();
	for (int q=0; q<kGP; q++) 
	{
		(it2->second)->Fill(nHits, it1->second);
		it1++;
		it2++;
	}

    }
    if (Norm2Data)
        NormalizeGlauberFit();
}


void Glauber::Fitter::NormalizeGlauberFit ()
{
    
    int fGlauberFitHistoInt {0}; 
    int fDataHistoInt {0};
    
    const int lowchibin = fFitMinBin;
    const int highchibin = fFitMaxBin<fNbins ? fFitMaxBin : fNbins;
    
    for (int i=lowchibin; i<highchibin; i++)
    {
        fGlauberFitHistoInt += fGlauberFitHisto.GetBinContent(i+1);
        fDataHistoInt += fDataHisto.GetBinContent(i+1);
    }

    const float ScaleFactor = (float)fDataHistoInt/fGlauberFitHistoInt;
    
//     std::cout << "Scale = " << Scale << std::endl;
    fGlauberFitHisto.Scale(ScaleFactor);    
}

/**
 *
 * @param mu mean value of negative binominal distribution (we are looking for it)
 * @param chi2 return value (indicates good match)
 * @param mu_min lower search edge for mean value NBD
 * @param mu_max upper search edge for mean value NBD
 * @param f parameter of Na
 * @param k NBD parameter
 */
void Glauber::Fitter::FindMuGoldenSection (TTree *tree, float *mu, float *chi2, float*chi2_error, int *n, float *sigma, float mu_min, float mu_max, float f, float k )
{
    const float phi {(1+TMath::Sqrt(5))/2};

    /* left */
    float mu_1 = mu_max - (mu_max-mu_min)/phi;

    /* right */
    float mu_2 = mu_min + (mu_max-mu_min)/phi;
    
    
    SetGlauberFitHisto (f, mu_1, k);
    float chi2_mu1 = GetChi2 ();
    float chi2_mu1_error = GetChi2Error ();
    
    *mu = mu_1;
    *chi2 = chi2_mu1;
    *chi2_error = chi2_mu1;
    *sigma = ( (*mu)/k + 1 ) * (*mu);
    tree->Fill();
    std::cout << "n = " << (*n) << " f = "  << f << " k = " << k << " mu = " << (*mu) << " chi2 = " << (*chi2)  << " chi2_error = " << (*chi2_error) << std::endl;

    SetGlauberFitHisto (f, mu_2, k);
    float chi2_mu2 = GetChi2 ();
    float chi2_mu2_error = GetChi2Error ();
    
    *mu = mu_2;
    *chi2 = chi2_mu2;
    *chi2_error = chi2_mu2;
    *sigma = ( (*mu)/k + 1 ) * (*mu);
    *n = *n + 1;
    tree->Fill();
    std::cout << "n = " << (*n) << " f = "  << f << " k = " << k << " mu = " << (*mu) << " chi2 = " << (*chi2)  << " chi2_error = " << (*chi2_error) << std::endl;
    
    for (int j=0; j<fnMuIter; j++)
    {        
        if (chi2_mu1 > chi2_mu2)
        {
            mu_min = mu_1;
            mu_1 = mu_2;
            mu_2 = mu_min + (mu_max-mu_min)/phi;
            chi2_mu1 = chi2_mu2;
            SetGlauberFitHisto (f, mu_2, k);
            chi2_mu2 = GetChi2 ();
	    chi2_mu2_error = GetChi2Error ();
            *n = *n + 1;
            *mu = mu_2;
            *chi2 = chi2_mu2;
            *chi2_error = chi2_mu2;
            *sigma = ( (*mu)/k + 1 ) * (*mu);
            tree->Fill();
        }
        else
        {
            mu_max = mu_2;
            mu_2 = mu_1;
            mu_1 = mu_max - (mu_max-mu_min)/phi; 
            chi2_mu2 = chi2_mu1;
            SetGlauberFitHisto (f, mu_1, k);
            chi2_mu1 = GetChi2 (); 
  	    chi2_mu1_error = GetChi2Error ();
            *n = *n + 1;
            *mu = mu_2;
            *chi2 = chi2_mu2;
            *chi2_error = chi2_mu2;
            *sigma = ( (*mu)/k + 1 ) * (*mu);
            tree->Fill();
        }
        
        std::cout << "n = " << (*n) << " f = "  << f << " k = " << k << " mu = " << (*mu) << " chi2 = " << (*chi2)  << " chi2_error = " << (*chi2_error) << std::endl;
    }

    /* take min(mu) */
    *mu = (chi2_mu1 < chi2_mu2) ? mu_1 : mu_2;
    /* take min(chi2) */
    *chi2 = (chi2_mu1 < chi2_mu2) ? chi2_mu1 : chi2_mu2;
    /* take min(chi2_error) */
    *chi2_error = (chi2_mu1 < chi2_mu2) ? chi2_mu1_error : chi2_mu2_error;
}

/**
 *
 * @param mu mean value of negative binominal distribution (we are looking for it)
 * @param chi2 return value (indicates good match)
 * @param mu_min lower search edge for mean value NBD
 * @param mu_max upper search edge for mean value NBD
 * @param f parameter of Na
 * @param k NBD parameter
 */
void Glauber::Fitter::FindMuIteration (TTree *tree, float mu, float *chi2, float *chi2_error, int *n, float *sigma, float f, float k)
{
    float chi2_best=1e10, chi2_error_best=1e10;
    SetGlauberFitHisto (f, mu, k);
    *sigma = ( mu/k + 1 ) * mu;
    for (int fRange = fMinFitRange; fRange < (fMaxMultiplicity-fMinMultiplicity); fRange = fRange + fMultiplicityStep)
        for (int fRangeCenter = fMinMultiplicity+fRange/2; fRangeCenter < (fMaxMultiplicity-fRange/2); fRangeCenter = fRangeCenter + fMultiplicityStep) {
            fFitMinBin = fRangeCenter - fRange/2;
            fFitMaxBin = fRangeCenter + fRange/2;
            *chi2 = GetChi2 ();
            *chi2_error = GetChi2Error ();
            tree->Fill();
            std::cout << "n = " << (*n) << " f = "  << f << " k = " << k << " mu = " << mu << " FitMinBin = " << fFitMinBin << " FitMaxBin = " << fFitMaxBin << " chi2 = " << (*chi2)  << " chi2_error = " << (*chi2_error) << std::endl;
            *n = *n + 1;
            if ((*chi2) < chi2_best) { chi2_best=(*chi2); chi2_error_best=(*chi2_error); }
        }
        
    /* take min(chi2) */
    *chi2 = chi2_best;
    /* take min(chi2_error) */
    *chi2_error = chi2_error_best;
}

/**
 * Find the best match
 *
 * @param return value of best fit parameters
 */
float Glauber::Fitter::FitGlauber (float *par)
{
    float f_fit{-1};
    float mu_fit{-1}; 
    float k_fit{-1};
    float Chi2Min {1e10};
    float Chi2Min_error {0};

    const TString filename = Form ( "%s/fit%s.root", fOutDirName.Data(), fOutFileIDName.Data() );
    
//     std::unique_ptr<TFile> file {TFile::Open(filename, "recreate")};    
//     std::unique_ptr<TTree> tree {new TTree("test_tree", "tree" )};

    TFile* file {TFile::Open(filename, "recreate")};    
    TTree* tree {new TTree("test_tree", "tree" )};
           
    float f, mu, k, chi2, chi2_error, sigma;
    int n=1;
    
    tree->Branch("n",    &n,    "n/F");
    tree->Branch("MinBin",    &fFitMinBin,    "fFitMinBin/F");
    tree->Branch("MaxBin",    &fFitMaxBin,    "fFitMaxBin/F");
    tree->Branch("f",    &f,    "f/F");   
    tree->Branch("mu",   &mu,   "mu/F");   
    tree->Branch("k",    &k,    "k/F");   
    tree->Branch("chi2", &chi2, "chi2/F");
    tree->Branch("chi2_error", &chi2_error, "chi2_error/F");
    tree->Branch("sigma",&sigma,"sigma/F");   

    
    
    for (float i=f_fMin; i<=f_fMax; i=i+f_fStep)
    {
	    f = i;
	    for (float j=f_kMin; j<=f_kMax; j=j+f_kStep)
	    {
		mu = fMaxValue / NancestorsMax(f) ;
		k = j;
		const float mu_min = f_MuMinPercentage*mu;
		const float mu_max = f_MuMaxPercentage*mu;

		if (fFit_Mode == "GoldenSection")  FindMuGoldenSection (tree, &mu, &chi2, &chi2_error, &n, &sigma, mu_min, mu_max, f, k);
                else if (fFit_Mode == "Iteration") for (float mu=mu_min; mu<=mu_max; mu=mu+f_muStep) FindMuIteration (tree, mu, &chi2, &chi2_error, &n, &sigma, f, k);
                else std::cout << "ERROR: ILLIGAL MU FINDING MODE" << std::endl;
		
		if (chi2 < Chi2Min)
		{
		    f_fit = f;
		    mu_fit = mu;
		    k_fit = k;
		    Chi2Min = chi2;
		    Chi2Min_error = chi2_error;
		    fBestFitHisto = fGlauberFitHisto;

		    std::map <TString, TH2F*>::iterator it1=Glauber_Parameters_VS_Multiplicity_BestHistos.begin();
		    std::map <TString, TH2F*>::iterator it2=Glauber_Parameters_VS_Multiplicity_Histos.begin();
		    for (int q=0; q<kGP; q++) 
		    {
	    	    	(it1->second)=(it2->second);
		    	it1++;
			it2++;
		    }
		}            

	    } 
    }

    tree->Write();
    file->Write();
    file->Close();

    par[0] = f_fit;
    par[1] = mu_fit;
    par[2] = k_fit;
    par[3] = Chi2Min_error;
    
    return Chi2Min;
}

/**
 * Compare fGlauberFitHisto with fDataHisto
 * @return chi2 value
 */
float Glauber::Fitter::GetChi2 () const
{
    float chi2 {0.0};

    const int lowchibin = fFitMinBin;
    const int highchibin = fFitMaxBin<fNbins ? fFitMaxBin : fNbins;

    for (int i=lowchibin; i<=highchibin; ++i) 
    {
        if (fDataHisto.GetBinContent(i) < 1.0) continue;
        const float error2 = TMath::Power(fDataHisto.GetBinError(i), 2) + TMath::Power(fGlauberFitHisto.GetBinError(i), 2);
//	std::cout<<"i="<<i<<"  DataError="<<fDataHisto.GetBinError(i)<<std::endl;
//	std::cout<<"i="<<i<<"  GlauberError="<<fGlauberFitHisto.GetBinError(i)<<std::endl;
        const float diff2 = TMath::Power((fGlauberFitHisto.GetBinContent(i) - fDataHisto.GetBinContent(i)), 2) / error2;
//	std::cout<<"i="<<i<<"  DataContent="<<fDataHisto.GetBinContent(i)<<std::endl;
//	std::cout<<"i="<<i<<"  GlauberContent="<<fGlauberFitHisto.GetBinContent(i)<<std::endl;

        chi2 += diff2;
    }
    
    chi2 = chi2 / (highchibin - lowchibin + 1);
    return chi2;
}

/**
 * Compare fGlauberFitHisto with fDataHisto
 * @return chi2 error value
 */
float Glauber::Fitter::GetChi2Error () const
{
    float chi2_error {0.0};

    const int lowchibin = fFitMinBin;
    const int highchibin = fFitMaxBin<fNbins ? fFitMaxBin : fNbins;
    
    for (int i=lowchibin; i<=highchibin; ++i) 
    {
        if (fDataHisto.GetBinContent(i) < 1.0) continue;
        const float error2 = TMath::Power(fDataHisto.GetBinError(i), 2) + TMath::Power(fGlauberFitHisto.GetBinError(i), 2);
//	std::cout<<"i="<<i<<"  DataError="<<fDataHisto.GetBinError(i)<<std::endl;
//	std::cout<<"i="<<i<<"  GlauberError="<<fGlauberFitHisto.GetBinError(i)<<std::endl;
        const float diff = (fGlauberFitHisto.GetBinContent(i) - fDataHisto.GetBinContent(i));
//	std::cout<<"i="<<i<<"  DataContent="<<fDataHisto.GetBinContent(i)<<std::endl;
//	std::cout<<"i="<<i<<"  GlauberContent="<<fGlauberFitHisto.GetBinContent(i)<<std::endl;
	const float error_diff = (fGlauberFitHisto.GetBinError(i) - fDataHisto.GetBinError(i));

        chi2_error += TMath::Power(diff*error_diff/error2, 2);
    }
    
    chi2_error = 2*TMath::Power(chi2_error,0.5) / (highchibin - lowchibin + 1);
    return chi2_error;
}

/**
 * Populates histogram nbd_<mean>_<k> with values of NBD
 * @param mu
 * @param k
 */
void Glauber::Fitter::SetNBDhist(float mu, float k)
{
    // Interface for TH1F.
    const int nBins = (mu+1.)*3 < 10 ? 10 : (mu+1.)*3;
    
    fNbdHisto = TH1F ("fNbdHisto", "", nBins, 0, nBins);
    fNbdHisto.SetName("nbd");
    
    for (int i=0; i<nBins; ++i) 
    {
        const float val = NBD(i, mu, k);
        if (val>1e-10) fNbdHisto.SetBinContent(i+1, val);
//         std::cout << "val " << val << std::endl;    
    }
}

/**
 * Negative Binomial Distribution (by definition)
 * @param n argument
 * @param mu mean
 * @param k argument
 * @return NBD for a given parameters
 */
float Glauber::Fitter::NBD(float n, float mu, float k) const
{
    // Compute NBD.
    float F;
    float f;

    if (n+k > 100.0) 
    {
        // log method for handling large numbers
        F  = TMath::LnGamma(n + k)- TMath::LnGamma(n + 1.)- TMath::LnGamma(k);
        f  = n * TMath::Log(mu/k) - (n + k) * TMath::Log(1.0 + mu/k);
        F += f;
        F = TMath::Exp(F);
    } 
    else 
    {
        F  = TMath::Gamma(n + k) / ( TMath::Gamma(n + 1.) * TMath::Gamma(k) );
        f  = n * TMath::Log(mu/k) - (n + k) * TMath::Log(1.0 + mu/k);
        f  = TMath::Exp(f);
        F *= f;
    }

    return F;
}
/**
 * Creates histo with a given model parameter distribution
 * @param range observable range
 * @param name name of the MC-Glauber model parameter 
 * @param par array with fit parameters
 * @return pointer to the histogram 
 */
std::unique_ptr<TH1F> Glauber::Fitter::GetModelHisto (const float range[2], TString name, const float par[3])
{    
    const float f =  par[0];
    const float mu = par[1];
    const float k = par[2];
    
    float modelpar{-999.};
    fSimTree->SetBranchAddress(name, &modelpar);
        
    SetNBDhist(mu, k);
  
//     TRandom random;  
//     random.SetSeed(mu*k);

    std::unique_ptr<TH1F> hModel(new TH1F ("hModel", "name", 100, fSimTree->GetMinimum(name),  fSimTree->GetMaximum(name)) );

    for (int i=0; i<fnEvents; i++)
    {
        fSimTree->GetEntry(i);
        const int Na = int(Nancestors(f));
        float nHits{0.};
        for (int j=0; j<Na; ++j) nHits += (int)fNbdHisto.GetRandom();
        
        if ( nHits > range[0] && nHits < range[1] ){
            hModel->Fill(modelpar);
        }
            
    }
    
    return std::move(hModel);
    
}
