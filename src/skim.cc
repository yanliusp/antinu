#include <iostream>
#include <stdexcept>

#include <TChain.h>
#include <TNtuple.h>
#include <TH1.h>
#include <TFile.h>

using namespace std;

TTree * skim() {

  TChain *chain = new TChain("output"); 
  chain->Add("./ntuple_files/Analysis*.ntuple.root");

  bool fitValid;
  int nhits, triggerWord;
  ULong64_t clockCount50, curtick, tickdiff;
  ULong64_t dcFlagged, dcApplied, dcClean = 0xFB0000017FFE;
  chain->SetBranchAddress("nhits", &nhits);
  chain->SetBranchAddress("clockCount50", &clockCount50);
  chain->SetBranchAddress("fitValid", &fitValid);
  chain->SetBranchAddress("dcFlagged", &dcFlagged);
  chain->SetBranchAddress("dcApplied", &dcApplied);
  chain->SetBranchAddress("triggerWord", &triggerWord);

  //declare ROOT objects
  TFile *writeFile = new TFile("skim.root","RECREATE");
  chain->LoadTree(0);
  TTree *skim = chain->GetTree()->CloneTree(0);
  skim->Branch("tickdiff",&tickdiff,"tickdiff/l");

  //first event
  chain->GetEvent(0);
  curtick = clockCount50;

  for(int iEv=1; iEv<chain->GetEntries(); iEv++) {
      chain->GetEvent(iEv);
      //triggerWord, data-cleaning, fitValid
      if (!(((triggerWord & 0x401400) == 0x0) && (triggerWord != 0x40))) continue;
      if (((dcApplied & dcClean) & dcFlagged) != (dcApplied & dcClean)) continue;
      if (!fitValid) continue;

      tickdiff = clockCount50-curtick;
      curtick = clockCount50;

      skim->Fill();

      //DO NOT REMOVE
      //monitor clock jump
      if (tickdiff>9999999999999 or tickdiff<0) throw std::invalid_argument
                                               ("abnormal tickdiff value");
    }

  writeFile->cd();
  skim->Write("skimdata");
  return skim;
}
