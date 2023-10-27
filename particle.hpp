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

 private:
 //todo: check if static is right.
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