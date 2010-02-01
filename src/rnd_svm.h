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
#ifndef RND_SVM_H
#define RND_SVM_H

/**
@author Krishnan S
*/
#include <fstream>
#include "sampler.h"

class RND_SVM
{
	private:
		std::ofstream svout, wout, vout;
		void DumpViolators();
 		void DumpWeights();
 		void DumpSV();	
	protected:
		SAMPLER &s;
		ulong no_of_variables;
		unsigned long long memory_size;
		size_t no_of_constraints, k, sample_size, no_of_sv, no_of_violators;
		bool use_all_avail_mem;
		string ifile, sfile, sv_file, weights_file, violators_file, classifier_input_file;
		double err;
		bool solver_failed;
		bool *violators;
		int *categories;
		bool *sv;
		void WriteSampleFile(const bool *);
	public:
		RND_SVM(const string , const ulong, const size_t, const size_t, const size_t , double err, SAMPLER &, const string = "", const string = "", const string = "");
		void sortSVIndices(ulong *, ulong );
		virtual void LearnModel();
		virtual void Solve() = 0 ;
		virtual size_t FindViolators(const bool *) = 0;
		void print_timing(const string &);
		~RND_SVM();
		
};

#endif
