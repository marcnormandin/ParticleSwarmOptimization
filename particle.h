#ifndef INC_PARTICLE_H
#define INC_PARTICLE_H

#include "common_types.h"
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
    Particle()
    {
        mFitness = -1.0 * std::numeric_limits<double>::max();
        mPBestFitness = -1.0 * std::numeric_limits<double>::max();
    }
    
    // Fixme   This doesn't really construct a fully valid particle.
    Particle(const std::vector<Dim>& dim)
    {
        mPos.resize(dim.size());
        mVel.resize(dim.size());
        mFitness = -1.0 * std::numeric_limits<dim_t>::max();
        mPBestPos = mPos;
        mPBestFitness = mFitness;
    }
    
    Particle(const std::vector<dim_t>& pos, const prob_t fitness = -1.0 * std::numeric_limits<dim_t>::max())
    {
        mPos = pos;
        mVel.resize(pos.size());
        mPBestPos = pos;
        mFitness  = fitness;
        mPBestFitness = fitness;
    }
    
    const std::vector<dim_t>& getPosition() const
    {
        return mPos;
    }
    
    std::vector<dim_t> getVelocity() const
    {
        return mVel;
    }
    
    double getFitness() const
    {
        return mFitness;
    }
    
    std::vector<dim_t> getPBestPosition() const
    {
        return mPBestPos;
    }
    
    double getPBestFitness() const
    {
        return mPBestFitness;
    }
    
    void updateFitness(const prob_t newFitness)
    {
        mFitness = newFitness;
        
        // Update the particles personal best if the new fitness is better
        if (newFitness > mPBestFitness)
        {
            mPBestFitness = newFitness;
            mPBestPos = mPos;
        }
    }
    
    void updatePosition(const Particle& GBest, const std::vector<Dim>& dim, const double C1, const double C2,
                        const RandomNumberGenerator& rng, const double inertiaWeight)
    {
        std::vector<dim_t> newVel;
        std::vector<dim_t> newPos;
        
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
    
    friend bool operator<(const Particle& p1, const Particle& p2);
    
    void recordPosition(std::ostream& out) const
    {
        for (unsigned int i = 0; i < mPos.size(); i++)
        {
            out << mPos[i] << "\t";
        }
    }
    
    size_t size() const
    {
        return mPos.size();
    }
    
private:
    std::vector<dim_t> 	mPos;
    std::vector<dim_t> 	mVel;
    prob_t					mFitness;
    
    // The particles best position
    std::vector<dim_t>		mPBestPos;
    prob_t 					mPBestFitness;
};

bool operator<(const Particle& p1, const Particle& p2);

#endif // #ifndef INC_PARTICLE_H
