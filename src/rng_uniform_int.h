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
#ifndef RNG_UNIFORM_INT_H
#define RNG_UNIFORM_INT_H

#include <rng.h>

/**
@author Krishnan S
*/
class RNG_UNIFORM_INT: public RNG
{
	protected:
		unsigned long max;
	public:
		RNG_UNIFORM_INT(const gsl_rng_type *, const unsigned long );
		RNG_UNIFORM_INT(const gsl_rng_type *, const unsigned long, const unsigned long );
		virtual long double Get();
		~RNG_UNIFORM_INT();
};


#endif
