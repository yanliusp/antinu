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
  //fit function for distance distribution
  //TF1 * cumuPos = new TF1("cumuPos", "(x*x*x/3. - 1.05147*3./4./5500.*x*x*x*x/4. + 1.16275/16./5500./5500./5500.*x*x*x*x*x*x/6.)/5500./5500./5500./0.286866", 0, 10452);

  //histograms
  TH1D *hPosdiff = new TH1D("hPosdiff", "distance distribution of coincidence candidates", global_cuts[0]/50., 0, 2*global_cuts[0]);
  TH1D *hTickdiff = new TH1D("hTickdiff", "distribution of tick differences", 100, 0, 1000000000); // range=20s

  candidate->Draw("posdiff_4>>hPosdiff");
  candidate->Draw("tickdiff50_4>>hTickdiff");

  //TF1 *funcPosdiff = new TF1("funcPosdiff", "[1]*(([2]+3)*x*x/TMath::Power([0],3) - ([3]+9./4.)*x*x*x/TMath::Power([0],4) + ([4]+3./16.)*TMath::Power(x,5)/TMath::Power([0],6))", 0, 2*global_cuts[0]); // random points distribution
  TF1 *funcPosdiff = new TF1("funcPosdiff", "[1]*(([2]+3)/TMath::Power([0],3)*x*x - ([3]+9./4.)/TMath::Power([0],4)*x*x*x + ([4]+3./16.)/TMath::Power([0],6)*TMath::Power(x,5) + [5]/TMath::Power([0],5)*TMath::Power(x,4))", 0, 2*global_cuts[0]);
  funcPosdiff->SetParameters(global_cuts[0], hPosdiff->Integral(), 0, 0, 0, 1);
  funcPosdiff->FixParameter(0, global_cuts[0]);
  funcPosdiff->FixParameter(1, hPosdiff->Integral()*100);
  hPosdiff->Fit("funcPosdiff", "M");
  // write the cumulative function
  TF1 *funccumuPosdiff = new TF1("funccumuPosdiff", "[1]*(([2]+3)/TMath::Power([0],3)/3.*x*x*x + ([3]+9./4.)/TMath::Power([0],4)/4.*TMath::Power(x,4) + ([4]+3./16.)/TMath::Power([0],6)/6.*TMath::Power(x,6) + [5]/TMath::Power([0],5)/5.*TMath::Power(x,5))", 0, 2*global_cuts[0]);
  //funccumuPosdiff->SetParameters(funcPosdiff->GetParameter(0),funcPosdiff->GetParameter(1), funcPosdiff->GetParameter(2), funcPosdiff->GetParameter(3), funcPosdiff->GetParameter(4), funcPosdiff->GetParameter(5) );
  funccumuPosdiff->SetParameters(funcPosdiff->GetParameters());
  cout << funccumuPosdiff->GetParameter(0);
  cout << funccumuPosdiff->GetParameter(2);
  cout << funccumuPosdiff->GetParameter(3);
  cout << funccumuPosdiff->GetParameter(4);
  cout << funccumuPosdiff->GetParameter(5);
  funccumuPosdiff->SetParameter(1, 1);
  cout << funccumuPosdiff->GetParameter(1);

  hTickdiff->Fit("expo");
  double lambda = hTickdiff->GetFunction("expo")->GetParameter(1);

  //fit function for time is "expo"

  double posdiff;
  ULong64_t tickdiff;

  candidate->SetBranchAddress("posdiff_4", &posdiff);
  candidate->SetBranchAddress("tickdiff50_4", &tickdiff);

  //fit for distance and time
  for(int iEv=0; iEv<candidate->GetEntries(); iEv++) {
    candidate->GetEvent(iEv);
  }

  outFile->cd();
  hTickdiff->Write();
  hPosdiff->Write();
  funccumuPosdiff->Write();
  funcPosdiff->Write();
}
/*
void test() {

  TFile posfile("test.root", "READ");
  TH1D * hist = (TH1D*) posfile.Get("hcumu_posdiff_4");
  hist->SetDirectory(0);

  TFile *f1 = new TFile("../candidate_old.root", "READ");
  TTree *candidate = (TTree *)f1->Get("candidate");

  TFile *fsave = new TFile("product_loose.root", "RECREATE");
  TTree product("product", "product");

  TH1D * hpro = new TH1D("hpro", "Ptime*Pdist", 100, 0., 1.);

  TF1 * cumuPos = new TF1("cumuPos", "(x*x*x/3. - 1.05147*3./4./5500.*x*x*x*x/4. + 1.16275/16./5500./5500./5500.*x*x*x*x*x*x/6.)/5500./5500./5500./0.286866", 0, 10452);

  double posdiff;
  ULong64_t tickdiff;
  double Pdist, Ptime, Pproduct;
  //double lambda = 1.282e-9; //with nhit max = 20 for delayed
  double lambda = 3.341e-09; //without nhit max = 20 for delayed

  product.Branch("Pdist", &Pdist, "Pdist/D");
  product.Branch("Ptime", &Ptime, "Ptime/D");
  product.Branch("Pproduct", &Pproduct, "Pproduct/D");
  product.Branch("posdiff", &posdiff, "posdiff/D");
  product.Branch("tickdiff", &tickdiff, "tickdiff/l");

  candidate->SetBranchAddress("posdiff_4", &posdiff);
  candidate->SetBranchAddress("tickdiff50_4", &tickdiff);

  for(int iEv=0; iEv<candidate->GetEntries(); iEv++) {
    candidate->GetEvent(iEv);
    //Pdist = hist->GetBinContent(hist->GetXaxis()->FindBin(posdiff)+1);
    //if (Pdist<0.54 && Pdist>0.48)
    //cout << Pdist << "   "  << posdiff  << "   " << hist->GetXaxis()->FindBin(posdiff) << "   " << hist->GetBinCenter(hist->GetXaxis()->FindBin(posdiff)) << endl;
    //write analytical cumulative function for Pdist
    Pdist = cumuPos->Eval(posdiff);
    cout << Pdist << "   " << hist->GetBinContent(hist->GetXaxis()->FindBin(posdiff)+1) << "   "<< posdiff << endl;
    Ptime = 1 - TMath::Exp(-lambda*(double)tickdiff);
    Pproduct = Pdist * Ptime;
    hpro->Fill(Pdist);
    product.Fill();
  }
  
  hpro->Write("");
  //hpro->Draw();
  cumuPos->Draw();
  product.Write();
}
*/
