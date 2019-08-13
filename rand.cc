#include<iostream>
#include <TChain.h>

using namespace std;

int main() {

  TChain chain("output"); 
  chain.Add("./ntuple_files/Analysis*.ntuple.root");

  Int_t nhits;
  ULong64_t clockCount50, tick=0, tickdiff;
  chain.SetBranchAddress("nhits", &nhits);
  chain.SetBranchAddress("clockCount50", &clockCount50);
  Int_t total = chain.GetEntries();

  for(int i=0; i<total; i++)
    {
      chain.GetEvent(i);
      tickdiff = clockCount50-tick;
      tick = clockCount50;
      cout << tick << " -  " <<clockCount50 <<"   =   "<< tickdiff<< endl;

      //DO NOT REMOVE
      //monitor clock jump
      if (tickdiff>9999999999999) return 2;
    }

  return 0;




}
