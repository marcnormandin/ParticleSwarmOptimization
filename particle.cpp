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
#include "particle.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <cmath>
#include <cstdlib>
#include <gsl/gsl_rng.h>
#include <limits>
#include <cassert>

#include "rng.h"
#include "dim.h"

Particle::Particle()
{
    mFitness = -1.0 * std::numeric_limits<double>::max();
    mPBestFitness = -1.0 * std::numeric_limits<double>::max();
}

// Fixme   This doesn't really construct a fully valid particle.
Particle::Particle(const std::vector<Dim>& dim)
{
    mPos.resize(dim.size());
    mVel.resize(dim.size());
    mFitness = -1.0 * std::numeric_limits<dim_t>::max();
    mPBestPos = mPos;
    mPBestFitness = mFitness;
}

Particle::Particle(const dvector& pos, const prob_t fitness)
{
    mPos = pos;
    mVel.resize(pos.size());
    mPBestPos = pos;
    mFitness  = fitness;
    mPBestFitness = fitness;
}

const Particle::dvector& Particle::getPosition() const
{
    return mPos;
}

Particle::dvector Particle::getVelocity() const
{
    return mVel;
}

double Particle::getFitness() const
{
    return mFitness;
}

Particle::dvector Particle::getBestFitness() const
{
    return mPBestPos;
}

double Particle::getBestFitness() const
{
    return mPBestFitness;
}

void Particle::updateFitness(const prob_t newFitness)
{
    mFitness = newFitness;
    
    // Update the particles personal best if the new fitness is better
    if (newFitness > mPBestFitness)
    {
        mPBestFitness = newFitness;
        mPBestPos = mPos;
    }
}

void Particle::updatePosition(const Particle& GBest, const std::vector<Dim>& dim, const double C1, const double C2,
                    const RandomNumberGenerator& rng, const double inertiaWeight)
{
    dvector newVel;
    dvector newPos;
    
    assert(mPos.size() == mVel.size());
    assert(mPos.size() == dim.size());
    assert(mPos.size() == mPBestPos.size());
    assert(mPos.size() == GBest.mPos.size());
    assert(mPos.size() == GBest.mVel.size());
    
    
    // Loop over each dimension
    for (unsigned int i = 0; i < mPos.size(); i++)
    {
        assert(dim[i].max() >= dim[i].min());
        
        // Calculate particle velocity according equation (a)
        // v[new] = v[old] + c1 * rand() * (pbest[new] - present[old]) + c2 * rand() * (gbest[new] - present[old]) (a)
        dim_t vel = inertiaWeight * mVel[i];
        dim_t diff1 = mPBestPos[i] - mPos[i];
        dim_t diff2 = GBest.getPosition()[i] - mPos[i];
        vel += C1 * rng.uniform() * diff1;
        vel += C2 * rng.uniform() * diff2;
        
        
        // hack!
        if (vel == 0.0) {
            // The GBest particle ends up having zero velocity using the
            // above formulas. So give it a kick in the pants!
            vel = mVel[i] + 2.0 * rng.uniform();
        }
        
        // Apply bounds to the velocity
        const dim_t maxVel = 0.01*(dim[i].max() - dim[i].min());
        if ( fabs(vel) > maxVel )
        {
            if (vel < 0.0)
            {
                vel = -1.0*maxVel;
            }
            else
            {
                vel = maxVel;
            }
        }
        
        // Update particle position according equation (b)
        // present[new] = present[old] + v[new] (b)
        dim_t pos = mPos[i] + vel;
        
        // Apply bounds to the position
        if ( pos < dim[i].min() )
        {
            pos = dim[i].min();
        }
        else if ( pos > dim[i].max())
        {
            pos = dim[i].max();
        }
        
        newVel.push_back(vel);
        newPos.push_back(pos);
    }
    
    mPos = newPos;
    mVel = newVel;
}

size_t Particle::size() const
{
    return mPos.size();
}

bool operator<(const Particle& p1, const Particle& p2)
{
    // hack
    return (p1.mPBestFitness < p2.mPBestFitness);
    //return (p1.mFitness < p2.mFitness);
}

