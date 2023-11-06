#include <iostream>

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
#include "particle.hpp"
#include "particletype.hpp"
#include "resonancetype.hpp"
constexpr int particleNumber = 100;
constexpr int particleCapacity = 200;

namespace particleNames {
char pionePlus[] = "pione+";
char pioneMinus[] = "pione-";
char kaonePlus[] = "kaone+";
char kaoneMinus[] = "kaone-";
char protonePlus[] = "protone+";
char protoneMinus[] = "protone-";
char kPlus[] = "K+";
}  // namespace particleNames

void polarToCartesian(double* pArray, double theta, double phi, double p) {
  pArray[0] = p * TMath::Sin(theta) * TMath::Cos(phi);
  pArray[1] = p * TMath::Sin(theta) * TMath::Sin(phi);
  pArray[2] = p * TMath::Cos(theta);
}

char* particleGen() {
  double random = gRandom->Rndm();
  if (random < 0.4) {
    return particleNames::pionePlus;
  } else if (random < 0.8) {
    return particleNames::pioneMinus;
  } else if (random < 0.85) {
    return particleNames::kaonePlus;
  } else if (random < 0.9) {
    return particleNames::kaoneMinus;
  } else if (random < 0.945) {
    return particleNames::protonePlus;
  } else if (random < 0.99) {
    return particleNames::protoneMinus;
  } else {
    return particleNames::kPlus;
  }
}

void decayGen(Particle& p1, Particle& p2) {
  double random = gRandom->Rndm();
  if (random < 0.5) {
    p1.setIndex(particleNames::pionePlus);
    p2.setIndex(particleNames::kaoneMinus);
  } else {
    p1.setIndex(particleNames::pioneMinus);
    p2.setIndex(particleNames::kaonePlus);
  }
}

