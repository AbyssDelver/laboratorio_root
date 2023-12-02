#ifndef RESONANCETYPE_HPP
#define RESONANCETYPE_HPP
#include "particletype.hpp"

class ResonanceType : public ParticleType{
    public:
    ResonanceType(const char* name, double mass, int charge, double fWidth);
    double getWidth() const override;
    void print() const override;
    private:
    double fWidth;
};
#endif