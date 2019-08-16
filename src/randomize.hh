#ifndef RANDOMIZE_H
#define RANDOMIZE_H

#include <string>

#include <TChain.h>
#include <TNtuple.h>
#include <TH1.h>
#include <TFile.h>

using namespace std;

void randomize(string coincidencefile, string treename, int step, const vector<double> &coincidence_cuts); 

#endif

