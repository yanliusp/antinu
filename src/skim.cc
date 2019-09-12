#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>
#include <string>

#include <TChain.h>
#include <TFile.h>

using namespace std;

void skim(const string &ntuplepath, int filesize) {

  //read ntuple files (data)
  TChain *chain = new TChain("output"); 
  chain->Add(ntuplepath.c_str());

  bool fitValid;
  int triggerWord;
  double posx, posy, posz, posr, dirx, diry, dirz, udotr;
  ULong64_t dcFlagged, dcApplied, dcClean = 0xFB0000017FFE;

  chain->SetBranchAddress("fitValid", &fitValid);
  chain->SetBranchAddress("dcFlagged", &dcFlagged);
  chain->SetBranchAddress("dcApplied", &dcApplied); 
  chain->SetBranchAddress("triggerWord", &triggerWord);

  chain->SetBranchAddress("posx", &posx);
  chain->SetBranchAddress("posy", &posy);
  chain->SetBranchAddress("posz", &posz);
  chain->SetBranchAddress("posr", &posr);
  chain->SetBranchAddress("dirx", &dirx);
  chain->SetBranchAddress("diry", &diry);
  chain->SetBranchAddress("dirz", &dirz);

  //output to skim files
  string filename = "./skim_files/skim_s0.root";
  TFile *writeFile = new TFile(filename.c_str(),"RECREATE");
  chain->LoadTree(0);
  TTree *skim = chain->GetTree()->CloneTree(0);
  skim->Branch("udotr",&udotr,"udotr/D");

  long int counter = 1;
  cout << "Total number of events to be skimmed: " << chain->GetEntries() << endl;
  for (long int iEv=0; iEv<chain->GetEntries(); iEv++) {
      if (iEv%10000000 == 0) cerr << "Processed " << iEv << " events, which is " << double(iEv)/double(chain->GetEntries())*100 << "%" << flush;
      chain->GetEvent(iEv);

      //triggerWord, data-cleaning, fitValid
      if (!(((triggerWord & 0x401400) == 0x0) && (triggerWord != 0x40))) continue;
      if (((dcApplied & dcClean) & dcFlagged) != (dcApplied & dcClean)) continue;
      if (!fitValid) continue;

      //calculate u.r
      vector<double> posv {posx,posy,posz};
      vector<double> dirv {dirx,diry,dirz};
      udotr = inner_product(posv.begin(), posv.end(), dirv.begin(), 0)/posr;

      //splitting skim files
      if (counter%filesize == 0) {
        writeFile->cd();
        skim->Write("skimdata");
        writeFile->Close();
        cout << "write to " << filename << endl;
       
        filename = "./skim_files/skim_s" + to_string(counter/filesize) + ".root";
        writeFile = new TFile(filename.c_str(), "RECREATE");
        chain->LoadTree(0);
        skim = chain->GetTree()->CloneTree(0);
        skim->Branch("udotr",&udotr,"udotr/D");
      }
      skim->Fill();
      counter++;

    }

  writeFile->cd();
  skim->Write("skimdata");
  writeFile->Close();
  cout << "write to " << filename << endl;

  return;
}
