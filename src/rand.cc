#include <iostream>
#include <stdexcept>
#include <string>

#include <TChain.h>
#include <TH1.h>
#include <TFile.h>

using namespace std;

void rand_assign(string skimfile, string treename, const int step) {

  TChain *chain = new TChain(treename.c_str()); 
  chain->Add(skimfile.c_str());

  int nhits;
  ULong64_t tickdiff50, cur;
  chain->SetBranchAddress("nhits", &nhits);
  chain->SetBranchAddress("tickdiff50", &tickdiff50);

  for(int iEv=0; iEv<chain->GetEntries()-step; iEv++) {
      chain->GetEvent(iEv);
      cur = tickdiff50;
      chain->GetEvent(iEv+step);
      cout << cur << "  " << tickdiff50 << endl;

    }

  return;
}
