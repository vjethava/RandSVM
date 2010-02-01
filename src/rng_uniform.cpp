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
#include "rng_uniform.h"

RNG_UNIFORM::RNG_UNIFORM(const gsl_rng_type *T): RNG(T)
{
}

RNG_UNIFORM::RNG_UNIFORM(const gsl_rng_type *T, const unsigned long seed): RNG(T, seed)
{
}

RNG_UNIFORM::~RNG_UNIFORM()
{
}

long double RNG_UNIFORM::Get()
{
	if(r) 
		return (long double)gsl_rng_uniform(r);
}


