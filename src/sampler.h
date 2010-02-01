/***************************************************************************
 *   Copyright (C) 2005 by Krishnan S                                      *
 *   krishi@csa.iisc.ernet.in                                              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef SAMPLER_H
#define SAMPLER_H

#include "rnd_discrete_general.h"
#include "randsvm-types.h"
#include <iostream>
#include <vector>

using namespace std;
/**
@author Krishnan S
*/
class SAMPLER
{
	protected:
		static const int scale = 10000;
		RND_DISCRETE_GENERAL rndgen, rndgen1;
		ulong no_of_variables;
		size_t sample_size, no_of_constraints;
		bool *sampled_constr;
		double *prev_weights;
		ldouble total_weight;
	public:
		SAMPLER(const ulong &n, const size_t &d,const size_t &s);
		virtual const bool *ChooseSample(const bool *);
		virtual const bool *ChooseSample(const bool *, const bool *);
		virtual void UpdateWeights(const bool *);
		virtual void GetWeights(double *) const;
		ldouble TotalWeight() const;
		ldouble ViolatorsWeight(const bool *) const;
		bool DoUpdateWeights(const bool *) const;
		void SetSampleSize(size_t );
		~SAMPLER();
};

#endif
