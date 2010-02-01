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
#ifndef LPSOLVE_H
#define LPSOLVE_H
// ORIGINAL CODE 
// #include <lpsolve/lp_lib.h>
#include "lp_lib.h" 
#include <iostream>
#include "randsvm-types.h"
/**
@author Krishnan S
*/
using namespace std;
/*class LPSolve{
	REAL *soln;
	REAL *obj;
	ulong no_of_features, no_of_constraints;
public:
	LPSolve(ulong no_of_constraints, ulong no_of_features);
	void Learn(string ifile);
	ulong Classify(string ifile, bool *categories);
	void SaveModel(string mfile);
	~LPSolve();
};*/

#endif
