#include <chrono>
#include <iostream>
#include <memory>

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
#include "timer.hpp"

namespace particleNames {
char pionPlus[] = "pione+";
char pionMinus[] = "pione-";
char kaonPlus[] = "kaone+";
char kaonMinus[] = "kaone-";
char protonPlus[] = "protone+";
char protonMinus[] = "protone-";
char kStar[] = "K+";
}  // namespace particleNames

void polarToCartesian(double* pArray, double theta, double phi, double p) {
  pArray[0] = p * TMath::Sin(theta) * TMath::Cos(phi);
  pArray[1] = p * TMath::Sin(theta) * TMath::Sin(phi);
  pArray[2] = p * TMath::Cos(theta);
}

char* particleGen() {
  double random = gRandom->Rndm();
  if (random < 0.4) {
    return particleNames::pionPlus;
  } else if (random < 0.8) {
    return particleNames::pionMinus;
  } else if (random < 0.85) {
    return particleNames::kaonPlus;
  } else if (random < 0.9) {
    return particleNames::kaonMinus;
  } else if (random < 0.945) {
    return particleNames::protonPlus;
  } else if (random < 0.99) {
    return particleNames::protonMinus;
  } else {
    return particleNames::kStar;
  }
}

void decayGen(Particle& p1, Particle& p2) {
  double random = gRandom->Rndm();
  if (random < 0.5) {
    p1.setIndex(particleNames::pionPlus);
    p2.setIndex(particleNames::kaonMinus);
  } else {
    p1.setIndex(particleNames::pionMinus);
    p2.setIndex(particleNames::kaonPlus);
  }
}

