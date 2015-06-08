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

/*
 * pso.h
 *
 *  Created on: Jun 30, 2013
 *      Author: marc
 */

#ifndef PSO_H_
#define PSO_H_

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
#include "particle.h"

template<class FitnessFunction>
class PSO
{
public:
    // This routine is used by the PSO unit test
    PSO(const unsigned int numParticles, const std::vector<Dim>& dim,
        const gslseed_t seed, FitnessFunction& fitnessFunction, const unsigned int maxIterations)
        : mNumParticles(numParticles), mGBest(dim), mDim(dim), mRng(seed), mFitnessFunction(fitnessFunction),
        mMaxIterations(maxIterations)
    {
        mParticles.reserve(mNumParticles);
    }

    unsigned int getNumParticles() const
    {
        return mNumParticles;
    }

    const Particle& iterate()
    {
        createRandomParticles();

        unsigned int numIterations = 0;
        std::vector<double> particleFitnesses(mParticles.size());
        do
        {
            // Evaluate the fitness/objective function
        	mFitnessFunction(mParticles, &particleFitnesses);

            //For each particle
            for (unsigned int i = 0; i < mParticles.size(); i++)
            {
                // Calculate fitness value
                const prob_t fitness = particleFitnesses[i]; //

                // If the fitness value is better than the best fitness value (pBest) in history
                // set current value as the new pBest
                mParticles[i].updateFitness( fitness );
            }

            // Choose the particle with the best fitness value of all the particles as the gBest
            std::vector<Particle>::const_iterator best = std::max_element(mParticles.begin(), mParticles.end());
            mGBest = Particle( best->getBestPosition(), best->getBestFitness() );

            // Update the inertia weight
            const double inertiaWeight = computeInertiaWeight(numIterations, mMaxIterations);
            
            // For each particle
            for (unsigned int i = 0; i < mParticles.size(); i++)
            {
                mParticles[i].updatePosition( mGBest, mDim, mCognitiveWeight, mSocialWeight, mRng, inertiaWeight );
            }

            numIterations++;
        }
        while(numIterations < mMaxIterations);

        return mGBest;
    }

protected:
    PSO(const PSO&);
    void operator=(const PSO&);
    
    void createRandomParticles() {
        mParticles.clear(); // Remove previous particles in the container
        
        // For each particle
        for (unsigned int i = 0; i < mNumParticles; i++)
        {
            std::vector<dim_t> pos;
            
            // For each dimension
            for (unsigned int d = 0; d < mDim.size(); d++)
            {
                dim_t posd = mRng.uniform( mDim[d].min(), mDim[d].max() );
                pos.push_back(posd);
            }
            
            // Add the particle to the collection
            mParticles.push_back( Particle(pos) );
        }
    }

    // Compute inertia. This is based on equation 4.1 from:
    // http://www.hindawi.com/journals/ddns/2010/462145/
    double computeInertiaWeight(const unsigned int iteration, const unsigned int maxInterations) const
    {
        // Note. We add +1 because our iterations go from 0 to max-1.
        double inertiaWeight = (mOmega1 - mOmega2) * ( (maxInterations - (iteration+1.0)) / (1.0 * (iteration+1.0) ) ) + mOmega2;
        return inertiaWeight;
    }

private:
    unsigned int            mNumParticles;
    std::vector<Particle> 	mParticles; // Particle positions
    Particle 				mGBest;
    std::vector<Dim>		mDim;

    // These values control how random the particle velocities are
    static const double 	mCognitiveWeight;
    static const double 	mSocialWeight;
    
    // These values control the rate of convergence
    static const double     mOmega1;
    static const double     mOmega2;

    RandomNumberGenerator 	mRng;

    FitnessFunction&        mFitnessFunction;
    unsigned int            mMaxIterations;
};


#endif /* PSO_H_ */
