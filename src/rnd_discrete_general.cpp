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
#include "rnd_discrete_general.h"
#include <iostream>
#include <time.h>

using namespace std;
RND_DISCRETE_GENERAL::RND_DISCRETE_GENERAL(const size_t n): RND(gsl_rng_taus), k(n), next_indx(0), rndgen(NULL), weights(NULL)
{
	if( !rng )
		return;
	weights = new double[k];
	if( !weights )
	{
		cerr << "RND_DISCRETE_GENERAL::Unable to allocate memory for weights" << endl;
		gsl_rng_free(rng);
		return;
	}
	tmp_weights = new double [k];
	if( !tmp_weights )
	{
		delete [] weights;
		cerr << "RND_DISCRETE_GENERAL::Unable to allocate memory for weights" << endl;
		gsl_rng_free(rng);
		return;
	}
			
}


RND_DISCRETE_GENERAL::RND_DISCRETE_GENERAL(const size_t n,const double w): RND(gsl_rng_taus), k(n), next_indx(0), rndgen(NULL), weights(NULL)
{
	weights = new double[k];
	if( !weights )
	{
		cerr << "RND_DISCRETE_GENERAL::Unable to allocate memory for weights" << endl;
		gsl_rng_free(rng);
		return;
	}
	tmp_weights = new double [k];
	if( !tmp_weights )
	{
		delete [] weights;
		cerr << "RND_DISCRETE_GENERAL::Unable to allocate memory for weights" << endl;
		gsl_rng_free(rng);
		return;
	}
	indx = new size_t[k];
	
	
	for( unsigned i = 0; i < k; i++ )
	{			
		tmp_weights[i] = weights[i] = w;
		indx[i] = i;
	}
	
	
	rndgen = gsl_ran_discrete_preproc(k, weights);
	
	if( !rndgen )
	{
		cerr << "RND_DISCRETE_GENERAL::Unable to allocate the Distribution Generator" << endl;
		delete []weights;
		delete []tmp_weights;
		return;
	}
	
}

RND_DISCRETE_GENERAL::RND_DISCRETE_GENERAL(const size_t n, const double *w): RND(gsl_rng_taus), k(n), next_indx(0), weights(NULL)
{
		
	weights = new double[k];
	if( !weights )
	{
		cerr << "RND_DISCRETE_GENERAL::Unable to allocate memory for weights" << endl;
		gsl_rng_free(rng);
		return;
	}
	
	tmp_weights = new double[k];
	if( !tmp_weights )
	{
		delete []weights;
		cerr << "RND_DISCRETE_GENERAL::Unable to allocate memory for weights" << endl;
		gsl_rng_free(rng);
		return;
	}
	
	for( unsigned i = 0; i < k; i++ )
		tmp_weights[i] = weights[i] = w[i];
	
	rndgen = gsl_ran_discrete_preproc(k, weights);
	
	if( !rndgen )
	{
		cerr << "RND_DISCRETE_GENERAL::Unable to allocate the Distribution Generator" << endl;
		delete []weights;
		delete []tmp_weights;
		return;
	}
	
		
}




void RND_DISCRETE_GENERAL::SetWeights(const double *w)
{
	if( rndgen )
		gsl_ran_discrete_free(rndgen);
	
	rndgen = gsl_ran_discrete_preproc(k, w);
	
	if( !rndgen )
	{
		cerr << "RND_DISCRETE_GENERAL::SetWeights : Unable to allocate the Generator" << endl;
		delete []weights;
		delete []tmp_weights;
		return;
	}
	
	for( unsigned i = 0; i < k; i++ )
		tmp_weights[i] = weights[i] = w[i];
}

void RND_DISCRETE_GENERAL::UpdateGenerator()
{
	if( rndgen )
		gsl_ran_discrete_free(rndgen);
	
	RND::Reset();
	rndgen = gsl_ran_discrete_preproc(k, weights);
	
	if( !rndgen )
	{
		cerr << "RND_DISCRETE_GENERAL::SetWeights : Unable to allocate the Generator" << endl;
		delete []weights;
		delete []tmp_weights;
		return;
	}
	
	for( size_t i = 0; i < k; i++ )
	{
		indx[i] = i;
		tmp_weights[i] = weights[i];
	}
	
	next_indx = 0;
}

RND_DISCRETE_GENERAL::~RND_DISCRETE_GENERAL()
{
	if(weights)
	{
		delete []weights;
		delete []tmp_weights;
		gsl_ran_discrete_free(rndgen);
	}
	
	
}


