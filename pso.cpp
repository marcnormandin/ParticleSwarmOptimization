/*
 * pso.cpp
 *
 *  Created on: Oct 23, 2013
 *      Author: marc
 */
#include "pso.h"

const double PSO::mC1 = 2.0;
const double PSO::mC2 = 2.0;

bool operator<(const Particle& p1, const Particle& p2)
{
    // hack
    return (p1.mPBestFitness < p2.mPBestFitness);
    //return (p1.mFitness < p2.mFitness);
}
