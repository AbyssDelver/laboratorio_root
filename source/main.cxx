#include <iostream>
#include "particle.hpp"

namespace particleNames {
char pionePlus[] = "pione+";
char pioneMinus[] = "pione-";
char kaonePlus[] = "kaone+";
char kaoneMinus[] = "kaone-";
char protonePlus[] = "protone+";
char protoneMinus[] = "protone-";
char kPlus[] = "K+";
}  // namespace particleNames

int main() {
  Particle::addParticleType(particleNames::pionePlus, 0.13957, 1);
  Particle::addParticleType(particleNames::pioneMinus, 0.13957, -1);
  Particle::addParticleType(particleNames::kaonePlus, 0.49367, 1);
  Particle::addParticleType(particleNames::kaoneMinus, 0.49367, -1);
  Particle::addParticleType(particleNames::protonePlus, 0.93827, 1);
  Particle::addParticleType(particleNames::protoneMinus, 0.93827, -1);
  Particle::addParticleType(particleNames::kPlus, 0.89166, 0, 0.050);
  Particle::printParticleType();

  for (int i{}; i < 10e7; ++i) {
    Particle p{particleNames::kPlus, 1., 1., 1.};
    Particle p1{particleNames::pionePlus, 1., 1., 1.};
    Particle p2{particleNames::kaoneMinus, 1., 1., 1.};

    p.Decay2body(p1, p2);
    p1.invMass(p2);
    // TODO: which histograms must also consider decayed particles?
  }
  std::cout << "program ended";

  return 0;
}