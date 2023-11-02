#include <iostream>
#include "particletype.hpp"
#include "resonancetype.hpp"

ResonanceType::ResonanceType(char* name, double mass, int charge, double width) : ParticleType(name, mass, charge), fWidth{width} {};

double ResonanceType::getWidth() const{
    return  fWidth;
}

void ResonanceType::print() const{
    ParticleType::print();
    std::cout << "width: " << fWidth << '\n'; 
}