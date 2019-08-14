#include "skim.hh"
#include "rand.hh"

int main () {
  //make skim files from ntuples(data)
  //skim();

  //randomization
  rand("./skim_files/skim.root","skimdata", 1);
  return 0;
}