int main() {
  // constants
  constexpr int eventNumber = 100000;
  constexpr int particleNumber = 100;
  constexpr int particleCapacity = 200;

  Timer timer{"total timer"};
  gRandom->SetSeed();

  Particle EventParticles[particleCapacity];

  // number of particles in addition to the generated 100 particles (generated
  // by decay).
  int overflow{0};

  Particle::addParticleType(particleNames::pionPlus, 0.13957, 1);
  Particle::addParticleType(particleNames::pionMinus, 0.13957, -1);
  Particle::addParticleType(particleNames::kaonPlus, 0.49367, 1);
  Particle::addParticleType(particleNames::kaonMinus, 0.49367, -1);
  Particle::addParticleType(particleNames::protonPlus, 0.93827, 1);
  Particle::addParticleType(particleNames::protonMinus, 0.93827, -1);
  Particle::addParticleType(particleNames::kStar, 0.89166, 0, 0.050);

  TFile* file = new TFile("./histograms/histos.root", "RECREATE");

  // initializing histograms
  TH1F* hTypes = new TH1F("types", "particle types", 7, 0., 7.);
  // TODO: check that bin numbers is right
  TH1F* hTheta = new TH1F("theta", "theta", 500, 0., TMath::Pi());
  TH1F* hPhi = new TH1F("phi", "phi", 500, 0., 2 * TMath::Pi());
  // TODO: check that bin range is right
  TH1F* hP = new TH1F("p", "momentum", 500, 0., 8.);
  TH1F* hPTrans = new TH1F("pTrans", "transverse momentum", 500, 0., 6.);
  TH1F* hEnergy = new TH1F("energy", "Energy", 500, 0., 8.);
  TH1F* hInvMass = new TH1F("invMass", "invaraint mass", 500, 0., 8.);
  // clang-format off
  TH1F* hInvMassDiscordant = new TH1F("invMassDiscordant", "invaraint mass for discordant charges", 500, 0., 8.);
  TH1F* hInvMassConcordant = new TH1F("invMassConcordant", "invaraint mass for concordant charges", 500, 0., 8.);
  TH1F* hInvMassDiscordantKP = new TH1F("invMassDiscordantKP", "invaraint mass for discordant charges pion and kaon", 500, 0., 8.);
  TH1F* hInvMassConcordantKP = new TH1F("invMassConcordantKP", "invaraint mass for concordant charges pion and kaon", 500, 0., 8.);
  TH1F* hInvMassDecay = new TH1F("invMassDecay", "invaraint mass for particles generated through decay", 70, 0.5, 1.5);
  // clang-format on

  hInvMass->Sumw2();
  hInvMassDiscordant->Sumw2();
  hInvMassConcordant->Sumw2();
  hInvMassDiscordantKP->Sumw2();
  hInvMassConcordantKP->Sumw2();
  hInvMassDecay->Sumw2();

  for (int l{}; l < eventNumber; ++l) {
    // particle generation loop
    for (int i{}; i < particleNumber; ++i) {
      // generating momentum
      double phi, theta, p;
      double pArray[3];
      phi = gRandom->Uniform(0, 2 * TMath::Pi());
      theta = gRandom->Uniform(0, TMath::Pi());
      p = gRandom->Exp(1);
      polarToCartesian(pArray, theta, phi, p);

      // randomly generates particle
      char* particleName = particleGen();

      // populate array of particles
      EventParticles[i].setP(pArray[0], pArray[1], pArray[2]);
      EventParticles[i].setIndex(particleName);

      // if particle is K* it decays in two other particle
      if (particleName == particleNames::kStar) {
        // assigning a particle type to the decay products
        decayGen(EventParticles[particleNumber + overflow],
                 EventParticles[particleNumber + overflow + 1]);
        EventParticles[i].Decay2body(
            EventParticles[particleNumber + overflow],
            EventParticles[particleNumber + overflow + 1]);
            
        hInvMassDecay->Fill(EventParticles[particleNumber + overflow].invMass(
            EventParticles[particleNumber + overflow + 1]));
        //  TODO: which histograms must also consider decayed particles?
        overflow += 2;
      }

      // filling histograms
      hTypes->Fill(EventParticles[i].getIndex());
      hTheta->Fill(theta);
      hPhi->Fill(phi);
      hP->Fill(p);
      hPTrans->Fill(TMath::Sqrt(
          (EventParticles[i].getPx()) * (EventParticles[i].getPx()) +
          (EventParticles[i].getPy()) * (EventParticles[i].getPy())));
      hEnergy->Fill(EventParticles[i].getEnergy());
      // end of event
    }

    //  filling invariant mass histograms
    for (int i{}; i < particleNumber + overflow; ++i) {
      for (int j{}; j < i; ++j) {
        // to make code more readable
        auto& particle = EventParticles[i];
        auto& other_particle = EventParticles[j];

        hInvMass->Fill(particle.invMass(EventParticles[j]));
        // TODO: what to do with K* particles that have 0 as charge?
        // concordant or not?
        //  if particle charges are discordant
        if (particle.getCharge() * other_particle.getCharge() < 0) {
          hInvMassDiscordant->Fill(particle.invMass(other_particle));
        }
        // if particle charges are concordant
        if (EventParticles[i].getCharge() * EventParticles[j].getCharge() > 0) {
          hInvMassConcordant->Fill(
              EventParticles[i].invMass(EventParticles[j]));
        }
        // if particles are pion+ and kaon-
        if ((particle.getIndex() == 0 && other_particle.getIndex() == 3) ||
            (particle.getIndex() == 3 && other_particle.getIndex() == 0)) {
          hInvMassDiscordantKP->Fill(particle.invMass(other_particle));
        };
        // if particles are pion- and kaon+
        if ((particle.getIndex() == 1 && other_particle.getIndex() == 2) ||
            (particle.getIndex() == 2 && other_particle.getIndex() == 1)) {
          hInvMassDiscordantKP->Fill(particle.invMass(other_particle));
        };
        // if particles are pion+ and kaon+
        if ((particle.getIndex() == 0 && other_particle.getIndex() == 2) ||
            (particle.getIndex() == 2 && other_particle.getIndex() == 0)) {
          hInvMassConcordantKP->Fill(particle.invMass(other_particle));
        };
        // if particles are pion- and kaon-
        if ((particle.getIndex() == 1 && other_particle.getIndex() == 3) ||
            (particle.getIndex() == 3 && other_particle.getIndex() == 1)) {
          hInvMassConcordantKP->Fill(particle.invMass(other_particle));
        };
      }
    }

    overflow = 0;
  }

  // writing to file:
  file->Write();
  file->Close();

  return 0;
}