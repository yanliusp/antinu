#include "randomize.hh"

#include <iostream>
#include <stdexcept>
#include <string>

#include <TChain.h>
#include <TH1.h>
#include <TFile.h>

using namespace std;

void write(const string &branchname, const vector<double> & coincidence_cuts) {

  TFile *writeFile = new TFile("./count.root","RECREATE");//CHANGE PATH LATER
  TTree *nodata = new TTree("nodata", "number of events that passes cuts");

  int step, event;

  nodata->Branch(branchname.c_str(), &event, (branchname+"/I").c_str());
  nodata->Branch("step", &step, "step/I");


  for(step=0; step<1000; step++) {
    event = randomize("./candidate_files/candidate.root", "candidate", step, coincidence_cuts);
    nodata->Fill();
  }
  
  writeFile->cd();
  nodata->Write();
  writeFile->Close();
  cout << "write to " << "./count.root" << endl;

  return;
}
