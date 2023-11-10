#ifndef PARTICLETYPE_HPP
#define PARTICLETYPE_HPP
class ParticleType{

    public:
    ParticleType(const char*, double, int);

    virtual void print() const;
    virtual ~ParticleType();

    const char* getName() const;
    double getMass() const;
    int getCharge() const;
    int getWidth() const;

    private:
   const char* fName;
    double fMass;
    int fCharge;
};
#endif