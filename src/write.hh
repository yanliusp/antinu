#ifndef WRITE_H
#define WRITE_H

#include <string>

#include <TChain.h>
#include <TNtuple.h>
#include <TH1.h>
#include <TFile.h>

using namespace std;

void write(const string &branchname, const vector<double> & coincidence_cuts);

#endif

