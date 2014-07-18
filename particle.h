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
    Particle();
    
    Particle(const std::vector<Dim>& dim);
    
    Particle(const std::vector<dim_t>& pos, const prob_t fitness = -1.0 * std::numeric_limits<dim_t>::max());
    
    const std::vector<dim_t>& getPosition() const;
    
    std::vector<dim_t> getVelocity() const;
    
    double getFitness() const;
    
    std::vector<dim_t> getPBestPosition() const;
    
    double getPBestFitness() const;
    
    void updateFitness(const prob_t newFitness);
    
    void updatePosition(const Particle& GBest, const std::vector<Dim>& dim, const double C1, const double C2,
                        const RandomNumberGenerator& rng, const double inertiaWeight);
    
    void recordPosition(std::ostream& out) const;
    
    size_t size() const;
    
    friend bool operator<(const Particle& p1, const Particle& p2);
    
private:
    std::vector<dim_t>      mPos;
    std::vector<dim_t>      mVel;
    prob_t					mFitness;
    
    // The particles best position
    std::vector<dim_t>		mPBestPos;
    prob_t 					mPBestFitness;
};

bool operator<(const Particle& p1, const Particle& p2);

#endif // #ifndef INC_PARTICLE_H
