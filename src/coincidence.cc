#include <iostream>
#include <stdexcept>
#include <string>

#include <TChain.h>
#include <TH1.h>
#include <TFile.h>

using namespace std;

void coincidence(string scanfile, string treename, const vector<double> &coincidence_cuts) {

  TFile file(scanfile.c_str(), "READ"); 
  TTree *scan = (TTree *)file.Get(treename.c_str());

  TTree candidate("candidate", "antinu candidates");

  bool prompt, delayed, tag;
  int runID, eventID, promptrunID, prompteventID;
  double posx, posy, posz, posdiff_4;
  vector<double> promptposv, posv;
  ULong64_t clockCount50, promptick50, tickdiff50_4;

  scan->SetBranchAddress("prompt", &prompt);
  scan->SetBranchAddress("delayed", &delayed);

  scan->SetBranchAddress("runID", &runID);
  scan->SetBranchAddress("eventID", &eventID);

  scan->SetBranchAddress("clockCount50", &clockCount50);
  
  scan->SetBranchAddress("posx", &posx);
  scan->SetBranchAddress("posy", &posy);
  scan->SetBranchAddress("posz", &posz);

  candidate.Branch("promptrunID", &promptrunID, "promptrunID/I");
  candidate.Branch("prompteventID", &prompteventID, "prompteventID/I");
  candidate.Branch("delayedrunID", &runID, "delayedrunID/I");
  candidate.Branch("delayedeventID", &eventID, "delayedeventID/I");
  candidate.Branch("posdiff_4", &posdiff_4, "posdiff_4/D");
  candidate.Branch("tickdiff50_4", &tickdiff50_4, "tickdiff50_4/l");

  for(int iEv=0; iEv<scan->GetEntries(); iEv++) {
    scan->GetEvent(iEv);

    //initialize
    tickdiff50_4 = 0; posdiff_4 = 0; //DO I NEED THIS?

    //applying cuts
    if(prompt) {
      //extract prompt time & position info
      promptick50 = clockCount50;
      promptposv = {posx,posy,posz};
      promptrunID = runID;
      prompteventID = eventID;
      cout << "promptID to be acted on: " << promptrunID <<" " <<prompteventID << "  " <<iEv << endl;

      tag = false;
      for(int next=1;!tag;next++) {
        scan->GetEvent(iEv+next);
        cout << "searching for delayed event: " << iEv+next  << " " << delayed<< endl;
        if(iEv+next >=scan->GetEntries()) tag=true; //end loop
        if(delayed) {
          //cout << next << endl;
          tag=true;
          //calculate time difference
          tickdiff50_4 = clockCount50 - promptick50;
          //calculate position difference
          posdiff_4 = 0;
          posv = {posx,posy,posz};
          for (int i=0;i<3;i++) posdiff_4 = posdiff_4 + pow (posv[i]-promptposv[i], 2.0);
          posdiff_4 = sqrt(posdiff_4);
          if(tickdiff50_4*20./1000.<coincidence_cuts[0]) cout << "yes" << endl;
          if(posdiff_4<coincidence_cuts[1]) {
            //cout << "pos: " << posdiff_4 << "   time: " << tickdiff50_4*20./1000. << endl;
            cout << "promptID: " << promptrunID <<" " <<prompteventID <<
                  "  delayedID: " << runID <<" " << eventID << endl; 
          }
          candidate.Fill();
        }
      }
    }
    //tickdiffBr->Fill(); posdiffBr->Fill();
  }
  
  TFile *writeFile = new TFile("./scan_files/candidate.root","RECREATE");//CHANGE PATH LATER
  writeFile->cd();
  candidate.Write("candidate");
  writeFile->Close();
  cout << "write to " << "./scan_files/candidate.root" << endl;

  file.Close();

  return;
}
