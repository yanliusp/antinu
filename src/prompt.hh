#ifndef PROMPT_H
#define PROMPT_H

#include <string>

#include <TChain.h>
#include <TNtuple.h>
#include <TH1.h>
#include <TFile.h>

using namespace std;

void prompt_select(string scanfile, string treename, const vector<double> &prompt_cuts);


#endif

