///////////////////////////
//    scan.cc
//    Yan Liu
//    Description:
//      apply global cuts
///////////////////////////

#include <iostream>
#include <stdexcept>
#include <vector>
#include <numeric>
#include <cmath>

#include <TChain.h>
#include <TNtuple.h>
#include <TH1.h>
#include <TFile.h>

using namespace std;

void scan(const vector<double> &global_cuts) {

  TChain *chain = new TChain("skimdata"); 
  chain->Add("./skim_files/skim_s2.root");

  double posx, posy, posz, posr, udotr, posdiff;
  vector<double> curposv, posv; 
  ULong64_t clockCount50, curtick50, tickdiff50;
  chain->SetBranchAddress("clockCount50", &clockCount50);

  chain->SetBranchAddress("posx", &posx);
  chain->SetBranchAddress("posy", &posy);
  chain->SetBranchAddress("posz", &posz);
  chain->SetBranchAddress("posr", &posr);
  chain->SetBranchAddress("udotr", &udotr);

  //declare ROOT objects
  TFile *writeFile = new TFile("./scan_files/scan.root","RECREATE");
  chain->LoadTree(0);
  TTree *scan = chain->GetTree()->CloneTree(0);
  scan->Branch("tickdiff50",&tickdiff50,"tickdiff50/l");
  scan->Branch("posdiff",&posdiff,"posdiff/D");

  //first event
  chain->GetEvent(0);
  curtick50 = clockCount50;
  curposv = {posx,posy,posz};

  cout << "Total number of events to be scanned: " << chain->GetEntries() << endl;
  for (int iEv=1; iEv<chain->GetEntries(); iEv++) {
      chain->GetEvent(iEv);

      //Apply global cuts
      //FV, u.r
      if ( posr>global_cuts[0]) continue;
      if (udotr<global_cuts[1]) continue;

      //calculate tickdiff50
      tickdiff50 = clockCount50-curtick50;
      curtick50 = clockCount50;

      //calculate posdiff
      posdiff = 0;
      posv = {posx,posy,posz};
      for (int i=0;i<3;i++) posdiff = posdiff + pow (posv[i]-curposv[i], 2.0);
      posdiff = sqrt(posdiff);
      curposv = posv;

      scan->Fill();

      //DO NOT REMOVE
      //monitor clock jump
      //if (tickdiff50>9999999999999) throw std::invalid_argument
      //                                         ("abnormal tickdiff50 value");
    }

  writeFile->cd();
  scan->Write("scandata");
  cout << "write to ./scan_files/scan.root" << endl;
  cout << "Total number of events after scan: " << scan->GetEntries() << endl;
  writeFile->Close();
  return;
}
