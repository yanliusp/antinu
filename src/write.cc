#include "randomize.hh"

#include <iostream>
#include <stdexcept>
#include <string>

#include <TChain.h>
#include <TH1.h>
#include <TFile.h>

using namespace std;

void write(const string &branchname, const vector<double> & coincidence_cuts) {

  TFile *writeFile = new TFile("./count.root","UPDATE");//CHANGE PATH LATER

  //check if tree exists
  if (writeFile->GetListOfKeys()->Contains("nodata")) {
    cout << "Tree \"nodata\" already exists, reading Tree.." << endl;

    TTree *nodata = (TTree *)writeFile->Get("nodata");

    //check if branch exists
    TBranch *checkBr = (TBranch*) nodata->GetListOfBranches()->FindObject(branchname.c_str());
    if (!checkBr) {

      int step, event;

      TBranch *thisBr = nodata->Branch(branchname.c_str(), &event, (branchname+"/I").c_str());

      for(step=1; step<1000; step++) {
        event = randomize("./candidate_files/candidate.root", "candidate", step, coincidence_cuts);
        cout << "The " << step << " dataset has " << event << " in it" << endl;
        thisBr->Fill();
      }
  
      writeFile->cd();
      nodata->Write("",TObject::kOverwrite);
      writeFile->Close();
      cout << "write to " << "./count.root" << endl;
    } else cout << "Branch \"" << branchname << "\" already exists in ./count.root!" << endl;
  }

  else {
    cout << "Tree \"nodata\" does not exists, creating Tree.." << endl;

    TTree *nodata = new TTree("nodata", "test");

    int step, event;

    nodata->Branch(branchname.c_str(), &event, (branchname+"/I").c_str());
    nodata->Branch("step", &step, "step/I");
  
    for(step=1; step<1000; step++) {
      event = randomize("./candidate_files/candidate.root", "candidate", step, coincidence_cuts);
      cout << "The " << step << " dataset has " << event << " in it" << endl;
      nodata->Fill();
    }
    
    writeFile->cd();
    nodata->Write("",TObject::kOverwrite);
    writeFile->Close();
    cout << "write to " << "./count.root" << endl;
  }
  return;
}
