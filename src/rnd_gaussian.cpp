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
#include<iostream>
#include "rnd_gaussian.h"

using namespace std;

/*RND_GAUSSIAN::RND_GAUSSIAN() : RND(), mean(0), sigma(1)
{
	rng = gsl_rng_alloc(gsl_rng_taus);
	
	if( !rng )
	{
		cerr << "RND_GAUSSIAN::Unable to allocate memory for the RNG" << endl;
		return;
	}
	gsl_rng_set(rng, (unsigned long)time(NULL) );	
}

RND_GAUSSIAN::RND_GAUSSIAN(gsl_rng *r): RND(r), mean(0), sigma(1)
{
}

RND_GAUSSIAN::RND_GAUSSIAN(double m, double s): RND(), mean(m), sigma(s)
{
	rng = gsl_rng_alloc(gsl_rng_taus);
	
	if( !rng )
	{
		cerr << "RND_GAUSSIAN::Unable to allocate memory for the RNG" << endl;
		return;
	}	
	gsl_rng_set(rng, (unsigned long)time(NULL) );	
}

RND_GAUSSIAN::RND_GAUSSIAN(gsl_rng *r, double m, double s): RND(r), mean(m), sigma(s)
{
}

double RND_GAUSSIAN::Get()
{
	return mean + gsl_ran_gaussian(rng, sigma);
}

RND_GAUSSIAN::~RND_GAUSSIAN()
{
}*/


