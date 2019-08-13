#include<iostream>
#include <TChain.h>

using namespace std;

int main() {

  TChain chain("output"); 
  chain.Add("./ntuple_files/Analysis*.ntuple.root");

  Int_t nhits;
  chain.SetBranchAddress("nhits", &nhits);
  Int_t total = chain.GetEntries();

  for(int i=0; i<total; i++)
    {
      cout << i << endl;
      chain.GetEvent(i);
      cout << nhits << endl;
    }

  return 0;




}
