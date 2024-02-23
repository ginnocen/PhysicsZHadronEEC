#include <TFile.h>
#include <TH1D.h>

class CorrectionFactorGetter {
public:
    CorrectionFactorGetter(const char* filename = "residualCorrection_20230508/totalCorrection.root") {
        f = new TFile(filename);
        hPtCorrTotal = (TH1D*)f->Get("hPtCorrTotal");
        hEtaCorrTotal = (TH1D*)f->Get("hEtaCorrTotal");
        hPhiCorrTotal = (TH1D*)f->Get("hPhiCorrTotal");
    }

    ~CorrectionFactorGetter() {
        f->Close();
        delete f;
    }

    double GetCorrectionFactor(double pt, double eta, double phi) {
        int bin_pt = hPtCorrTotal->GetXaxis()->FindBin(pt);
        int bin_eta = hEtaCorrTotal->GetXaxis()->FindBin(eta);
        int bin_phi = hPhiCorrTotal->GetXaxis()->FindBin(phi);
        double corr = hPtCorrTotal->GetBinContent(bin_pt) *
               hEtaCorrTotal->GetBinContent(bin_eta) *
               hPhiCorrTotal->GetBinContent(bin_phi);
	       
	if (corr>10) {
	   cout <<"Error! 0 efficiency! "<<bin_pt<<" "<<bin_eta<<" "<<bin_phi<<endl;
	   corr = 1;
	}
	return corr;
    }

private:
    TFile* f;
    TH1D* hPtCorrTotal;
    TH1D* hEtaCorrTotal;
    TH1D* hPhiCorrTotal;
};

CorrectionFactorGetter cfg;



