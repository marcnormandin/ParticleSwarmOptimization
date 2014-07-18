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

class PSO
{
public:
    // This routine is used by the PSO unit test
    PSO(const unsigned int numParticles, const std::vector<Dim>& dim,
        const gslseed_t seed)
        : mNumParticles(numParticles), mGBest(dim), mDim(dim), mRng(seed)
    {
        mParticles.reserve(mNumParticles);
    }

    unsigned int getNumParticles() const
    {
        return mNumParticles;
    }

    template<class FitnessFunction>
    const Particle& compute(FitnessFunction& fitnessFunction, const unsigned int maxIterations, std::ostream* out)
    {
        assert(maxIterations > 0);

        createRandomParticles();
        
        // Write the dimension of the particles
        if (out != NULL)
        {
            *out << mDim.size() << std::endl;
        }

        unsigned int numIterations = 0;
        std::vector<double> particleFitnesses(mParticles.size());
        do
        {
            // Evaluate the fitness/objective function
        	fitnessFunction(mParticles, &particleFitnesses);

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
            mGBest = Particle( best->getPBestPosition(), best->getPBestFitness() );

            // hack
            /*
            std::cout << "PSO iteration " << numIterations << ": best found = (";
            for (unsigned int k = 0; k < mDim.size(); k++)
            {
                std::cout << mGBest.getPosition()[k];
                if (k != mDim.size()-1)
                {
                    std::cout << ",";
                }
                else
                {
                    std::cout << ")";
                }
            }
            std::cout << std::endl;
             */

            // Update the inertia weight
            const double inertiaWeight = computeInertiaWeight(numIterations, maxIterations);
            
            // For each particle
            for (unsigned int i = 0; i < mParticles.size(); i++)
            {
                mParticles[i].updatePosition( mGBest, mDim, mC1, mC2, mRng, inertiaWeight );

                if (out != NULL)
                {
                    mParticles[i].recordPosition(*out);
                }
            }
            if (out != NULL)
            {
                *out << std::endl;
            }

            numIterations++;
        }
        while(numIterations < maxIterations);

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
    static const double 	mC1;
    static const double 	mC2;
    
    // These values control the rate of convergence
    static const double     mOmega1;
    static const double     mOmega2;

    RandomNumberGenerator 	mRng;
};


#endif /* PSO_H_ */
