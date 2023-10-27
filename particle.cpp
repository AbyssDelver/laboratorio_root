#include "particle.hpp"

#include <cstring>
#include <iostream>

// todo: check if good practice, or makes sense
int Particle::fNParticleType = 0;
ParticleType* Particle::fParticleType[Particle::fMaxNumParticleType];

void Particle::addParticleType(char* name, double mass, int charge,
                               double width = 0.) {
  if (FindParticle(name) == -1 && fNParticleType != fMaxNumParticleType) {
    if (width == 0.) {
      ParticleType* particle = new ParticleType{name, mass, charge};
      fParticleType[fNParticleType] = particle;
    } else {
      ParticleType* particle = new ResonanceType{name, mass, charge, width};
      fParticleType[fNParticleType] = particle;
    }
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

Particle::Particle(char* name, double Px = 0., double Py = 0., double Pz = 0.)
    : fPx(Px), fPy(Py), fPz(Pz) {
  fIndex = FindParticle(name);
  if (fIndex == -1) {
    throw "invalid particle type";
  }
};

int Particle::FindParticle(char* name) {
  for (int i{}; i < fNParticleType; i++) {
    if (fParticleType[i] != nullptr &&
        strcmp(name, fParticleType[i]->getName())) {
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

  double Particle::getPx() const{
    return fPx;
  };
  double Particle::getPy() const{
    return fPy;
  };
  double Particle::getPz() const{
    return fPz;
  };
