#include "randomize.hh"

#include <iostream>
#include <stdexcept>
#include <string>

#include <TChain.h>
#include <TH1.h>
#include <TFile.h>

using namespace std;

void write(const vector<double> & coincidence_cuts) {

  TFile *writeFile = new TFile("./count.root","RECREATE");//CHANGE PATH LATER
  TTree count("count", "final count");

  int step, event;

  count.Branch("event", &event, "event/I");
  count.Branch("step", &step, "step/I");

  //SOMETHING FUNNY IS HAPPENNING HERE. I HAVE TO coincidence_cuts for the loop
  //cout << "timediff cut: " << coincidence_cuts[0] << endl;
  //cout << "posidiff cut: " << coincidence_cuts[1] << endl;

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
