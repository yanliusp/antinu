#ifndef PROMPT_H
#define PROMPT_H

#include <string>

#include <TChain.h>
#include <TNtuple.h>
#include <TH1.h>
#include <TFile.h>

using namespace std;

void event_select(string scanfile, string treename, string eventype, const vector<double> &individual_cuts);

#endif

