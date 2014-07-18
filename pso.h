/*
 * pso.h
 *
 *  Created on: Jun 30, 2013
 *      Author: marc
 */

#ifndef PSO_H_
#define PSO_H_

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
    // Used by toymodel.cpp for an initial best particle
    Particle()
    {
        mFitness = -1.0 * std::numeric_limits<double>::max();
        mPBestFitness = -1.0 * std::numeric_limits<double>::max();
    }

    // Fixme   This doesn't really construct a valid particle.
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
        //std::cout << "Creating particle: Initial valued constructor.\n";

        mPos = pos;
        mVel.resize(pos.size());
        mPBestPos = pos;
        mFitness  = fitness;
        mPBestFitness = fitness;
    }

    ~Particle()
    {
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
                    //std::cout << "vel < -1.0 * maxvel\n";
                }
                else
                {
                    vel = maxVel;
                    //std::cout << "vel > +1.0 * maxvel\n";
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

            /*
            if ( pos < dim[i].min() ) {
            	pos = dim[i].min() + 0.1 * randOne * (dim[i].max() - dim[i].min());
            	vel = 0.0;
            	std::cout << "pos < min\n";
            } else if ( pos > dim[i].max() ) {
            	pos = dim[i].max() - 0.1 * randOne * (dim[i].max() - dim[i].min());
            	vel = 0.0;
            	std::cout << "pos > max\n";
            }*/

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

    /*
    PSO(const std::vector<Particle>& initialParticles, const Particle& GBest, const std::vector<Dim>& dim,
        const gslseed_t seed)
        : mParticles(initialParticles), mGBest(GBest), mDim(dim), mRng(seed)
    {
        std::cout << "PSO created\n";
    }*/

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
        	fitnessFunction(mParticles, &particleFitnesses);

            //For each particle
            for (unsigned int i = 0; i < mParticles.size(); i++)
            {
                // Calculate fitness value
                const prob_t fitness = particleFitnesses[i]; //fitnessFunction( mParticles[i] );

                // If the fitness value is better than the best fitness value (pBest) in history
                // set current value as the new pBest
                mParticles[i].updateFitness( fitness );
            }

            // Choose the particle with the best fitness value of all the particles as the gBest
            std::vector<Particle>::const_iterator best = std::max_element(mParticles.begin(), mParticles.end());
            mGBest = Particle( best->getPBestPosition(), best->getPBestFitness() );

            // hack
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

            // Update the inertia weight
            const double inertiaWeight = computeInertiaWeight(numIterations, maxIterations);
            
            // For each particle
            for (unsigned int i = 0; i < mParticles.size(); i++)
            {
                //const double randOne = mRng.uniform();
                //const double randTwo = mRng.uniform();
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
        //mParticles.reserve(mNumParticles);
        
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
        static const double omega1 = 0.9; // Soumya gave these values from top of his head. Need to verify.
        static const double omega2 = 0.4;

        // Note. We add +1 because our iterations go from 0 to max-1.
        double inertiaWeight = (omega1 - omega2) * ( (maxInterations - (iteration+1.0)) / (1.0 * (iteration+1.0) ) ) + omega2;
        return inertiaWeight;
    }

private:
    unsigned int            mNumParticles;
    std::vector<Particle> 	mParticles; // Particle positions
    Particle 				mGBest;
    std::vector<Dim>		mDim;

    static const double 	mC1;
    static const double 	mC2;

    RandomNumberGenerator 	mRng;
};


#endif /* PSO_H_ */
