#include "particle.hpp"

#include <cmath>
#include <cstring>
#include <iostream>

int Particle::fNParticleType = 0;
ParticleType* Particle::fParticleType[Particle::fMaxNumParticleType];

void Particle::addParticleType(char* name, double mass, int charge,
                               double width) {
  if (FindParticle(name) == -1 && fNParticleType != fMaxNumParticleType) {
    if (width == 0.) {
      ParticleType* particle = new ParticleType{name, mass, charge};
      fParticleType[fNParticleType] = particle;
    } else {
      ParticleType* particle = new ResonanceType{name, mass, charge, width};
      fParticleType[fNParticleType] = particle;
    }
    ++fNParticleType;
  };
}

void Particle::printParticleType() {
  for (int i{}; i < fNParticleType; ++i) {
    std::cout << "particle " << i << '\n';
    fParticleType[i]->print();
  }
}

void Particle::printParticle() const {
  std::cout << "name: " << fParticleType[fIndex]->getName() << '\n';
  std::cout << "momentum x direction: " << fPx << '\n';
  std::cout << "momentum y direction: " << fPy << '\n';
  std::cout << "momentum z direction: " << fPz << '\n';
}

Particle::Particle(char* name, double Px, double Py, double Pz)
    : fPx(Px), fPy(Py), fPz(Pz) {
  if (name == nullptr) {
    return;
  }
  fIndex = FindParticle(name);
  if (fIndex == -1) {
    throw "invalid particle type";
  }
};

int Particle::FindParticle(char* name) {
  for (int i{}; i < fNParticleType; i++) {
    if (fParticleType[i] != nullptr &&
        strcmp(name, fParticleType[i]->getName()) == 0) {
      return i;
    }
  }
  return -1;
}

int Particle::getIndex() const { return fIndex; }

void Particle::setIndex(int index) {
  if (index < fNParticleType) {
    fIndex = index;
  } else {
    std::cout << "index is not valid" << '\n';
  }
}

void Particle::setIndex(char* name) {
  int index = FindParticle(name);
  if (index != -1) {
    fIndex = index;
  }
}

double Particle::getPx() const { return fPx; };
double Particle::getPy() const { return fPy; };
double Particle::getPz() const { return fPz; };
int Particle::getCharge() const { return fParticleType[fIndex]->getCharge(); };
double Particle::getMass() const { return fParticleType[fIndex]->getMass(); };
double Particle::getEnergy() const {
  double mass = fParticleType[fIndex]->getMass();
  return std::sqrt(mass * mass + (fPx * fPx + fPy * fPy + fPz * fPz));
}

double Particle::invMass(Particle& p) const {
  double momentumSum = (fPx + p.fPx) * (fPx + p.fPx) +
                       (fPy + p.fPy) * (fPy + p.fPy) +
                       (fPz + p.fPz) * (fPz + p.fPz);
  double energySum =
      (this->getEnergy() + p.getEnergy()) * (this->getEnergy() + p.getEnergy());
  return std::sqrt(energySum - momentumSum);
}
void Particle::setP(double Px, double Py, double Pz) {
  fPx = Px;
  fPy = Py;
  fPz = Pz;
}

int Particle::Decay2body(Particle& dau1, Particle& dau2) const {
  if (getMass() == 0.0) {
    printf("Decayment cannot be preformed if mass is zero\n");
    return 1;
  }

  double massMot = getMass();
  double massDau1 = dau1.getMass();
  double massDau2 = dau2.getMass();

  if (fIndex > -1) {  // add width effect

    // gaussian random numbers

    float x1, x2, w, y1;

    double invnum = 1. / RAND_MAX;
    do {
      x1 = 2.0 * rand() * invnum - 1.0;
      x2 = 2.0 * rand() * invnum - 1.0;
      w = x1 * x1 + x2 * x2;
    } while (w >= 1.0);

    w = sqrt((-2.0 * log(w)) / w);
    y1 = x1 * w;

    massMot += fParticleType[fIndex]->getWidth() * y1;
  }

  if (massMot < massDau1 + massDau2) {
    printf(
        "Decayment cannot be preformed because mass is too low in this "
        "channel\n");
    return 2;
  }

  double pout =
      sqrt(
          (massMot * massMot - (massDau1 + massDau2) * (massDau1 + massDau2)) *
          (massMot * massMot - (massDau1 - massDau2) * (massDau1 - massDau2))) /
      massMot * 0.5;

  double norm = 2 * M_PI / RAND_MAX;

  double phi = rand() * norm;
  double theta = rand() * norm * 0.5 - M_PI / 2.;
  dau1.setP(pout * sin(theta) * cos(phi), pout * sin(theta) * sin(phi),
            pout * cos(theta));
  dau2.setP(-pout * sin(theta) * cos(phi), -pout * sin(theta) * sin(phi),
            -pout * cos(theta));

  double energy = sqrt(fPx * fPx + fPy * fPy + fPz * fPz + massMot * massMot);

  double bx = fPx / energy;
  double by = fPy / energy;
  double bz = fPz / energy;

  dau1.Boost(bx, by, bz);
  dau2.Boost(bx, by, bz);

  return 0;
}

void Particle::Boost(double bx, double by, double bz) {
  double energy = getEnergy();

  // Boost this Lorentz vector
  double b2 = bx * bx + by * by + bz * bz;
  double gamma = 1.0 / sqrt(1.0 - b2);
  double bp = bx * fPx + by * fPy + bz * fPz;
  double gamma2 = b2 > 0 ? (gamma - 1.0) / b2 : 0.0;

  fPx += gamma2 * bp * bx + gamma * bx * energy;
  fPy += gamma2 * bp * by + gamma * by * energy;
  fPz += gamma2 * bp * bz + gamma * bz * energy;
}