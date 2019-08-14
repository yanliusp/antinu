#include <iostream>
#include <stdexcept>
#include <string>

#include <TChain.h>
#include <TH1.h>
#include <TFile.h>

using namespace std;

void rand(string skimfile, string treename, const int option) {

  TChain *chain = new TChain(treename.c_str()); 
  chain->Add(skimfile.c_str());

  int nhits;
  ULong64_t clockCount50, curtick50, tickdiff50;
  chain->SetBranchAddress("nhits", &nhits);
  chain->SetBranchAddress("clockCount50", &clockCount50);
  chain->SetBranchAddress("tickdiff50", &tickdiff50);


  if (option == 1) {
    cout << "using 'move to the next' method to randomize" << endl;}
  //first event
  chain->GetEvent(0);
  curtick50 = clockCount50;

  for(int iEv=1; iEv<chain->GetEntries(); iEv++) {
      chain->GetEvent(iEv);

      cout << clockCount50-curtick50 << "     " << tickdiff50 << endl;
      curtick50 = clockCount50;

      //DO NOT REMOVE
      //monitor clock jump
      if (tickdiff50>9999999999999 or tickdiff50<0) throw std::invalid_argument
                                               ("abnormal tickdiff50 value");
    }

  return;
}
