#include<iostream>
#include <TChain.h>
#include <TH1.h>
#include <TFile.h>

using namespace std;

int main() {

  TChain chain("output"); 
  chain.Add("./ntuple_files/Analysis*.ntuple.root");

  TH1D *hTime = new TH1D("hTime","Time difference", 2000,0.,2000);

  Int_t nhits;
  ULong64_t clockCount50, curtick, tickdiff;
  chain.SetBranchAddress("nhits", &nhits);
  chain.SetBranchAddress("clockCount50", &clockCount50);

  //first event
  chain.GetEvent(0);
  curtick = clockCount50;

  //for(int iEv=1; iEv<chain.GetEntries(); iEv++)
  for(int iEv=1; iEv<9990000; iEv++)
    {
      chain.GetEvent(iEv);
      tickdiff = clockCount50-curtick;
      curtick = clockCount50;

      //DO NOT REMOVE
      //monitor clock jump
      if (tickdiff>9999999999999 or tickdiff<0) return 2;

      hTime->Fill(tickdiff*20./1000.);
    }

  TFile *writeFile = new TFile("write.root","RECREATE");
  writeFile->cd();
  hTime->Write();
  return 0;




}