int main() {
  // todo: uncomment vvv
  gRandom->SetSeed();

  Particle EventParticles[particleCapacity];

  // number of particles over the generated 100 particles.
  int overfill{0};

  Particle::addParticleType(particleNames::pionePlus, 0.13957, 1);
  Particle::addParticleType(particleNames::pioneMinus, 0.13957, -1);
  Particle::addParticleType(particleNames::kaonePlus, 0.49367, 1);
  Particle::addParticleType(particleNames::kaoneMinus, 0.49367, -1);
  Particle::addParticleType(particleNames::protonePlus, 0.93827, 1);
  Particle::addParticleType(particleNames::protoneMinus, 0.93827, -1);
  Particle::addParticleType(particleNames::kPlus, 0.89166, 0, 0.050);

  // initializing histograms
  TH1F* hTypes = new TH1F("types", "particle types", 7, 0., 7.);
  // TODO: check that bin numbers is right
  TH1F* hTheta = new TH1F("theta", "theta", 10, 0., TMath::Pi());
  TH1F* hPhi = new TH1F("phi", "phi", 10, 0., 2 * TMath::Pi());
  // TODO: check that bin range is right
  TH1F* hP = new TH1F("p", "momentum", 10, 0., 10.);
  TH1F* hPTrans = new TH1F("pTrans", "transverse momentum", 10., 0., 10.);
  TH1F* hEnergy = new TH1F("energy", "Energy", 10, 0., 10.);
  TH1F* hInvMass = new TH1F("invMass", "invaraint mass", 10, 0., 10.);
  hInvMass->Sumw2();

  // particle generation loop
  for (int i{}; i < particleNumber; ++i) {
    // generating momentum
    double phi = gRandom->Uniform(0, 2 * TMath::Pi());
    double theta = gRandom->Uniform(0, TMath::Pi());
    double p = gRandom->Exp(1);
    double pArray[3];
    polarToCartesian(pArray, theta, phi, p);

    // randomly generates particle
    char* particleName = particleGen();

    // populate array of particles
    EventParticles[i].setP(pArray[0], pArray[1], pArray[2]);
    EventParticles[i].setIndex(particleName);

    // if particle is K+ it decays in two other particle
    if (particleName == particleNames::kPlus) {
      decayGen(EventParticles[particleNumber + overfill],
               EventParticles[particleNumber + overfill + 1]);
      EventParticles[i].Decay2body(
          EventParticles[particleNumber + overfill],
          EventParticles[particleNumber + overfill + 1]);
      overfill += 2;

      // filling histograms with decayed particles
      hTypes->Fill(EventParticles[particleNumber + overfill].getIndex());
      hTypes->Fill(EventParticles[particleNumber + overfill + 1].getIndex());
      // TODO: which histograms must also consider decayed particles?
    }

    // filling histograms
    hTypes->Fill(EventParticles[i].getIndex());
    hTheta->Fill(theta);
    hPhi->Fill(phi);
    hP->Fill(p);
    hPTrans->Fill(TMath::Sqrt(TMath::Power(EventParticles[i].getPx(), 2) +
                              TMath::Power(EventParticles[i].getPy(), 2)));
    hEnergy->Fill(EventParticles[i].getEnergy());

    // TODO: not sure if we have to iterate through all particles
    for (int j{}; j < i; ++j)
      hInvMass->Fill(EventParticles[i].invMass(EventParticles[j]));
  }

  //------------------- draw histograms -------------------
  //--- Types ---
  TCanvas* cTypes = new TCanvas("cTypes", "types canvas", 800, 800);

  hTypes->SetLineWidth(1);
  hTypes->SetLineColor(kBlack);
  hTypes->SetTitle("number of particles generated");
  hTypes->SetMarkerStyle(kFullCircle);
  hTypes->SetMarkerSize(0.8);
  hTypes->SetMarkerColor(kBlack);
  hTypes->SetFillColor(kBlue);

  // changing axis name
  hTypes->GetXaxis()->SetBinLabel(1, particleNames::pionePlus);
  hTypes->GetXaxis()->SetBinLabel(2, particleNames::pioneMinus);
  hTypes->GetXaxis()->SetBinLabel(3, particleNames::kaonePlus);
  hTypes->GetXaxis()->SetBinLabel(4, particleNames::kaoneMinus);
  hTypes->GetXaxis()->SetBinLabel(5, particleNames::protonePlus);
  hTypes->GetXaxis()->SetBinLabel(6, particleNames::protoneMinus);
  hTypes->GetXaxis()->SetBinLabel(7, particleNames::kPlus);
  hTypes->Draw("H");

  cTypes->Print("../histograms/canvasTypes.pdf");

  //--- Angles ---
  TCanvas* cAngles = new TCanvas("cAngles", "angle canvas", 800, 800);
  cAngles->Divide(1, 2);
  cAngles->cd(1);

  hTheta->SetLineWidth(1);
  hTheta->SetLineColor(kBlack);
  hTheta->SetTitle("theta distribution");
  hTheta->SetMarkerStyle(kFullCircle);
  hTheta->SetMarkerSize(0.8);
  hTheta->SetMarkerColor(kBlack);
  hTheta->SetFillColor(kViolet);
  hTheta->SetMinimum(0);
  hTheta->Draw("H");

  cAngles->cd(2);

  hPhi->SetLineWidth(1);
  hPhi->SetLineColor(kBlack);
  hPhi->SetTitle("phi distribution");
  hPhi->SetMarkerStyle(kFullCircle);
  hPhi->SetMarkerSize(0.8);
  hPhi->SetMarkerColor(kBlack);
  hPhi->SetFillColor(kViolet);
  hPhi->SetMinimum(0);
  hPhi->Draw("H");

  cAngles->Print("../histograms/canvasAngles.pdf");

  //--- Momentum ---
  TCanvas* cP = new TCanvas("cP", "momentum canvas", 800, 800);
  cP->Divide(1, 2);
  cP->cd(1);

  hP->SetLineWidth(1);
  hP->SetLineColor(kBlack);
  hP->SetTitle("momentum distribution");
  hP->SetMarkerStyle(kFullCircle);
  hP->SetMarkerSize(0.8);
  hP->SetMarkerColor(kBlack);
  hP->SetFillColor(kRed);
  hP->SetMinimum(0);
  hP->Draw("H");

  cP->cd(2);

  hPTrans->SetLineWidth(1);
  hPTrans->SetLineColor(kBlack);
  hPTrans->SetTitle("transverse momentum distribution");
  hPTrans->SetMarkerStyle(kFullCircle);
  hPTrans->SetMarkerSize(0.8);
  hPTrans->SetMarkerColor(kBlack);
  hPTrans->SetFillColor(kRed);
  hPTrans->SetMinimum(0);
  hPTrans->Draw("H");

  cP->Print("../histograms/canvasMomentum.pdf");

  //--- Energy ---
  TCanvas* cEnergy = new TCanvas("cEnergy", "energy canvas", 800, 800);

  hEnergy->SetLineWidth(1);
  hEnergy->SetLineColor(kBlack);
  hEnergy->SetTitle("energy distribution");
  hEnergy->SetMarkerStyle(kFullCircle);
  hEnergy->SetMarkerSize(0.8);
  hEnergy->SetMarkerColor(kBlack);
  hEnergy->SetFillColor(kOrange);
  hEnergy->SetMinimum(0);
  hEnergy->Draw("H");

  cEnergy->Print("../histograms/canvasEnergy.pdf");

  //--- Inv Mass ---
  TCanvas* cInvMass = new TCanvas("cInvMass", "invariant mass canvas", 800, 800);

  hInvMass->SetLineWidth(1);
  hInvMass->SetLineColor(kBlack);
  hInvMass->SetTitle("invariant mass distribution");
  hInvMass->SetMarkerStyle(kFullCircle);
  hInvMass->SetMarkerSize(0.8);
  hInvMass->SetMarkerColor(kBlack);
  hInvMass->SetFillColor(kTeal);
  hInvMass->SetMinimum(0);
  hInvMass->Draw("H");

  cInvMass->Print("../histograms/canvasInvMass.pdf");
}