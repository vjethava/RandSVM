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
#ifndef RND_SVMLight_H
#define RND_SVMLight_H

/**
@author Krishnan S
*/
#include <iostream>
#include "rnd_svm.h"

class RND_SVMLight: public RND_SVM
{
	protected:
		string mfile, ofile, afile;
		string learner_command, learner_options;
		string classifier_command;
		problem_type type;
		double eps;
		char **learner_argv, **classifier_argv;
		double *alphas;
		void InitializeLearnerArgs();
		void InitializeClassifierArgs();
		
	public:
		RND_SVMLight(const string , const string, const string, const string, const string, const string, const ulong, const size_t, const size_t, const double, const size_t, SAMPLER &, const string = "", const string = "", const string = "", problem_type t = classification, double ep = -1);
		virtual void Solve();
		virtual size_t FindViolators(const bool *);
		~RND_SVMLight();
};

#endif
