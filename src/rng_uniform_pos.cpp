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
#include "rng_uniform_pos.h"

RNG_UNIFORM_POS::RNG_UNIFORM_POS(const gsl_rng_type *T): RNG_UNIFORM(T)
{
}

RNG_UNIFORM_POS::RNG_UNIFORM_POS(const gsl_rng_type *T, const unsigned long seed): RNG_UNIFORM(T,seed)
{
}

RNG_UNIFORM_POS::~RNG_UNIFORM_POS()
{
}

long double RNG_UNIFORM_POS::Get()
{
	if( r )
		return (long double)gsl_rng_uniform_pos(r);
}
		


