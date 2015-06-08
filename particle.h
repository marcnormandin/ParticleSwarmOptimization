/*
 * Copyright 2014 Marc Normandin
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *     Unless required by applicable law or agreed to in writing, software
 *     distributed under the License is distributed on an "AS IS" BASIS,
 *     WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *     See the License for the specific language governing permissions and
 *     limitations under the License.
 */
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
