#ifndef INC_PARTICLE_H
#define INC_PARTICLE_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <cmath>
#include <cstdlib>
#include <gsl/gsl_rng.h>
#include <limits>
#include <fstream>
#include <cassert>

#include "rng.h"
#include "dim.h"


class Particle
{
public:
    typedef std::vector<dim_t> dvector;
    
    Particle();
    
    Particle(const std::vector<Dim>& dim);
    
    Particle(const dvector& pos, const prob_t fitness = -1.0 * std::numeric_limits<dim_t>::max());
    
    const dvector& getPosition() const;
    
    dvector getVelocity() const;
    
    double getFitness() const;
    
    dvector getPBestPosition() const;
    
    double getPBestFitness() const;
    
    void updateFitness(const prob_t newFitness);
    
    void updatePosition(const Particle& GBest, const std::vector<Dim>& dim, const double C1, const double C2,
                        const RandomNumberGenerator& rng, const double inertiaWeight);
    
    void recordPosition(std::ostream& out) const;
    
    size_t size() const;
    
    friend bool operator<(const Particle& p1, const Particle& p2);
    
private:
    dvector     mPos;
    dvector     mVel;
    prob_t      mFitness;
    
    // The particles best position
    dvector		mPBestPos;
    prob_t 		mPBestFitness;
};

bool operator<(const Particle& p1, const Particle& p2);

#endif // #ifndef INC_PARTICLE_H
