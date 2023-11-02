#include "TH1F.h"
void testMacro(){
  TH1F* hTypes = new TH1F("types", "particle types", 7, 0., 6.);
}
