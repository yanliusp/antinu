#include "skim.hh"
#include "scan.hh"
#include "individual.hh"
#include "coincidence.hh"
#include "randomize.hh"
#include "write.hh"

#include <sys/stat.h>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <iterator>
#include <iostream>

using namespace std;

inline bool exists (const string& name) {
  struct stat buffer;
  return (stat (name.c_str(), &buffer) == 0);
}

int main () {

  const string SKIMDIR = "./skim_files/skim_s0.root";
  const string SCANDIR = "./scan_files/scan.root";
  //const string ntuplepath = "/data/snoplus/processed_data/timebin_5-2/*ntuple.root";
  const string ntuplepath = "./ntuple_files/*ntuple.root";
  // number of entries per skim file
  int filesize = 1000000;

  // cuts
  const vector<double> global_cuts = {5300., 0.0, 0.55, -0.12, 0.95}; //FV, u.r, itr, beta14_min, beta14_max
  const vector<double> prompt_cuts={2.5,0.9,1000.}; //energy, nhits_min, nhits_max
  const vector<double> delayed_cuts={1.5,9.9,20.1}; //energy, nhits_min, nhits_max
  const vector<double> coincidence_cuts={15/20.*1000,2000.}; //time [us], position

  //convert cuts to a string
  ostringstream ocuts;
  copy(global_cuts.begin(), global_cuts.end(), std::ostream_iterator<double>(ocuts, "_"));
  copy(prompt_cuts.begin(), prompt_cuts.end(), std::ostream_iterator<double>(ocuts, "_"));
  copy(delayed_cuts.begin(), delayed_cuts.end(), std::ostream_iterator<double>(ocuts, "_"));
  copy(coincidence_cuts.begin(), coincidence_cuts.end(), std::ostream_iterator<double>(ocuts, "_"));
  const string cuts = ocuts.str();

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
  //randomize("./candidate_files/candidate.root", "candidate", 0, coincidence_cuts);

  write(cuts, coincidence_cuts);

  return 0;
}
