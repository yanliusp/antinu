#ifndef SCAN_H
#define SCAN_H

#include <vector>

#include <TChain.h>
#include <TNtuple.h>
#include <TH1.h>
#include <TFile.h>

using namespace std;

void scan(const vector<double> &global_cuts);

#endif

