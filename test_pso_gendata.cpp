/*
 * test_pso_gendata.cpp
 *
 *  Created on: Jul 1, 2013
 *      Author: marc
 */

#include <cassert>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <gsl/gsl_math.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_sf_exp.h>
#include <gsl/gsl_sf_trig.h>

#include "pso.h"
#include "rng.h"

class GaussianFunction
{
public:
    GaussianFunction(const double mean, const double std, const gslseed_t seed)
        : mMean(mean), mStd(std), mRng(seed)
    {
        // Generate data from the true distribution
        const unsigned int numDataPoints = 1000;
        for (unsigned int i = 0; i < numDataPoints; i++)
        {
            double x = mMean + mRng.gaussian( mStd );
            mData.push_back( x );
        }
    }

    void operator()(const std::vector<Particle>& particleSet, std::vector<double>* particleFitnesses)
	{
		for(unsigned int particleNum = 0; particleNum < particleSet.size(); ++particleNum)
		{
			(*particleFitnesses)[ particleNum ] = operator()( particleSet[ particleNum ] );
		}
	}

    double operator()(const Particle& p) const
    {
        const double mean = p.getPosition()[0];
        double totalProb = 0.0;
        for (unsigned int i = 0; i < mData.size(); i++)
        {
            double prob = gsl_ran_gaussian_pdf( mData[i] - mean, mStd);
            totalProb += log(prob);
        }
        return totalProb;
    }

    double computeSampleMean() const
    {
        double mean = 0.0;
        for (unsigned int i = 0; i < mData.size(); i++)
        {
            mean += mData[i];
        }
        mean /= 1.0*mData.size();
        return mean;
    }

private:
    double mMean;
    double mStd;

    RandomNumberGenerator mRng;

    std::vector<double> mData;
};

class AckleyFunction
{
public:
    // Formula found from:
    // http://anvilofcode.wordpress.com/2012/01/21/optimizing-the-ackleys-function/
    AckleyFunction(const double muX = 0.0, const double muY = 0.0)
        : mTrueX(muX), mTrueY(muY)
    {
    }

    void operator()(const std::vector<Particle>& particleSet, std::vector<double>* particleFitnesses)
	{
		for(unsigned int particleNum = 0; particleNum < particleSet.size(); ++particleNum)
		{
			(*particleFitnesses)[ particleNum ] = operator()( particleSet[ particleNum ] );
		}
	}

    double operator()(const Particle& p) const
    {
        const double x = p.getPosition()[0];
        const double y = p.getPosition()[1];

        // Flip the Ackely function because PSO looks for a maximum.
        return (-1.0 * ackley(x,y));
    }

    double ackley(const double x, const double y) const
    {
        const double termOne = 20.0 + gsl_sf_exp(1.0);

        const double termTwoArg = -0.5 * (gsl_pow_2(x - mTrueX) + gsl_pow_2(y - mTrueY));
        const double termTwo = -20.0 * gsl_sf_exp( termTwoArg );

        const double termThreeArg = 0.5 * (gsl_sf_cos(2.0*M_PI*(x - mTrueX)) + gsl_sf_cos(2.0*M_PI*(y - mTrueY)));
        const double termThree = -1.0 * gsl_sf_exp( termThreeArg );

        return (termOne + termTwo + termThree);
    }

private:
    double mTrueX;
    double mTrueY;
};

void evaluateGaussian1D(const unsigned int numParticles, const unsigned int maxIterations,
                        const std::string& fn)
{
    std::cout << "\nEvaluating: One-dimensional Gaussian with unknown mean.\n";

    std::ostringstream oss;
    oss << fn.c_str() << "_gaussian_1d.part";
    std::ofstream out( oss.str().c_str() );

    std::vector<Dim> dims;
    dims.push_back( Dim(-100, 100) );

    const double trueMean = 0.0;
    const double trueStd = 1.0;
    const gslseed_t gaussianSeed = 0;
    const gslseed_t psoSeed = 0;

    GaussianFunction ff( trueMean, trueStd, gaussianSeed );
    PSO pso( numParticles, dims, psoSeed );
    Particle p = pso.compute(ff, maxIterations, &out);
    std::cout << "True mean = " << trueMean << std::endl;
    std::cout << "Standard sample mean = " << ff.computeSampleMean() << std::endl;
    std::cout << "Best PSO mean found = " << p.getPosition()[0] << std::endl;
}

void evaluateAckley(const unsigned int numParticles, const unsigned int maxIterations,
                    const std::string& fn)
{
    std::cout << "\nEvaluating: Two-dimensional Ackley.\n";

    std::ostringstream oss;
    oss << fn.c_str() << "_ackley.part";
    std::ofstream out( oss.str().c_str() );

    std::vector<Dim> dims;
    dims.push_back( Dim(-10, 10) );
    dims.push_back( Dim(-10, 10) );

    const double trueX = 4.0;
    const double trueY = 6.8;
    AckleyFunction ff( trueX, trueY );

    const gslseed_t psoSeed = 0;
    PSO pso( numParticles, dims, psoSeed );

    Particle p = pso.compute(ff, maxIterations, &out);
    const double psoBestX = p.getPosition()[0];
    const double psoBestY = p.getPosition()[1];


    std::cout << "True: (x,y) = (" << trueX << "," << trueY << ")" << std::endl;
    std::cout << "PSO best: (x,y) = (" << psoBestX << "," << psoBestY << ")" << std::endl;
}


int main(int argc, char* argv[])
{
    if (argc != 4)
    {
        std::cout << "Error: Invalid arguments.\n";
        std::cout << "Usage: " << argv[0] << " <num particles> <max iterations> <output file prefix>\n";
        return -1;
    }

    const unsigned int numParticles = atoi( argv[1] );
    assert(numParticles > 0);

    const unsigned int maxIterations = atoi( argv[2] );

    const std::string fn( argv[3] );

    // Fixme
    //evaluateGaussian1D( numParticles, maxIterations, fn );
    evaluateAckley( numParticles, maxIterations, fn );

    return 0;
}
