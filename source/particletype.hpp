#ifndef PARTICLETYPE_HPP
#define PARTICLETYPE_HPP
class ParticleType{

    public:
    ParticleType(const char*, double, int);

    virtual void print() const;
    virtual ~ParticleType();

    //TODO: should it return a const??
    const char* getName() const;
    double getMass() const;
    int getCharge() const;
    virtual double getWidth() const;

    private:
    const char* fName;
    double fMass;
    int fCharge;
};
#endif