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
 * rng.h
 *
 *  Created on: Jul 3, 2013
 *      Author: marc
 */

#ifndef RNG_H_
#define RNG_H_

#include <climits> // ULONG_MAX
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <iostream>

// Seeds for GNU GSL random number generators
typedef unsigned long int gslseed_t;

class RandomNumberGenerator
{
public:
    /*
    RandomNumberGenerator()
    : mRngType( gsl_rng_default ), mSeed( time(0) ) {
    	initializeRNG();
    }*/

    RandomNumberGenerator(const gslseed_t& seed)
        : mRngType( gsl_rng_default ), mSeed( seed )
    {
        initializeRNG();
    }

    RandomNumberGenerator(const RandomNumberGenerator& rhs)
    {
        mRngType = rhs.mRngType;
        mRng = gsl_rng_clone( rhs.mRng );
        mSeed = rhs.randomSeed();
        gsl_rng_set (mRng, mSeed);
    }

    ~RandomNumberGenerator()
    {
        gsl_rng_free( mRng );
    }

    // Random uniform number between 0 and 1.
    double uniform() const
    {
        return gsl_rng_uniform( mRng );
    }

    // Random uniform number between min and max
    double uniform(const double min, const double max) const
    {
        const double u = uniform();
        return ( min + u * (max - min) );
    }
    
    double gaussian(const double sigma) const
    {
        return ( gsl_ran_gaussian (mRng, sigma) );
    }

    // Beta distribution. Random x between 0 and 1.
    double beta(const double alpha, const double beta) const
    {
        return ( gsl_ran_beta(mRng, alpha, beta) );
    }

    gslseed_t randomSeed() const
    {
    	// I wrote the following after looking into the GSL code itself.
        const unsigned long int offset = mRng->type->min;
        const unsigned long int range = mRng->type->max - offset;
        gslseed_t rseed = 1 + gsl_rng_uniform_int(mRng, range-2);
        //std::cerr << "generated random seed = " << rseed << std::endl;
        return rseed;
    }

protected:
    void initializeRNG()
    {
        mRng = gsl_rng_alloc (mRngType);
        gsl_rng_set (mRng, mSeed);
    }

private:
    // Prevent copying and assignment
    //RandomNumberGenerator(const RandomNumberGenerator&);
    void operator=(const RandomNumberGenerator&);

    const gsl_rng_type* mRngType;
    gslseed_t			mSeed;
    gsl_rng* 			mRng;
};


#endif /* RNG_H_ */
