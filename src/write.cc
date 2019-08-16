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
  TTree count("count", "final count");

  int step, event;

  count.Branch(branchname.c_str(), &event, (branchname+"/I").c_str());
  count.Branch("step", &step, "step/I");


  for(step=0; step<1000; step++) {
    event = randomize("./candidate_files/candidate.root", "candidate", step, coincidence_cuts);
    count.Fill();
  }
  
  writeFile->cd();
  count.Write("final");
  writeFile->Close();
  cout << "write to " << "./count.root" << endl;

  return;
}
