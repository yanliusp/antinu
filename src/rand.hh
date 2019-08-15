#ifndef RAND_H
#define RAND_H

#include <string>

#include <TChain.h>
#include <TNtuple.h>
#include <TH1.h>
#include <TFile.h>

using namespace std;

void rand_assign(string scanfile, string treename, const int step, const vector<double> &prompt_cuts);


#endif

