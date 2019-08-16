#ifndef COINCIDENCE_H
#define COINCIDENCE_H

#include <string>

#include <TChain.h>
#include <TNtuple.h>
#include <TH1.h>
#include <TFile.h>

using namespace std;

void coincidence(string scanfile, string treename, const vector<double> &coincidence_cuts); 

#endif

