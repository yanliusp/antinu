#include "skim.hh"
#include "scan.hh"
#include "individual.hh"
#include "coincidence.hh"
#include "randomize.hh"

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
  const vector<double> delayed_cuts={1.5,9.9}; //energy, nhits
  const vector<double> coincidence_cuts={2000.,2500.}; //time, position

  //make skim files from ntuples(data)
  if (!exists(SKIMDIR)) skim(ntuplepath, filesize);

  //apply global cuts
  if (!exists(SCANDIR)) scan(global_cuts);

  //apply individual cuts
  event_select("./scan_files/scan.root","scandata", "prompt", prompt_cuts);
  event_select("./scan_files/scan.root","scandata", "delayed", delayed_cuts);

  //apply coincidence cuts
  coincidence("./scan_files/scan.root","scandata");

  //randomization && applying cuts
  //Method: "Moving to the next"
  randomize("./candidate_files/candidate.root", "candidate", 10, coincidence_cuts);

  return 0;
}
