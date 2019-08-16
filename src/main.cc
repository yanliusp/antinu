#include "skim.hh"
#include "scan.hh"
#include "prompt.hh"

#include <sys/stat.h>

using namespace std;

inline bool exists (const string& name) {
  struct stat buffer;
  return (stat (name.c_str(), &buffer) == 0);
}

int main () {

  const string SKIMDIR = "./skim_files/skim_s0.root";
  const string SCANDIR = "./scan_files/scan.root";
  const string ntuplepath = "/data/snoplus/processed_data/timebin_4/*ntuple.root";
  //number of entries per skim file
  int filesize = 1000000;
  const vector<double> global_cuts = {5300., 0.0}; //FV, u.r
  const vector<double> prompt_cuts={2.5,0.9}; //energy, nhits
  const vector<double> delayed_cuts={0.,0.9}; //energy, nhits

  //make skim files from ntuples(data)
  if (!exists(SKIMDIR)) skim(ntuplepath, filesize);

  //apply global cuts
  if (!exists(SCANDIR)) scan(global_cuts);

  //randomization && applying cuts
  //Method: "Moving to the next"
  prompt_select("./scan_files/scan.root","scandata", prompt_cuts);
  delayed_select("./scan_files/scan.root","scandata", delayed_cuts);
  return 0;
}
