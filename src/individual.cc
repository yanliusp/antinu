#include <iostream>
#include <stdexcept>
#include <string>

#include <TChain.h>
#include <TH1.h>
#include <TFile.h>

using namespace std;

void event_select(string scanfile, string treename, string eventype, const vector<double> &individual_cuts) {

  TFile file(scanfile.c_str(), "UPDATE"); 
  TTree *scan = (TTree *)file.Get(treename.c_str());

  //check if branch exists
  TBranch *checkBr = (TBranch*) scan->GetListOfBranches()->FindObject(eventype.c_str());
  if (!checkBr) {

    bool bit;
    int nhits;
    double energy;
    scan->SetBranchAddress("nhits", &nhits);
    scan->SetBranchAddress("energy", &energy);
  
    TBranch *bitBr = scan->Branch(eventype.c_str(), &bit, (eventype+"/O").c_str());
  
    for(int iEv=0; iEv<scan->GetEntries(); iEv++) {
      scan->GetEvent(iEv);

      //applying cuts
      bit = false;
      if (energy>individual_cuts[0] && nhits>individual_cuts[1] && nhits<individual_cuts[2]) bit = true;
      cout << energy << "  " << nhits << "   " << bit << endl;
      bitBr->Fill();
    }
  
    scan->Write("scandata", TObject::kOverwrite);
    cout << "Write Branch \"" << eventype << "\" to " << scanfile.c_str() << endl;

  } else cout << "Branch \"" << eventype << "\" already exists in " << scanfile.c_str() <<"!" << endl;
  file.Close();

  return;
}
