#include <iostream>

#include <TChain.h>
#include <TFile.h>

using namespace std;

void skim(int filesize) {

  //read ntuple files (data)
  TChain *chain = new TChain("output"); 
  chain->Add("./ntuple_files/Analysis*.ntuple.root");

  bool fitValid;
  int triggerWord;
  ULong64_t dcFlagged, dcApplied, dcClean = 0xFB0000017FFE;

  chain->SetBranchAddress("fitValid", &fitValid);
  chain->SetBranchAddress("dcFlagged", &dcFlagged);
  chain->SetBranchAddress("dcApplied", &dcApplied); chain->SetBranchAddress("triggerWord", &triggerWord);

  //output to skim files
  string filename = "./skim_files/skim_s0.root";
  TFile *writeFile = new TFile(filename.c_str(),"RECREATE");
  chain->LoadTree(0);
  TTree *skim = chain->GetTree()->CloneTree(0);

  int counter = 1;
  for (int iEv=0; iEv<chain->GetEntries(); iEv++) {
      chain->GetEvent(iEv);

      //triggerWord, data-cleaning, fitValid
      if (!(((triggerWord & 0x401400) == 0x0) && (triggerWord != 0x40))) continue;
      if (((dcApplied & dcClean) & dcFlagged) != (dcApplied & dcClean)) continue;
      if (!fitValid) continue;

      //splitting skim files
      if (counter%filesize == 0) {
        writeFile->cd();
        skim->Write("skimdata");
        cout << "write to " << filename << endl;
       
        filename = "./skim_files/skim_s" + to_string(counter/filesize) + ".root";
        writeFile = new TFile(filename.c_str(), "RECREATE");
        skim = chain->GetTree()->CloneTree(0);
      }
      skim->Fill();
      counter++;

    }

  writeFile->cd();
  skim->Write("skimdata");
  cout << "write to " << filename << endl;

  return;
}
