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
constexpr int particleCapacity = 300;

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
    p1.setIndex(particleNames::pionePlus);
    p2.setIndex(particleNames::kaonePlus);
  }
}

int main() {
  //todo: uncomment vvv
  //gRandom->SetSeed();
  // todo: must it be an array of pointers?
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
  for (int i{}; i < particleNumber; ++i) {
    // generating momentum
    double phi = gRandom->Uniform(0, 2 * TMath::Pi());
    double theta = gRandom->Uniform(0, TMath::Pi());
    double p = gRandom->Exp(1);
    double pArray[3];
    polarToCartesian(pArray, theta, phi, p);

    // randomly generates particle
    char* particleName = particleGen();

    // popualte array of particles
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
    }
  }

  // define histograms

  TH1F* hTypes = new TH1F("types", "particle types", 7, 0., 6.);

  for (int i{}; i < particleNumber + overfill; ++i) {
    hTypes->Fill(EventParticles[i].getIndex());
  }

  // draw histograms
  TCanvas* c1 = new TCanvas("c1", "first canvas", 800, 800);
  hTypes->Draw();
}