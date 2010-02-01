/**************************************************************************
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
#include "rnd.h"
#include <iostream>

using namespace std;

RND::RND(const gsl_rng_type *T)
{
	rng = gsl_rng_alloc(T);
	
	if( !rng )
	{
		cerr << "RND_DISCRETE_GENERAL::Unable to allocate memory for the RNG" << endl;
		return;
	}
	seed = (unsigned long)time(NULL);
	gsl_rng_set(rng, seed );	
}

void RND::Reset()
{
	seed = (unsigned long)time(NULL);
	gsl_rng_set(rng, seed);
}

RND::~RND()
{
	if( rng )
		gsl_rng_free(rng);
}


