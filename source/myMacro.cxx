#include "TApplication.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TFile.h"
#include "TFitResult.h"
#include "TH1F.h"
#include "TLegend.h"
#include "TMath.h"
#include "TMatrixD.h"
#include "TROOT.h"
#include "TRandom.h"
#include "TStyle.h"
#include "memory_test.hpp"
#include "particle.hpp"
#include "particletype.hpp"
#include "resonancetype.hpp"
#include "timer.hpp"
int myMacro() {
  // getting histograms from files
  TFile* file = new TFile("histograms/histos.root");
  TH1F* hTypes = (TH1F*)file->Get("particle types");
  TH1F* hTheta = (TH1F*)file->Get("theta");
  TH1F* hPhi = (TH1F*)file->Get("phi");
  TH1F* hP = (TH1F*)file->Get("momentum");
  TH1F* hPTrans = (TH1F*)file->Get("pTrans");
  TH1F* hEnergy = (TH1F*)file->Get("energy");
  TH1F* hInvMass = (TH1F*)file->Get("invMass");
  TH1F* hInvMassDiscordant = (TH1F*)file->Get("invMassDiscordant");
  TH1F* hInvMassConcordant = (TH1F*)file->Get("invMassDiscordant");
  TH1F* hInvMassDiscordantKP = (TH1F*)file->Get("invMassDiscordant");
  TH1F* hInvMassConcordantKP = (TH1F*)file->Get("invMassDiscordant");
  TH1F* hInvMassDecay = (TH1F*)file->Get("invMassDiscordant");

  // checking that the histograms have the correct number of entries
  if (hTypes->GetEntries() == 1e7) {
    std::cout << "true" << '\n';
  }
  if (hTheta->GetEntries() == 1e7) {
    std::cout << "true" << '\n';
  }
  if (hPhi->GetEntries() == 1e7) {
    std::cout << "true" << '\n';
  }
  if (hP->GetEntries() == 1e7) {
    std::cout << "true" << '\n';
  }
  if (hPTrans->GetEntries() == 1e7) {
    std::cout << "true" << '\n';
  }
  if (hEnergy->GetEntries() == 1e7) {
    std::cout << "true" << '\n';
  }
}