#include <iostream>
#include <stdexcept>
#include <string>

#include <TChain.h>
#include <TH1.h>
#include <TFile.h>

using namespace std;

void prompt_select(string scanfile, string treename, const vector<double> &prompt_cuts) {

  TFile file(scanfile.c_str(), "UPDATE"); 
  TTree *scan = (TTree *)file.Get(treename.c_str());

  //check if prompt branch exists
  TBranch *checkBr = (TBranch*) scan->GetListOfBranches()->FindObject("prompt");
  if (!checkBr) {

    bool prompt;
    int nhits;
    double energy;
    scan->SetBranchAddress("nhits", &nhits);
    scan->SetBranchAddress("energy", &energy);
  
    TBranch *promptBr = scan->Branch("prompt", &prompt, "prompt/O");
  
    for(int iEv=0; iEv<scan->GetEntries(); iEv++) {
      scan->GetEvent(iEv);

      //applying cuts
      prompt = false;
      if (energy>prompt_cuts[0] && nhits>prompt_cuts[0]) prompt = true;
      promptBr->Fill();
    }
  
    scan->Write("scandata", TObject::kOverwrite);

  } else cout << "Branch \"prompt\" already exists in " << scanfile.c_str() <<"!" << endl;
  file.Close();

  return;
}

void delayed_select(string scanfile, string treename, const vector<double> &delayed_cuts) {

  TFile file(scanfile.c_str(), "UPDATE"); 
  TTree *scan = (TTree *)file.Get(treename.c_str());

  //check if prompt branch exists
  TBranch *checkBr = (TBranch*) scan->GetListOfBranches()->FindObject("prompt");
  if (!checkBr) throw std::invalid_argument("No prompts has been identified!")
  else {

    bool prompt,delayed;
    int nhits;
    double energy;
    scan->SetBranchAddress("prompt", &prompt);
    scan->SetBranchAddress("nhits", &nhits);
    scan->SetBranchAddress("energy", &energy);
    
    TBranch *delayedBr = scan->Branch("delayed", &delayed, "delayed/O");

    for(int iEv=0; iEv<scan->GetEntries(); iEv++) {
      scan->GetEvent(iEv);

      //applying cuts
      delayed = false;
      //identify prompt candidate
      if (!prompt) { delayedBr->Fill; continue;}
      for (int next=1;delayed=true;next++) {
        scan->GetEvent(iEv+next);
        if (energy>delayed_cuts[0] && nhits>delayed_cuts[0]) delayed = true;
      }
      delayedBr->Fill();
    }

    //fill delayedBr
    for(int iEv=0; iEv<scan->GetEntries(); iEv++) {
      if(delayed)
      
    }

    scan->Write("scandata", TObject::kOverwrite);
  }
  file.Close();
  return;
}
