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

TTree * scan() {

  TChain *chain = new TChain("output"); 
  chain->Add("./ntuple_files/Analysis*.ntuple.root");

  bool fitValid;
  int triggerWord;
  double posx, posy, posz, posr, dirx, diry, dirz, udotr, posdiff;
  vector<double> curposv; 
  ULong64_t clockCount50, curtick50, tickdiff50;
  ULong64_t dcFlagged, dcApplied, dcClean = 0xFB0000017FFE;
  chain->SetBranchAddress("clockCount50", &clockCount50);

  chain->SetBranchAddress("fitValid", &fitValid);
  chain->SetBranchAddress("dcFlagged", &dcFlagged);
  chain->SetBranchAddress("dcApplied", &dcApplied);
  chain->SetBranchAddress("triggerWord", &triggerWord);

  chain->SetBranchAddress("posx", &posx);
  chain->SetBranchAddress("posy", &posy);
  chain->SetBranchAddress("posz", &posz);
  chain->SetBranchAddress("posr", &posr);
  chain->SetBranchAddress("dirx", &dirx);
  chain->SetBranchAddress("diry", &diry);
  chain->SetBranchAddress("dirz", &dirz);

  //declare ROOT objects
  TFile *writeFile = new TFile("./skim_files/skim.root","RECREATE");
  chain->LoadTree(0);
  TTree *skim = chain->GetTree()->CloneTree(0);
  skim->Branch("tickdiff50",&tickdiff50,"tickdiff50/l");
  skim->Branch("posdiff",&posdiff,"posdiff/D");
  skim->Branch("udotr",&udotr,"udotr/D");

  //first event
  chain->GetEvent(0);
  curtick50 = clockCount50;
  curposv = {posx,posy,posz};

  for(int iEv=1; iEv<chain->GetEntries(); iEv++) {
      chain->GetEvent(iEv);
      //triggerWord, data-cleaning, fitValid
      if (!(((triggerWord & 0x401400) == 0x0) && (triggerWord != 0x40))) continue;
      if (((dcApplied & dcClean) & dcFlagged) != (dcApplied & dcClean)) continue;
      if (!fitValid) continue;

      //calculate tickdiff50
      tickdiff50 = clockCount50-curtick50;
      curtick50 = clockCount50;

      //calculate u.r
      vector<double> posv {posx,posy,posz};
      vector<double> dirv {dirx,diry,dirz};
      udotr = inner_product(posv.begin(), posv.end(), dirv.begin(), 0)/posr;

      posdiff = 0;
      for (int i=0;i<3;i++) posdiff = posdiff + pow (posv[i]-curposv[i], 2.0);
      posdiff = sqrt(posdiff);
      curposv = posv;

      skim->Fill();

      //DO NOT REMOVE
      //monitor clock jump
      if (tickdiff50>9999999999999 or tickdiff50<0) throw std::invalid_argument
                                               ("abnormal tickdiff50 value");
    }

  writeFile->cd();
  skim->Write("skimdata");
  return skim;
}
