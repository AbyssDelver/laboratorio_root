#ifndef PARTICLETYPE_HPP
#define PARTICLETYPE_HPP
class ParticleType{

    public:
    ParticleType(char*, double, int);

    virtual void print() const;
    virtual ~ParticleType();

    char* getName() const;
    double getMass() const;
    int getCharge() const;

    private:
    char* fName;
    double fMass;
    int fCharge;
};
#endif