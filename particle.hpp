#ifndef PARTICLE_HPP
#define PARTICLE_HPP
#include "particletype.hpp"
#include "resonancetype.hpp"

class Particle {
  // todo: make private
 public:
  Particle(char*, double, double, double);
  int getIndex() const;
  static void addParticleType(char*, double, int, double);
  void setIndex(int index);
  void setIndex(char* name);
  static void printParticleType();
  void printParticle() const;
  double getPx() const;
  double getPy() const;
  double getPz() const;
  double getMass() const;
  double getEnergy() const;
  double invMass(Particle& p) const;
  void setP(double px,double py,double pz);

 private:
  // todo: check if static is right.
  static int FindParticle(char* name);
  static const int fMaxNumParticleType = 10;
  static int fNParticleType;
  static ParticleType* fParticleType[fMaxNumParticleType];

  int fIndex;
  double fPx;
  double fPy;
  double fPz;
};

#endif