#include <iostream>
#include <stdexcept>
#include <string>

#include <TChain.h>
#include <TH1.h>
#include <TFile.h>

using namespace std;

void rand_assign(string scanfile, string treename, const int step, const vector<double> &prompt_cuts) {

  cout << "HERE" << endl;
  TFile file(scanfile.c_str(), "UPDATE"); 
  cout << "HERE" << endl;
  TTree *scan = (TTree *)file.Get(treename.c_str());
  cout << "HERE" << endl;

  float prompt;
  //int nhits;
  //double energy;
  //scan->SetBranchAddress("nhits", &nhits);
  //scan->SetBranchAddress("energy", &energy);

  TBranch *promptBr = scan->Branch("prompt", &prompt, "prompt/F");

  cout << scan->GetEntries() << endl;
  for(int iEv=0; iEv<scan->GetEntries(); iEv++) {
      //scan->GetEvent(iEv);

      //applying cuts
      prompt = 1.;
      //if (energy>prompt_cuts[0]) prompt = true;
      promptBr->Fill();
    }

  cout << "HERE" << endl;
  scan->Write("", TObject::kOverwrite);
  //file.Close();

  return;
}
