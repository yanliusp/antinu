#include "skim.hh"
#include "rand.hh"

int main () {
  //make skim files from ntuples(data)
  skim();

  //randomization
  //Options: 1. "Moving to the next"
  //rand_assign("./skim_files/skim.root","skimdata", 1);
  return 0;
}
