/***************************************************************************
 *   Copyright (C) 2005 by Krishnan S                                      *
 *   krishi@csa.iisc.ernet.in                                              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *    the Free Software Foundation; either version 2 of the License, or     *
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
#ifndef RND_DISCRETE_GENERAL_H
#define RND_DISCRETE_GENERAL_H


#include "rnd.h"
#include "randsvm-types.h"
#include <iostream>

using namespace std;
/**
Class for arbitrary discrete distributions


@author Krishnan S
*/
class RND_DISCRETE_GENERAL : public RND
{
protected:
	double *weights, *tmp_weights;
	size_t k, next_indx, *indx;
	gsl_ran_discrete_t *rndgen;
public:
	RND_DISCRETE_GENERAL(const size_t );
	RND_DISCRETE_GENERAL(const size_t , const double );
	RND_DISCRETE_GENERAL(const size_t , const double *);
	
	virtual void SetWeights(const double *);
	virtual inline void UpdateWeight(const size_t  indx, const double w) 
	{
		tmp_weights[indx] = weights[indx] = w;
	}
	virtual inline double Weight(const size_t indx) const
	{
		return weights[indx];
	}
	virtual inline ulong Get() 
	{
		if( !rndgen )
		{
			cout << "RND_DISCRETE_GENERAL::Get() : Weights not assigned" << endl;
			return 0;
		}
		
		return gsl_ran_discrete(rng, rndgen);
		/*if( rndgen )
			gsl_ran_discrete_free(rndgen);
		//rndgen = gsl_ran_discrete_preproc(k, tmp_weights);
		//return gsl_ran_discrete(rng, rndgen);
		rndgen = gsl_ran_discrete_preproc(k - next_indx, tmp_weights + next_indx);
		
		ulong next = gsl_ran_discrete(rng, rndgen);
		
		ulong t = indx[next_indx];
		indx[next_indx] = indx[next_indx + next];
		indx[next_indx + next] = t;
		
		double tw = tmp_weights[next_indx];
		tmp_weights[next_indx] = tmp_weights[next_indx + next];
		tmp_weights[next_indx + next] = tw;
		
		next_indx++;
		if( !(next_indx%1000) )
		{
			cout << next_indx << ".." ;
			cout.flush();
		}
		
		return indx[next_indx - 1];*/
	}
	virtual void UpdateGenerator();
	~RND_DISCRETE_GENERAL();
};

#endif
