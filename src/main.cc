#include "skim.hh"
#include "scan.hh"
#include "rand.hh"

#include <sys/stat.h>

using namespace std;

inline bool exists (const string& name) {
  struct stat buffer;
  return (stat (name.c_str(), &buffer) == 0);
}

int main () {

  //number of entries per skim file
  int filesize = 1000000;

  //make skim files from ntuples(data)
  const string SKIMDIR = "./skim_files/skim_*.root";
  if (!exists(SKIMDIR)) skim(filesize);

  //randomization && applying cuts
  //Method: "Moving to the next"
  //const vector<double> cuts={2.5,0.0};
  //rand_assign("./skim_files/skim.root","skimdata", 1, cuts);
  return 0;
}
