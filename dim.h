/*
 * dim.h
 *
 *  Created on: Jul 2, 2013
 *      Author: marc
 */

#ifndef DIM_H_
#define DIM_H_

#include <cassert>

typedef double prob_t;
typedef double dim_t;

class Dim
{
public:
    // Fixme
    Dim() : mMin(1.0), mMax(2.0) {}

    Dim(const dim_t mi, const dim_t ma)
        : mMin(mi), mMax(ma)
    {
        assert(mMax >= mMin);
    }

    dim_t min() const
    {
        return mMin;
    }
    dim_t max() const
    {
        return mMax;
    }

private:
    dim_t mMin;
    dim_t mMax;
};


#endif /* DIM_H_ */
