#ifndef CUMUPROB_H
#define CUMUPROB_H

#include <string>

#include <TChain.h>
#include <TNtuple.h>
#include <TH1.h>
#include <TFile.h>
#include <TTree.h>

using namespace std;

void cumuProb(string coincidencefile, string treename, const vector<double> &global_cuts);

#endif

