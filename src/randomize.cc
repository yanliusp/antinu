#include <iostream>
#include <stdexcept>
#include <string>

#include <TChain.h>
#include <TH1.h>
#include <TFile.h>

using namespace std;

int randomize(string coincidencefile, string treename, int step, const vector<double> &coincidence_cuts) {
  TFile file(coincidencefile.c_str(), "READ"); 
  TTree *candidate = (TTree *)file.Get(treename.c_str());

  int counter;
  int delayedrunID, delayedeventID, promptrunID, prompteventID;
  double posdiff_4, PH_posdiff_4;
  ULong64_t tickdiff50_4;//, PH_tickdiff50_4;

  candidate->SetBranchAddress("promptrunID", &promptrunID);
  candidate->SetBranchAddress("prompteventID", &prompteventID);
  candidate->SetBranchAddress("delayedrunID", &delayedrunID);
  candidate->SetBranchAddress("delayedeventID", &delayedeventID);
  candidate->SetBranchAddress("posdiff_4", &posdiff_4);
  candidate->SetBranchAddress("tickdiff50_4", &tickdiff50_4);

  for(int iEv=0; iEv<candidate->GetEntries(); iEv++) {
    candidate->GetEvent(iEv);
    PH_posdiff_4 = posdiff_4;
    //PH_tickdiff50_4 = tickdiff50_4;
    //randomize
    if (iEv+step>=candidate->GetEntries()) candidate->GetEvent(iEv+step-candidate->GetEntries()); else candidate->GetEvent(iEv+step);

    //apply cuts
    if (PH_posdiff_4<coincidence_cuts[1] && tickdiff50_4*20./1000.<coincidence_cuts[0]) 
    counter++;
    //cout << "Randomized: " << PH_posdiff_4 << "   " <<tickdiff50_4*20./1000./1000. << endl;
  }
  
  file.Close();

  return counter;
}
