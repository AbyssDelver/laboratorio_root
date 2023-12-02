#include <iostream>
//TODO: remove, and remove timers
#include "timer.hpp"
#include "particletype.hpp"


ParticleType::ParticleType(const char* name, double mass, int charge): fName{name}, fMass{mass}, fCharge{charge} {};
const char* ParticleType::getName() const{
    return fName;
}

ParticleType::~ParticleType() = default;

double ParticleType::getMass() const{
    return fMass;
}
int ParticleType::getCharge() const{
    return fCharge;
}

void ParticleType::print() const{
    std::cout << "name: " << fName << '\n'; 
    std::cout << "mass: " << fMass << '\n'; 
    std::cout << "charge: " << fCharge << '\n'; 
}

double ParticleType::getWidth() const{
  return 0.;  
}
