#include <chrono>
#include <iostream>
#include <memory>

#include "memory_test.hpp"

#define ROOTLESS

#ifndef ROOTLESS
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
#else
struct fakeRndm {
  float Rndm() { return 4; }
  float Exp(float x) { return 4; }
  float Uniform(float x, float y) { return 4; }
  void SetSeed(){};
};
fakeRndm* gRandom;

struct TMath {
  static float Sin(float x) { return 4; }
  static float Cos(float x) { return 4; }
  static float Pi() { return 4; }
  static float Power(float x, float y) { return 4; }
  static float Sqrt(float x) { return 4; }
};

struct fakeH {
  void Fill(float x) {}
};

fakeH *hTypes, *hTheta, *hPhi, *hP, *hPTrans, *hEnergy, *hInvMass,
    *hInvMassDiscordant, *hInvMassConcordant, *hInvMassDiscordantKP,
    *hInvMassConcordantKP, *hInvMassDecay;

#endif

#include "particle.hpp"
#include "particletype.hpp"
#include "resonancetype.hpp"
#include "timer.hpp"
constexpr int eventNumber = 1000000;
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
  Timer timer{"total timer"};
  // todo: uncomment vvv
  gRandom->SetSeed();

  Particle EventParticles[particleCapacity];

  // number of particles over the generated 100 particles.
  int overflow{0};

  Particle::addParticleType(particleNames::pionePlus, 0.13957, 1);
  Particle::addParticleType(particleNames::pioneMinus, 0.13957, -1);
  Particle::addParticleType(particleNames::kaonePlus, 0.49367, 1);
  Particle::addParticleType(particleNames::kaoneMinus, 0.49367, -1);
  Particle::addParticleType(particleNames::protonePlus, 0.93827, 1);
  Particle::addParticleType(particleNames::protoneMinus, 0.93827, -1);
  Particle::addParticleType(particleNames::kPlus, 0.89166, 0, 0.050);

#ifndef ROOTLESS
  // initializing histograms
  TH1F* hTypes = new TH1F("types", "particle types", 7, 0., 7.);
  // TODO: check that bin numbers is right
  TH1F* hTheta = new TH1F("theta", "theta", 500, 0., TMath::Pi());
  TH1F* hPhi = new TH1F("phi", "phi", 500, 0., 2 * TMath::Pi());
  // TODO: check that bin range is right
  TH1F* hP = new TH1F("p", "momentum", 500, 0., 15.);
  TH1F* hPTrans = new TH1F("pTrans", "transverse momentum", 500, 0., 15.);
  TH1F* hEnergy = new TH1F("energy", "Energy", 500, 0., 15.);
  TH1F* hInvMass = new TH1F("invMass", "invaraint mass", 500, 0., 15.);
  // clang-format off
  TH1F* hInvMassDiscordant = new TH1F("invMassDiscordant", "invaraint mass for discordant charges", 500, 0., 15.);
  TH1F* hInvMassConcordant = new TH1F("invMassDiscordant", "invaraint mass for concordant charges", 500, 0., 15.);
  TH1F* hInvMassDiscordantKP = new TH1F("invMassDiscordant", "invaraint mass for discordant charges pion and kaon", 500, 0., 15.);
  TH1F* hInvMassConcordantKP = new TH1F("invMassDiscordant", "invaraint mass for concordant charges pion and kaon", 500, 0., 15.);
  TH1F* hInvMassDecay = new TH1F("invMassDiscordant", "invaraint mass for particles generated through decay", 500, 0.8, 0.9);
  // clang-format on

  hInvMass->Sumw2();
  hInvMassDiscordant->Sumw2();
  hInvMassConcordant->Sumw2();
  hInvMassDiscordantKP->Sumw2();
  hInvMassConcordantKP->Sumw2();
  hInvMassDecay->Sumw2();
#endif

  for (int l{}; l < eventNumber; ++l) {
    // particle generation loop
    for (int i{}; i < particleNumber; ++i) {
      tracker.reset();
      // generating momentum
      double phi, theta, p;
      double pArray[3];

      {
        Timer timer{"random number generation"};
        //phi = gRandom->Uniform(0, 2 * TMath::Pi());
        //theta = gRandom->Uniform(0, TMath::Pi());
        //p = gRandom->Exp(1);
      }

      continue;
      {
        Timer timer{"polar to cartesian"};
        polarToCartesian(pArray, theta, phi, p);
      }

      // randomly generates particle
      char* particleName = particleGen();

      // populate array of particles
      EventParticles[i].setP(pArray[0], pArray[1], pArray[2]);
      EventParticles[i].setIndex(particleName);
      // if particle is K+ it decays in two other particle
      {
        Timer timer{"decay"};
        if (particleName == particleNames::kPlus) {
          decayGen(EventParticles[particleNumber + overflow],
                   EventParticles[particleNumber + overflow + 1]);
          EventParticles[i].Decay2body(
              EventParticles[particleNumber + overflow],
              EventParticles[particleNumber + overflow + 1]);

          hInvMassDecay->Fill(EventParticles[particleNumber + overflow].invMass(
              EventParticles[particleNumber + overflow + 1]));
          // TODO: which histograms must also consider decayed particles?
          overflow += 2;
          tracker.print();
        }
      }
    }
  }
  return 0;
}