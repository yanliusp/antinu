#include <iostream>
#include <string> 
#include <TH1.h>
#include <TFile.h>
#include <TTree.h>
#include <TF1.h>
#include <TMath.h>

using namespace std;

void cumuProb(string coincidencefile, string treename, const vector<double> &global_cuts) {
  //read in coincidence candidates
  TFile file(coincidencefile.c_str(), "READ");
  TTree *candidate = (TTree *)file.Get(treename.c_str());

  TFile *outFile = new TFile("test.root", "RECREATE");
  TTree *data = new TTree("data", "tree");

  //histograms
  TH1D *hPosdiff = new TH1D("hPosdiff", "distance distribution of coincidence candidates", global_cuts[0]/50., 0, 2*global_cuts[0]);
  TH1D *hTickdiff = new TH1D("hTickdiff", "distribution of tick differences", 100, 0, 1000000000); // range=20s
  TH1D *hProduct = new TH1D("hProduct", "distribution of Log(product)", 120, -12, 0);

  candidate->Draw("posdiff_4>>hPosdiff", "tickdiff50_4<1000000000");
  candidate->Draw("tickdiff50_4>>hTickdiff", "tickdiff50_4<1000000000");

  //fit function for distance distribution
  //TF1 *funcPosdiff = new TF1("funcPosdiff", "[1]*(([2]+3)*x*x/TMath::Power([0],3) - ([3]+9./4.)*x*x*x/TMath::Power([0],4) + ([4]+3./16.)*TMath::Power(x,5)/TMath::Power([0],6))", 0, 2*global_cuts[0]); // random points distribution

  TF1 *funcPosdiff = new TF1("funcPosdiff", "[1]*(([2]+3)/TMath::Power([0],3)*x*x - ([3]+9./4.)/TMath::Power([0],4)*x*x*x + ([4]+3./16.)/TMath::Power([0],6)*TMath::Power(x,5) + [5]/TMath::Power([0],5)*TMath::Power(x,4) + [6]/TMath::Power([0],7)*TMath::Power(x,6) )", 0, 2*global_cuts[0]);
  funcPosdiff->SetParameters(global_cuts[0], hPosdiff->Integral(), 0, 0, 0, 1, 1);
  funcPosdiff->FixParameter(0, global_cuts[0]);
  funcPosdiff->FixParameter(1, hPosdiff->Integral()*100);
  hPosdiff->Fit("funcPosdiff", "M");

  //normalize
  funcPosdiff->SetParameter(1, 1.);

  //fit function for time is "expo"
  hTickdiff->Fit("expo");
  TF1 *funcTickdiff = hTickdiff->GetFunction("expo");
  double lambda = funcTickdiff->GetParameter(1);

  double posdiff;
  ULong64_t tickdiff;
  double ptime, pdist, product;

  candidate->SetBranchAddress("posdiff_4", &posdiff);
  candidate->SetBranchAddress("tickdiff50_4", &tickdiff);

  data->Branch("product", &product, "product/D");
  data->Branch("ptime", &ptime, "ptime/D");
  data->Branch("pdist", &pdist, "pdist/D");
  data->Branch("posdiff", &posdiff, "posdiff/D");
  data->Branch("tickdiff", &tickdiff, "tickdiff/l");

  //fit for distance and time
  for(int iEv=0; iEv<candidate->GetEntries(); iEv++) {
    candidate->GetEvent(iEv);

    if (tickdiff<1000000000) { //timediff < 20s
      ptime = 1 - TMath::Exp(lambda*(double)tickdiff);
      pdist = funcPosdiff->Integral(0., posdiff);
      product = funcPosdiff->Integral(0., posdiff) * (1 - TMath::Exp(lambda*(double)tickdiff));
      hProduct->Fill(TMath::Log10(product));
      data->Fill();
    }
    
  }

  outFile->cd();
  hTickdiff->Write();
  hPosdiff->Write();
  funcPosdiff->Write();
  funcTickdiff->Write("funcTickdiff");
  hProduct->Write();
  data->Write("",TObject::kOverwrite);
  outFile->Close();
}
