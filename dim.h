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
