#include <iostream>
#include <stdexcept>
#include <string>

#include <TChain.h>
#include <TH1.h>
#include <TFile.h>

using namespace std;

void rand_assign(string skimfile, string treename, const int step, const vector<double> &cuts) {

  TChain *chain = new TChain(treename.c_str()); 
  chain->Add(skimfile.c_str());

  int nhits;
  double energy, udotr;
  ULong64_t tickdiff50, cur;
  chain->SetBranchAddress("nhits", &nhits);
  chain->SetBranchAddress("tickdiff50", &tickdiff50);
  chain->SetBranchAddress("energy", &energy);
  chain->SetBranchAddress("udotr", &udotr);

  for(int iEv=0; iEv<chain->GetEntries()-step; iEv++) {
      chain->GetEvent(iEv);
      cur = tickdiff50;
      chain->GetEvent(iEv+step);
      cout << cur << "  " << tickdiff50 << endl;

      //applying cuts
      if (energy>cuts[0] && udotr>cuts[1]) cout << "yes" << endl;
    }

  return;
}
