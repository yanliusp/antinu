#include "skim.hh"
#include "rand.hh"

int main () {
  //make skim files from ntuples(data)
  //skim();

  //randomization
  //Method: "Moving to the next"
  const vector<double> cuts={2.5,0.0};
  rand_assign("./skim_files/skim.root","skimdata", 1,cuts);
  return 0;
}
