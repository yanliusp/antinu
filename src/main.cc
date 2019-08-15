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
  vector<double> global_cuts = {5300., 0.0};
  const vector<double> prompt_cuts={2.5};

  //make skim files from ntuples(data)
  const string SKIMDIR = "./skim_files/skim_s0.root";
  if (!exists(SKIMDIR)) skim(filesize);

  //apply global cuts
  scan(global_cuts);

  //randomization && applying cuts
  //Method: "Moving to the next"
  rand_assign("./scan_files/scan.root","scandata", 1, prompt_cuts);
  return 0;
}
