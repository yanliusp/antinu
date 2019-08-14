#include "skim.hh"
#include "rand.hh"

int main () {
  skim();
  rand("./skim_files/skim.root","skimdata");
  return 0;
}
